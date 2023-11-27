#include <Avatar.h>
#include "Whisper.h"
#include "rootCA.h"

extern m5avatar::Avatar avatar;
extern String openai_apikey;
extern uint16_t https_timeout;

// 送信データ
// form_data
//     header_data
//     record_data(header_data, body_data)
//     footer_data
const String boundary = "123456789000000000000987654321";
const String header_data = ""\
    "--" + boundary + "\r\n"\
    "Content-Disposition: form-data; name=\"model\"\r\n\r\nwhisper-1\r\n"\
    "--" + boundary + "\r\n"\
    "Content-Disposition: form-data; name=\"language\"\r\n\r\nja\r\n"\
    "--" + boundary + "\r\n"\
    "Content-Disposition: form-data; name=\"file\"; filename=\"speak.wav\"\r\n"\
    "Content-Type: application/octet-stream\r\n\r\n";
const String footer_data = "\r\n--" + boundary + "--\r\n";

const size_t header_size = header_data.length();
constexpr size_t record_header_size = 44;
constexpr size_t record_number = 400;
constexpr size_t record_length = 150;
constexpr size_t record_body_size = record_number * record_length * sizeof(int16_t);
const size_t footer_size = footer_data.length();
constexpr size_t record_samplerate = 16000;
const size_t form_data_size = header_size + record_header_size + record_body_size + footer_size + 1;
constexpr size_t fileSizeMinus8 = record_header_size + record_body_size - 8;

void MakeHeader(byte* header) {
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    header[4] = (byte)(fileSizeMinus8 & 0xFF);
    header[5] = (byte)((fileSizeMinus8 >> 8) & 0xFF);
    header[6] = (byte)((fileSizeMinus8 >> 16) & 0xFF);
    header[7] = (byte)((fileSizeMinus8 >> 24) & 0xFF);
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    header[16] = 0x10;    // linear PCM
    header[17] = 0x00;
    header[18] = 0x00;
    header[19] = 0x00;
    header[20] = 0x01;    // linear PCM
    header[21] = 0x00;
    header[22] = 0x01;    // monoral
    header[23] = 0x00;
    header[24] = 0x80;    // sampling rate 16000
    header[25] = 0x3E;
    header[26] = 0x00;
    header[27] = 0x00;
    header[28] = 0x00;    // Byte/sec = 16000x2x1 = 32000
    header[29] = 0x7D;
    header[30] = 0x00;
    header[31] = 0x00;
    header[32] = 0x02;    // 16bit monoral
    header[33] = 0x00;
    header[34] = 0x10;    // 16bit
    header[35] = 0x00;
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    header[40] = (byte)(record_body_size & 0xFF);
    header[41] = (byte)((record_body_size >> 8) & 0xFF);
    header[42] = (byte)((record_body_size >> 16) & 0xFF);
    header[43] = (byte)((record_body_size >> 24) & 0xFF);
}

Whisper::Whisper() {
    form_data = (byte*)heap_caps_malloc(form_data_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    memset(form_data, '\0', form_data_size);
    memcpy(form_data, header_data.c_str(), header_size);
    MakeHeader(form_data + header_size);
    memcpy(form_data + header_size + record_header_size + record_body_size, footer_data.c_str(), footer_size);
}

Whisper::~Whisper() {
    heap_caps_free(form_data);
    form_data = nullptr;
}

void Whisper::record() {
    M5.Log.println("録音開始");
    uint32_t start_time = millis();
    M5.Mic.begin();

    auto wavData = (int16_t*)&form_data[header_size + record_header_size];
    for (int rec_record_idx = 0; rec_record_idx < record_number; ++rec_record_idx) {
        auto data = &wavData[rec_record_idx * record_length];
        M5.Mic.record(data, record_length, record_samplerate);
    }

    M5.Mic.end();
    M5.Log.printf("録音終了(%.1f秒)\n", (millis() - start_time) / 1000.0);
}

String Whisper::transcriptions() {
    M5.Log.println("Whisper：開始");
    uint32_t start_time = millis();

    HTTPClient https;
    https.setTimeout(https_timeout);
    if (!https.begin(url, root_ca_openai)) {
        M5.Log.println("Whisper：接続失敗");
        return "";
    }

    https.addHeader("Authorization", "Bearer " + openai_apikey);
    https.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

    int http_code = https.POST(form_data, form_data_size);
    if (!(http_code == HTTP_CODE_OK)) {
            M5.Log.printf("Whisper：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
            avatar.setSpeechText("たいむあうと");
            avatar.setExpression(m5avatar::Expression::Doubt);
            delay(2000);
            return "";
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
            M5.Log.println("Whisper：JSONシリアル化エラー");
            return "";
    }
    const String text = doc["text"];
    doc.clear();

    M5.Log.printf("Whisper：%s(%.1f秒)\n", text.c_str(), (millis() - start_time) / 1000.0); 
    return text;
}

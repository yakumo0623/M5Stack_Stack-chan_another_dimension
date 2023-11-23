#include <Avatar.h>
#include "VoiceVox.h"
#include "rootCA.h"
#include "MyFunction.h"

extern m5avatar::Avatar avatar;
extern String voicevox_apikey;
extern uint8_t config_speaker;
extern uint16_t https_timeout;

TaskHandle_t voicevox_task_handle;

void voicevox_task_loop(void *args) {
    VoiceVox *ptr = static_cast<VoiceVox*>(args);
    for (;;) {
        if (ptr->is_talking && ptr->mp3->isRunning()) {
            float level = abs(*(ptr->out->getBuffer()));
            if(level < 100) { level = 0; }
            if(level > 15000) { level = 15000; }
            avatar.setMouthOpenRatio(level / 15000.0);
            if (!ptr->mp3->loop()) {
                ptr->is_talking = false;
                ptr->mp3->stop();
                delete ptr->mp3;
                delete ptr->out;
                delete ptr->file;
                delete ptr->buff;
                ptr->mp3 = nullptr;
                ptr->out = nullptr;
                ptr->file = nullptr;
                ptr->buff = nullptr;
                avatar.setMouthOpenRatio(0);
                M5.Speaker.end();
                M5.Mic.begin();
                log_free_size("VOICEVOX：OUT");
                M5.Log.println("発話：終了");
            }
        }
        vTaskDelay(1);
    }
}

VoiceVox::VoiceVox() {
    is_talking = false;
    xTaskCreateUniversal(voicevox_task_loop, "voicevox_task_loop", 4096, this, 1, &voicevox_task_handle, APP_CPU_NUM);
}

VoiceVox::~VoiceVox() {
    vTaskDelete(voicevox_task_handle);
}

String VoiceVox::synthesis(String text) {
    M5.Log.println("VOICEVOX：開始");
    uint32_t start_time = millis();

    https.setTimeout(https_timeout);
    if (!https.begin(url, root_ca_voicevox)) {
        M5.Log.println("VOICEVOX：接続失敗");
        return "";
    }

    https.addHeader("content-type", "application/x-www-form-urlencoded");

    const String send_data = "key=" + voicevox_apikey + "&speaker=" + config_speaker + "&text=" + text;

    int http_code = https.POST(send_data.c_str());
    if (!(http_code == HTTP_CODE_OK)) {
        M5.Log.printf("VOICEVOX：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
        avatar.setSpeechText("たいむあうと");
        avatar.setExpression(m5avatar::Expression::Doubt);
        delay(2000);
        return "";
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        M5.Log.println("VOICEVOX：JSONシリアル化エラー");
        return "";
    }

    const String mp3_url = doc["mp3StreamingUrl"].as<String>();
    doc.clear();
    M5.Log.printf("VOICEVOX：%s(%.1f秒)\n", mp3_url.c_str(), (millis() - start_time) / 1000.0);
    M5.Log.println("VOICEVOX：終了");
    return mp3_url;
}

void VoiceVox::talk(String url) {
    M5.Log.println("発話：開始");
    mp3 = new AudioGeneratorMP3();
    out = new AudioOutputM5Speaker(&M5.Speaker);
    file = new AudioFileSourceHTTPSStream(url.c_str(), root_ca_voicevox);
    buff = new AudioFileSourceBuffer(file, 1024 * 10);
    M5.Mic.end();
    M5.Speaker.begin();
    is_talking = true;
    mp3->begin(buff, out);
}

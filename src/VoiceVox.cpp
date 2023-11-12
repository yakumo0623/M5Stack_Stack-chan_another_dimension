#include <Avatar.h>
#include "VoiceVox.h"
#include "rootCA.h"

extern m5avatar::Avatar avatar;
extern String voicevox_apikey;
extern uint8_t config_speaker;
extern uint16_t https_timeout;

VoiceVox::VoiceVox() {
}

VoiceVox::~VoiceVox() {
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
    M5.Log.printf("VOICEVOX：%s(%.1f秒)\n", mp3_url.c_str(), (millis() - start_time) / 1000.0);
    M5.Log.println("VOICEVOX：終了");
    return mp3_url;
}

void VoiceVox::talk(String url) {
     M5.Log.println("発話：開始");
    uint32_t start_time = millis();
    mp3 = new AudioGeneratorMP3();
    file = new AudioFileSourceHTTPSStream(url.c_str(), root_ca_voicevox);
    buff = new AudioFileSourceBuffer(file, 1024 * 10);
    out = new AudioOutputM5Speaker(&M5.Speaker);

    M5.Mic.end();
    M5.Speaker.begin();
    mp3->begin(buff, out);

    int level = 0;
    for (;;) {
        if (mp3->isRunning()) {
            level = abs(*out->getBuffer());
            if(level<100) level = 0;
            if(level > 15000)
            {
            level = 15000;
            }
            avatar.setMouthOpenRatio((float)level/15000.0);

            if (!mp3->loop()) {
                avatar.setMouthOpenRatio(0);
                mp3->stop();
                M5.Log.printf("発話：終了(%.1f秒)\n", (millis() - start_time) / 1000.0);
                delete out;
                delete buff;
                delete file;
                delete mp3;
                break;
            }
            delay(1);
        }
    }
    M5.Speaker.end();
    M5.Mic.begin();
}

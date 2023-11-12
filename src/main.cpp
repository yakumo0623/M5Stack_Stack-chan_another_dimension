#include <SD.h>
#include <M5Unified.h>
#include <Avatar.h>
#include <regex>
#include <ESPAsyncWebServer.h>
#include <nvs.h>
#include "Whisper.h"
#include "ChatGPT.h"
#include "VoiceVox.h"
#include "RootCA.h"
#include "Html.h"
#include "Weather.h"

using namespace m5avatar;
Avatar avatar;

AsyncWebServer server(80);

// 初期値
uint8_t display_brightness = 120;
String ssid = "1234";             // WIFIのSSIDダミー
String password = "1234";         // WIFIのパスワードダミー
String openai_apikey = "1234";    // OPENAIのAPIキーダミー
String voicevox_apikey = "1234";  // VOICEVOXのAPIキーダミー
uint8_t config_volume = 100;
uint8_t config_speaker = 3;
uint8_t config_color1_red = 0;
uint8_t config_color1_green = 0;
uint8_t config_color1_blue = 0;
uint8_t config_color2_red = 255;
uint8_t config_color2_green = 255;
uint8_t config_color2_blue = 255;
uint8_t config_color3_red = 248;
uint8_t config_color3_green = 171;
uint8_t config_color3_blue = 166;
String config_tone = "やさしい";
String config_age = "若者";
String config_first_person = "わたし";
String config_second_person = "あなた";
String config_weather_code = "130000";

// ネットワーク接続
void connect_wifi() {
    M5.Log.println("WIFI接続開始");
    avatar.setSpeechText("せつぞくかいし");
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);    
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        M5.Log.println("WIFI接続中");
        avatar.setSpeechText("せつぞくちゅう …");
        delay(500);     
    }
    M5.Log.printf("WIFI接続成功(%s)\n", WiFi.localIP().toString());
    avatar.setSpeechText("せつぞくせいこう");
    delay(500);
    avatar.setSpeechText("");
}

// アバターの表情
std::regex regex_Happy("ポジティブ");
std::regex regex_Doubt("ネガティブ");
std::regex regex_parentheses("（(.*?)）");
std::smatch regex_match;
std::string set_expression(std::string text) {
    if (text == "") { return ""; }
    if (std::regex_search(text, regex_match, regex_Doubt)) {
        avatar.setExpression(Expression::Doubt);
        M5.Log.println("感情（ネガティブ）");
    } else if (std::regex_search(text, regex_match, regex_Happy)) {
        avatar.setExpression(Expression::Happy);
        M5.Log.println("感情（ポジティブ）");
    } else {
        avatar.setExpression(Expression::Neutral);
        M5.Log.println("感情（ニュートラル）");
    }
    std::string text2 = std::regex_replace(text, regex_parentheses, "");
    M5.Log.printf("トリミング：%s\n", text2.c_str());
    return text2;
}

// アバターの色
void set_avatar_color() {
    ColorPalette cp;
    cp.set(COLOR_BACKGROUND, M5.Lcd.color565(config_color1_red, config_color1_green, config_color1_blue));
    cp.set(COLOR_PRIMARY, M5.Lcd.color565(config_color2_red, config_color2_green, config_color2_blue));
    cp.set(COLOR_SECONDARY, M5.Lcd.color565(config_color3_red, config_color3_green, config_color3_blue));
    cp.set(COLOR_BALLOON_FOREGROUND, M5.Lcd.color565(config_color1_red, config_color1_green, config_color1_blue));
    cp.set(COLOR_BALLOON_BACKGROUND, M5.Lcd.color565(config_color2_red, config_color2_green, config_color2_blue));
    avatar.setColorPalette(cp);
}

// 設定情報の保存
void set_nvs_config() {
    nvs_handle_t nvs;
    M5.Log.println("NVS：設定情報の保存開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READWRITE, &nvs);
    if (openResult == ESP_OK) {
        nvs_set_u8(nvs, "volume", config_volume);
        nvs_set_u8(nvs, "speaker", config_speaker);
        nvs_set_str(nvs, "tone", config_tone.c_str());
        nvs_set_str(nvs, "age", config_age.c_str());
        nvs_set_str(nvs, "first_person", config_first_person.c_str());
        nvs_set_str(nvs, "second_person", config_second_person.c_str());
        nvs_set_u8(nvs, "color1_red", config_color1_red);
        nvs_set_u8(nvs, "color1_green", config_color1_green);
        nvs_set_u8(nvs, "color1_blue", config_color1_blue);
        nvs_set_u8(nvs, "color2_red", config_color2_red);
        nvs_set_u8(nvs, "color2_green", config_color2_green);
        nvs_set_u8(nvs, "color2_blue", config_color2_blue);
        nvs_set_u8(nvs, "color3_red", config_color3_red);
        nvs_set_u8(nvs, "color3_green", config_color3_green);
        nvs_set_u8(nvs, "color3_blue", config_color3_blue);
        M5.Log.printf("NVS：設定情報の保存成功(%d %d %s %s %s %s %d %d %d %d %d %d %d %d %d)\n", config_volume, config_speaker,
            config_tone.c_str(), config_age.c_str(), config_first_person.c_str(), config_second_person.c_str(), 
            config_color1_red, config_color1_green, config_color1_blue, config_color2_red, config_color2_green, config_color2_blue,
            config_color3_red, config_color3_green, config_color3_blue);
        avatar.setSpeechText("せっていへんこう");
    } else {
        M5.Log.println("NVS：設定情報の保存失敗");
    }
    nvs_close(nvs);
    delay(1000);
    avatar.setSpeechText("");
}

// APIKEY情報の保存
void set_nvs_apikey() {
    nvs_handle_t nvs;
    M5.Log.println("NVS：APIKEYの保存開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READWRITE, &nvs);
    if (openResult == ESP_OK) {
        nvs_set_str(nvs, "openai", openai_apikey.c_str());
        nvs_set_str(nvs, "voicevox", voicevox_apikey.c_str());
        M5.Log.printf("NVS：APIKEYの保存成功(%s %s)\n", openai_apikey.c_str(), voicevox_apikey.c_str());
        avatar.setSpeechText("せっていへんこう");
    } else {
        M5.Log.println("NVS：APIKEYの保存失敗");
    }
    nvs_close(nvs);
    delay(1000);
    avatar.setSpeechText("");
}

// WIFI情報の保存
void set_nvs_wifi() {
    nvs_handle_t nvs;
    M5.Log.println("NVS：WIFIの保存開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READWRITE, &nvs);
    if (openResult == ESP_OK) {
        nvs_set_str(nvs, "ssid", ssid.c_str());
        nvs_set_str(nvs, "password", password.c_str());
        M5.Log.printf("NVS：WIFIの保存成功(%s %s)\n", ssid.c_str(), password.c_str());
        avatar.setSpeechText("せっていへんこう");
    } else {
        M5.Log.println("NVS：WIFIの保存失敗");
    }
    nvs_close(nvs);
    delay(1000);
    avatar.setSpeechText("");
}

// 設定情報の読み込み
void get_nvs_config() {
    nvs_handle_t nvs;
    char value[256];
    size_t length;
    M5.Log.println("NVS：設定情報の読み込み開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READONLY, &nvs);
    if (openResult == ESP_OK) {
        nvs_get_u8(nvs, "volume", &config_volume);
        nvs_get_u8(nvs, "speaker", &config_speaker);
        if (nvs_get_str(nvs, "tone", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "tone", value, &length);
            config_tone = String(value);
        }
        if (nvs_get_str(nvs, "age", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "age", value, &length);
            config_age = String(value);
        }
        if (nvs_get_str(nvs, "first_person", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "first_person", value, &length);
            config_first_person = String(value);
        }    
        if (nvs_get_str(nvs, "second_person", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "second_person", value, &length);
            config_second_person = String(value);
        }   
        nvs_get_u8(nvs, "color1_red", &config_color1_red);
        nvs_get_u8(nvs, "color1_green", &config_color1_green);
        nvs_get_u8(nvs, "color1_blue", &config_color1_blue);
        nvs_get_u8(nvs, "color2_red", &config_color2_red);
        nvs_get_u8(nvs, "color2_green", &config_color2_green);
        nvs_get_u8(nvs, "color2_blue", &config_color2_blue);
        nvs_get_u8(nvs, "color3_red", &config_color3_red);
        nvs_get_u8(nvs, "color3_green", &config_color3_green);
        nvs_get_u8(nvs, "color3_blue", &config_color3_blue);
        M5.Log.printf("NVS：設定情報の読み込み成功(%d %d %s %s %s %s %d %d %d %d %d %d %d %d %d)\n", config_volume, config_speaker,
            config_tone.c_str(), config_age.c_str(), config_first_person.c_str(), config_second_person.c_str(), 
            config_color1_red, config_color1_green, config_color1_blue, config_color2_red, config_color2_green, config_color2_blue,
            config_color3_red, config_color3_green, config_color3_blue);
    } else {
        M5.Log.println("NVS：設定情報の読み込み失敗");
    }
    nvs_close(nvs);
}

// 読み込み時にAPIKEYを全て表示しないためのマスキング
String mask(String value) {
    if (value.length() < 4) { return ""; }
    String prefix = value.substring(0, 2);
    String suffix = value.substring(value.length() - 2);
    String asterisk = "";
    for (int i=0; i<(value.length() - 4); i++) { asterisk += "*"; }
    return prefix + asterisk + suffix;
}

// APIKEY情報の読み込み
void get_nvs_apikey() {
    nvs_handle_t nvs;
    char value[256];
    size_t length;
    M5.Log.println("NVS：APYKEYの読み込み開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READONLY, &nvs);
    if (openResult == ESP_OK) {
        if (nvs_get_str(nvs, "openai", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "openai", value, &length);
            openai_apikey = String(value);
        }
        if (nvs_get_str(nvs, "voicevox", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "voicevox", value, &length);
            voicevox_apikey = String(value);
        } 
        M5.Log.printf("NVS：APYKEYの読み込み成功(%s %s)\n", mask(openai_apikey).c_str(), mask(voicevox_apikey).c_str());
    } else {
        M5.Log.println("NVS：APYKEYの読み込み失敗");
    }
    nvs_close(nvs);
}

// WIFI情報の読み込み
void get_nvs_wifi() {
    nvs_handle_t nvs;
    char value[256];
    size_t length;
    M5.Log.println("NVS：WIFIの読み込み開始");
    esp_err_t openResult = nvs_open("MyConfig", NVS_READONLY, &nvs);
    if (openResult == ESP_OK) {
        if (nvs_get_str(nvs, "ssid", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "ssid", value, &length);
            ssid = String(value);
        }
        if (nvs_get_str(nvs, "password", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "password", value, &length);
            password = String(value);
        } 
        M5.Log.printf("NVS：WIFIの読み込み成功(%s %s)\n", mask(ssid).c_str(), mask(password).c_str());
    } else {
        M5.Log.println("NVS：WIFIの読み込み失敗");
    }
    nvs_close(nvs);
}

// RGB文字列を赤緑青に分解
void hex_to_dec(String hexString, uint8_t* red, uint8_t* green, uint8_t* blue) {
    char* endPtr;
    *red = strtol(hexString.substring(1, 3).c_str(), &endPtr, 16);
    *green = strtol(hexString.substring(3, 5).c_str(), &endPtr, 16);
    *blue = strtol(hexString.substring(5, 7).c_str(), &endPtr, 16);
}

// 空きメモリをシリアル出力
void log_free_size(const char* text) {
    M5.Log.printf("%s メモリ残/最大ブロック残（DEFAULT->DMA）：%4dKB/%4dKB %3dKB/%3dKB\n", text, 
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) / 1024, 
        heap_caps_get_free_size(MALLOC_CAP_DMA) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DMA) / 1024);
}

// SDカードがある場合、wifi.txtの設定を読み込む
void get_sdcard_wifi() {
    if (SD.begin(GPIO_NUM_4, SPI, 25000000)) {
        ssid = "";
        password = "";
        auto fs = SD.open("/wifi.txt", FILE_READ);
        if(fs) {
            while (fs.available()) {
                char currentChar = fs.read();
                if (currentChar == '\r' || currentChar == '\n') {
                    break;
                }
                ssid += currentChar;
            }
            while (fs.available()) {
                char currentChar = fs.read();
                if (currentChar != '\r' && currentChar != '\n') {
                    password += currentChar;
                }
            }
            set_nvs_wifi();
        }
        fs.close();
    }
}

String execute_whisper() {
    log_free_size("Whisper");
    avatar.setSpeechText("ききとりちゅう …");
    Whisper* stt = new Whisper();
    stt->record();
    avatar.setSpeechText("もじおこしちゅう …");
    String return_string = stt->transcriptions();
    delete stt;
    return return_string;
}

String execute_chatgpt(String text) {
    if (text == "") { return ""; }
    log_free_size("ChatGPT");
    avatar.setSpeechText("かんがえちゅう …");
    ChatGPT* chat = new ChatGPT();
    String return_string = chat->completions(text);
    delete chat;
    return return_string;
}

void execute_voicevox(String text) {
    if (text == "") { return; }
    log_free_size("VOICEVOX");
    avatar.setSpeechText("すぅー …");
    VoiceVox* tts = new VoiceVox();
    String return_string = tts->synthesis(text);
    if (return_string != "") {
        avatar.setSpeechText("おはなしちゅう …");
        tts->talk(return_string);
    }
    delete tts;
}

String today_weather;
String tomorrow_weather;

void execute_weather() {
    log_free_size("Weather");
    Weather* weather = new Weather();
    weather->report();
    delete weather;
}

void setup() {
    get_nvs_config();
    get_nvs_wifi();
    get_nvs_apikey();

    auto cfg = M5.config();
    M5.begin(cfg);

    auto spk_cfg = M5.Speaker.config();
    M5.Speaker.config(spk_cfg);
    M5.Speaker.setVolume(config_volume);

    M5.Display.setBrightness(display_brightness);

    avatar.setBatteryIcon(true);
    avatar.setSpeechFont(&fonts::efontJA_16_b);
    avatar.init(8);
    set_avatar_color();

    get_sdcard_wifi();
    connect_wifi();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", html_root()); });
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_config()); });
    server.on("/update_config", HTTP_ANY, [](AsyncWebServerRequest *request) {
        config_volume = request->arg("volume").toInt();
        config_speaker = request->arg("speaker").toInt();
        config_tone = request->arg("tone");
        config_age = request->arg("age");
        config_first_person = request->arg("first_person");
        config_second_person = request->arg("second_person");
        hex_to_dec(request->arg("color1"), &config_color1_red, &config_color1_green, &config_color1_blue);
        hex_to_dec(request->arg("color2"), &config_color2_red, &config_color2_green, &config_color2_blue);
        hex_to_dec(request->arg("color3"), &config_color3_red, &config_color3_green, &config_color3_blue);
        M5.Speaker.setVolume(config_volume);
        set_avatar_color();
        set_nvs_config();
        request->send(200, "text/html", html_update_config());
    });
    server.on("/apikey", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_apikey()); });
    server.on("/update_apikey", HTTP_ANY, [](AsyncWebServerRequest *request) {
        openai_apikey = request->arg("openai_apikey");
        voicevox_apikey = request->arg("voicevox_apikey");
        set_nvs_apikey();
        request->send(200, "text/html", html_update_apikey());
    });
    server.onNotFound([](AsyncWebServerRequest *request){ request->send(200, "text/html", html_not_found()); });
    server.begin();

    configTime(3600 * 9, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
    avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
    execute_weather();
    log_free_size("初期化終了");
}

constexpr int duration_500 = 5000;      // 500ミリ秒
constexpr int duration_6000 = 60000;    // 60秒
constexpr int duration_9000 = 90000;    // 90秒
constexpr int duration_60000 = 600000;  // 600秒
uint32_t start_time = millis();
uint32_t talk_time = millis();
uint32_t sleepy_time = millis();
uint32_t weather_time = millis();
String week[] = {"(日)", "(月)", "(火)", "(水)", "(木)", "(金)", "(土)"};
String sleepy_text[] = {"すやすや", "むにゃむにゃ", "すーすー", "すぴー", "ふにゃふにゃ"};
String sleepy_text_selected;

void loop() {
    M5.update();

    auto count = M5.Touch.getCount();
    if (count) {
        auto t = M5.Touch.getDetail();
        if (t.wasPressed()) {
            avatar.setExpression(Expression::Happy);
            avatar.setSpeechText("");
            M5.Display.setBrightness(display_brightness);           
            talk_time = millis();
            if (t.y <= 30 && t.x >= M5.Display.width() - 30) {
                // IPアドレスを表示                
                String local_ip =  WiFi.localIP().toString();
                avatar.setSpeechText(local_ip.c_str());
                delay(3000);
                avatar.setSpeechText("");
            } else if (t.y <= M5.Display.height() / 2) {
                // 会話
                avatar.setExpression(Expression::Neutral);           
                String return_string = execute_whisper();                                 // Whisper
                return_string = execute_chatgpt(return_string);                           // ChatGPT
                return_string = String((set_expression(return_string.c_str())).c_str());  // 表情セット
                execute_voicevox(return_string);                                          // WebVoiceVox
                avatar.setSpeechText("");
            } else if (t.y > M5.Display.height() / 2 && t.x <= M5.Display.width() / 2) {
                // 現在日時を表示
                struct tm timeinfo;
                getLocalTime(&timeinfo);
                char formatted_time[6];
                sprintf(formatted_time, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
                String datetime = String(timeinfo.tm_mon + 1) + "/" + String(timeinfo.tm_mday) + week[timeinfo.tm_wday];
                datetime += " " + String(formatted_time);
                avatar.setSpeechText(datetime.c_str());
                delay(3000);
                avatar.setSpeechText("");
            } else {
                // 天気を表示
                avatar.setSpeechText("今日の天気");
                delay(1000);
                avatar.setSpeechText(today_weather.c_str());
                delay(2000);
                avatar.setSpeechText("明日の天気");
                delay(1000);
                avatar.setSpeechText(tomorrow_weather.c_str());
                delay(3000);
                avatar.setSpeechText("");
            }
        }
    }

    // バッテリー状態を更新
    if (millis() - start_time >= duration_6000) {
        avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
        start_time = millis();
    }
    // 居眠りモード
    if (avatar.getExpression() != Expression::Sleepy && millis() - talk_time >= duration_9000) {
        M5.Display.setBrightness(int(display_brightness * 0.4));
        avatar.setExpression(Expression::Sleepy);
        sleepy_text_selected = sleepy_text[random(0, sizeof(sleepy_text) / sizeof(sleepy_text[0]))];
        avatar.setSpeechText(sleepy_text_selected.c_str());
    }
    // 天気予報を更新
    if (millis() - weather_time >= duration_60000) {
        execute_weather();
        weather_time = millis();
    }
    delay(1);
}

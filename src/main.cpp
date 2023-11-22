#include <SD.h>
#include <M5Unified.h>
#include <Avatar.h>
#include <regex>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <nvs.h>
#include "Whisper.h"
#include "ChatGPT.h"
#include "VoiceVox.h"
#include "RootCA.h"
#include "Html.h"
#include "Weather.h"

using namespace m5avatar;
Avatar avatar;

VoiceVox* tts = new VoiceVox();

AsyncWebServer server(80);

// 初期値
String ssid = "1234";                      // WIFIのSSID：ダミー値
String password = "1234";                  // WIFIのパスワード：ダミー値
String openai_apikey = "1234";             // OPENAIのAPIキー：ダミー値
String voicevox_apikey = "1234";           // VOICEVOXのAPIキー：ダミー値
String config_machine_name = "stackchan";  // マシン名
uint8_t config_volume = 100;               // 音量
uint8_t config_brightness = 120;           // 明るさ
uint8_t config_word_count = 20;            // 応答文字数
uint8_t config_speaker = 3;                // 声：ずんだもん
uint8_t config_color1_red = 0;             // 背景の色
uint8_t config_color1_green = 0;           // 背景の色
uint8_t config_color1_blue = 0;            // 背景の色
uint8_t config_color2_red = 255;           // 目口の色
uint8_t config_color2_green = 255;         // 目口の色
uint8_t config_color2_blue = 255;          // 目口の色
uint8_t config_color3_red = 248;           // ほっぺの色
uint8_t config_color3_green = 171;         // ほっぺの色
uint8_t config_color3_blue = 166;          // ほっぺの色
String config_tone = "やさしい";           // 口調
String config_age = "若者";                // 年代
String config_first_person = "わたし";     // 一人称
String config_second_person = "あなた";    // 二人称
String config_weather = "130000";          // 天気：東京
uint16_t https_timeout = 20000;            // HTTPタイムアウト時間

String today_weather;     // 今日の天気
String tomorrow_weather;  // 明日の天気

constexpr int duration_500 = 500;              // 500ミリ秒
constexpr int duration_1000 = 1 * 1000;        // 1秒
constexpr int duration_60000 = 60 * 1000;      // 60秒
constexpr int duration_90000 = 90 * 1000;      // 90秒
constexpr int duration_600000 = 600 * 1000;    // 10分
constexpr int duration_1800000 = 1800 * 1000;  // 30分

uint32_t battery_time = millis();  // 前回チェック：バッテリー
uint32_t action_time = millis();   // 前回チェック：操作
uint32_t weather_time = millis();  // 前回チェック：天気予報

const String week[] = {"(日)", "(月)", "(火)", "(水)", "(木)", "(金)", "(土)"};                   // 曜日
const String sleepy_text[] = {"すやすや", "むにゃむにゃ", "すーすー", "すぴー", "ふにゃふにゃ"};  // 居眠り
const String surprised_text[] = {"あわわ", "どきっ", "びくっ"};                                   // 驚き
String sleepy_text_selected;     // 選択テキスト：居眠り
String surprised_text_selected;  // 選択テキスト：驚き
constexpr const float sleepy_threshold = 0.4;     // 閾値：居眠：明るさ
constexpr const float surprised_threshold = 3.0;  // 閾値：驚き

bool http_chatgpt_flag = false;   // ChatGPT直接実行：フラグ
String http_chatgpt_text;         // ChatGPT直接実行：テキスト
bool http_voicevox_flag = false;  // VOICEVOX直接実行：フラグ
String http_voicevox_text;        // VOICEVOX直接実行：テキスト

float ax, ay, az;  // 加速度

// 読み込み時にAPIKEYを全て表示しないためのマスキング
String mask(String value) {
    if (value.length() < 4) { return value; }
    String prefix = value.substring(0, 2);
    String suffix = value.substring(value.length() - 2);
    String asterisk = "";
    for (int i = 0; i < (value.length() - 4); i++) { asterisk += "*"; }
    return prefix + asterisk + suffix;
}

// ネットワーク接続
void connect_wifi() {
    M5.Log.println("WIFI：接続開始");
    avatar.setSpeechText("せつぞくかいし");
    M5.Log.printf("WIFI：接続情報（%s %s）\n", mask(ssid).c_str(), mask(password).c_str());
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);    
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        M5.Log.println("WIFI：接続中");
        avatar.setSpeechText("せつぞくちゅう …");
        delay(500);     
    }
    M5.Log.printf("WIFI：接続成功(%s)\n", WiFi.localIP().toString());
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
        nvs_set_str(nvs, "machine_name", config_machine_name.c_str());
        nvs_set_u8(nvs, "volume", config_volume);
        nvs_set_u8(nvs, "brightness", config_brightness);
        nvs_set_u8(nvs, "word_count", config_word_count);
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
        nvs_set_str(nvs, "weather", config_weather.c_str());
        M5.Log.printf("NVS：設定情報の保存成功(%s %d %d %d %d %s %s %s %s %d %d %d %d %d %d %d %d %d %s)\n", 
            config_machine_name.c_str(), config_volume, config_brightness, config_word_count, config_speaker,
            config_tone.c_str(), config_age.c_str(), config_first_person.c_str(), config_second_person.c_str(), 
            config_color1_red, config_color1_green, config_color1_blue, config_color2_red, config_color2_green, config_color2_blue,
            config_color3_red, config_color3_green, config_color3_blue, config_weather);
        avatar.setSpeechText("せっていへんこう");
    } else {
        M5.Log.println("NVS：設定情報の保存失敗");
    }
    nvs_close(nvs);
    delay(500);
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
    delay(500);
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
    delay(500);
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
        if (nvs_get_str(nvs, "machine_name", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "machine_name", value, &length);
            config_machine_name = String(value);
        }
        nvs_get_u8(nvs, "volume", &config_volume);
        nvs_get_u8(nvs, "brightness", &config_brightness);
        nvs_get_u8(nvs, "word_count", &config_word_count);
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
        if (nvs_get_str(nvs, "weather", 0, &length) == ESP_OK) {
            nvs_get_str(nvs, "weather", value, &length);
            config_weather = String(value);
        }
        M5.Log.printf("NVS：設定情報の読み込み成功(%s %d %d %d %d %s %s %s %s %d %d %d %d %d %d %d %d %d %s)\n", 
            config_machine_name.c_str(), config_volume, config_brightness, config_word_count, config_speaker,
            config_tone.c_str(), config_age.c_str(), config_first_person.c_str(), config_second_person.c_str(), 
            config_color1_red, config_color1_green, config_color1_blue, config_color2_red, config_color2_green, config_color2_blue,
            config_color3_red, config_color3_green, config_color3_blue, config_weather);
    } else {
        M5.Log.println("NVS：設定情報の読み込み失敗");
    }
    nvs_close(nvs);
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
        M5.Log.println("SD：カードあり");
        if (SD.exists("/wifi.txt")) {
            M5.Log.println("SD：wifi.txtあり");
            ssid = "";      // SDカードの内容を入れるためにクリア
            password = "";  // SDカードの内容を入れるためにクリア
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
                M5.Log.printf("SD：読み取り情報（%s %s）\n", ssid.c_str(), password.c_str());
                set_nvs_wifi();
            }
            fs.close();
        } else {
            M5.Log.println("SD：wifi.txtなし");
        }
    } else {
        M5.Log.println("SD：カードなし");
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
    stt = nullptr;
    return return_string;
}

String execute_chatgpt(String text) {
    if (text == "") { return ""; }
    log_free_size("ChatGPT");
    avatar.setSpeechText("かんがえちゅう …");
    ChatGPT* chat = new ChatGPT();
    String return_string = chat->completions(text);
    delete chat;
    chat = nullptr;
    return return_string;
}

String execute_voicevox(String text) {
    if (text == "") { return ""; }
    log_free_size("VOICEVOX");
    avatar.setSpeechText("すぅー …");
    String return_string = tts->synthesis(text);
    return return_string;
}

void execute_talk(String text) {
    if (text == "") { return; }
    avatar.setSpeechText("おはなしちゅう …");
    tts->talk(text);
}

void execute_weather() {
    log_free_size("Weather");
    Weather* weather = new Weather();
    weather->report();
    delete weather;
    weather = nullptr;
}

void action() {
    M5.Display.setBrightness(config_brightness);
    avatar.setExpression(Expression::Neutral);
    avatar.setSpeechText("");       
    action_time = millis();
}

void janken(String text) {
    const String gesture_text[] = {"ぐー", "ちょき", "ぱー"};
    randomSeed(millis());
    String gesture_text_selected = gesture_text[random(0, 3)];
    String speech_text = "";

    action();
    if (text == "") {
        avatar.setExpression(Expression::Doubt);
        speech_text = "？？？";            
    }
    else if (text == gesture_text_selected) {
        avatar.setExpression(Expression::Neutral);
        speech_text = gesture_text_selected + "：ひきわけ";
    }
    else if (text == "ぐー" && gesture_text_selected == "ちょき" ||
        text == "ちょき" && gesture_text_selected == "ぱー" ||
        text == "ぱー" && gesture_text_selected == "ぐー") {
            avatar.setExpression(Expression::Sad);
            speech_text = gesture_text_selected + "：まけたー";
    } else {
        avatar.setExpression(Expression::Happy);
        speech_text = gesture_text_selected + "：かったー";
    }
    avatar.setSpeechText(speech_text.c_str());
    M5.Log.printf("じゃんけん：%s %s %s\n", text.c_str(), gesture_text_selected.c_str(), speech_text.c_str());
}

void hoi(String text) {
    const String gesture_text[] = {"うえ", "ひだり", "みぎ", "した"};
    randomSeed(millis());
    String gesture_text_selected = gesture_text[random(0, 4)];
    String speech_text = "";

    action();
    if (text == "") {
        avatar.setExpression(Expression::Doubt);
        speech_text = "？？？";            
    }
    else if (text == gesture_text_selected) {
        avatar.setExpression(Expression::Sad);
        speech_text = gesture_text_selected + "：まけたー";
    } else {
        avatar.setExpression(Expression::Happy);
        speech_text = gesture_text_selected + "：せーふ";
    }
    float x = M5.Display.width() / 25;
    float y = M5.Display.height() / 25;
    if (gesture_text_selected == "うえ") { avatar.setGaze(y * -1, 0); }
    else if (gesture_text_selected == "ひだり") { avatar.setGaze(0, x * -1); }
    else if (gesture_text_selected == "みぎ") { avatar.setGaze(0, x); }
    else if (gesture_text_selected == "した") { avatar.setGaze(y, 0); }
    else { avatar.setGaze(0, 0); }
    avatar.setSpeechText(speech_text.c_str());
    M5.Log.printf("あっちむいてほい：%s %s %s\n", text.c_str(), gesture_text_selected.c_str(), speech_text.c_str());
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

    M5.Display.setBrightness(config_brightness);

    avatar.setBatteryIcon(true);
    avatar.setSpeechFont(&fonts::efontJA_16_b);
    avatar.init(8);
    set_avatar_color();

    get_sdcard_wifi();
    connect_wifi();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", html_root()); });
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_config()); });
    server.on("/update_config", HTTP_ANY, [](AsyncWebServerRequest *request) {
        config_machine_name = request->arg("machine_name");
        config_volume = request->arg("volume").toInt();
        config_brightness = request->arg("brightness").toInt();
        config_word_count = request->arg("word_count").toInt();
        config_speaker = request->arg("speaker").toInt();
        config_tone = request->arg("tone");
        config_age = request->arg("age");
        config_first_person = request->arg("first_person");
        config_second_person = request->arg("second_person");
        hex_to_dec(request->arg("color1"), &config_color1_red, &config_color1_green, &config_color1_blue);
        hex_to_dec(request->arg("color2"), &config_color2_red, &config_color2_green, &config_color2_blue);
        hex_to_dec(request->arg("color3"), &config_color3_red, &config_color3_green, &config_color3_blue);
        config_weather = request->arg("weather");
        M5.Speaker.setVolume(config_volume);
        set_avatar_color();
        M5.Display.setBrightness(config_brightness);
        set_nvs_config();
        MDNS.end();
        MDNS.begin(config_machine_name);
        execute_weather();
        request->send(200, "text/html", html_ok());
    });
    server.on("/chatgpt", HTTP_ANY, [](AsyncWebServerRequest *request) {
        http_chatgpt_flag = true;
        http_chatgpt_text = request->arg("text");
        request->send(200, "text/html", html_ok());
    });
    server.on("/voicevox", HTTP_ANY, [](AsyncWebServerRequest *request) {
        http_voicevox_flag = true;
        http_voicevox_text = request->arg("text");
        request->send(200, "text/html", html_ok());
    });
    server.on("/apikey", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_apikey()); });
    server.on("/update_apikey", HTTP_ANY, [](AsyncWebServerRequest *request) {
        openai_apikey = request->arg("openai_apikey");
        voicevox_apikey = request->arg("voicevox_apikey");
        set_nvs_apikey();
        request->send(200, "text/html", html_ok());
    });
    server.on("/janken", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_janken()); });
    server.on("/execute_janken", HTTP_ANY, [](AsyncWebServerRequest *request) {
        String text = request->arg("text");
        janken(text);
        request->send(200, "text/html", html_ok());
    });
    server.on("/hoi", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(200, "text/html", html_hoi()); });
    server.on("/execute_hoi", HTTP_ANY, [](AsyncWebServerRequest *request) {
        String text = request->arg("text");
        hoi(text);
        request->send(200, "text/html", html_ok());
    });
    server.onNotFound([](AsyncWebServerRequest *request){ request->send(200, "text/html", html_not_found()); });
    server.begin();
    MDNS.begin(config_machine_name);

    configTime(3600 * 9, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
    avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
    execute_weather();
    log_free_size("初期化終了");
}

void loop() {
    M5.update();

    auto count = M5.Touch.getCount();
    if (count) {
        auto t = M5.Touch.getDetail();
        if (t.wasPressed()) {
            action();
            if (t.y <= 30 && t.x >= M5.Display.width() - 30) {
                // マシン名とIPアドレスを表示
                avatar.setSpeechText(config_machine_name.c_str());
                delay(2000);
                String local_ip =  WiFi.localIP().toString();
                avatar.setSpeechText(local_ip.c_str());
                delay(3000);
            } else if (t.y <= M5.Display.height() / 2) {
                // 会話
                avatar.setExpression(Expression::Neutral);           
                String return_string = execute_whisper();                                 // Whisper
                return_string = execute_chatgpt(return_string);                           // ChatGPT
                return_string = String((set_expression(return_string.c_str())).c_str());  // 表情セット
                return_string = execute_voicevox(return_string);                          // WebVoiceVox
                execute_talk(return_string);                                              // 発話
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
            }
            avatar.setSpeechText("");
        }
    }

    // 直接ChatGPTを呼ぶ
    if (http_chatgpt_flag == true) {
        http_chatgpt_flag = false;
        action();
        String return_string = execute_chatgpt(http_chatgpt_text);                // ChatGPT
        return_string = String((set_expression(return_string.c_str())).c_str());  // 表情セット
        return_string = execute_voicevox(return_string);                          // WebVoiceVox
        execute_talk(return_string);                                              // 発話
        avatar.setSpeechText("");
    }

    // 直接VOICEVOXを呼ぶ
    if (http_voicevox_flag == true) {
        http_voicevox_flag = false;
        action();
        String return_string = execute_voicevox(http_voicevox_text);
        execute_talk(return_string); 
        avatar.setSpeechText("");
    }

    // バッテリー状態を更新
    if (millis() - battery_time >= duration_60000) {
        avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
        battery_time = millis();
    }

    // 居眠りモード
    if (avatar.getExpression() != Expression::Sleepy && millis() - action_time >= duration_60000) {
        M5.Display.setBrightness(int(config_brightness * sleepy_threshold));
        avatar.setExpression(Expression::Sleepy);
        sleepy_text_selected = sleepy_text[random(0, sizeof(sleepy_text) / sizeof(sleepy_text[0]))];
        avatar.setSpeechText(sleepy_text_selected.c_str());
    }

    // シェイク時の驚き
    M5.Imu.getAccel(&ax, &ay, &az);
    if (abs(ax) + abs(ay) + abs(az) > surprised_threshold && millis() - action_time >= duration_1000) {
        action();
        avatar.setExpression(Expression::Doubt);
        surprised_text_selected = surprised_text[random(0, sizeof(surprised_text) / sizeof(surprised_text[0]))];
        avatar.setSpeechText(surprised_text_selected.c_str());
    }

    // 天気予報を更新
    if (millis() - weather_time >= duration_1800000) {
        execute_weather();
        weather_time = millis();
    }
    delay(1);
}

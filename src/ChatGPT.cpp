#include <Avatar.h>
#include "ChatGPT.h"
#include "rootCA.h"

extern m5avatar::Avatar avatar;
extern String openai_apikey;
extern uint8_t config_word_count;
extern String config_tone;
extern String config_age;
extern String config_first_person;
extern String config_second_person;
extern uint16_t https_timeout;

constexpr int chat_history_max = 5;
std::deque<String> chat_history;

// ChatGPTの応答文に改行があった場合に取り除く
String remove_newline(const String& input) {
    String result;
    for (int i = 0; i < input.length(); i++) {
        char currentChar = input.charAt(i);
        if (currentChar != '\r' && currentChar != '\n') {
            result += currentChar;
        }
    }
    return result;
}

ChatGPT::ChatGPT() {
    role =  "以下の役割を演じてください。";
    role += "あなたは、" + config_age + "、口調は、" + config_tone + "、";
    role += "一人称は、" + config_first_person + "、二人称は、" + config_second_person + "です。";
    role += "私の発言がポジティブの場合、（ポジティブ）を出力し、その後、共感します。";
    role += "私の発言がネガティブの場合、（ネガティブ）を出力し、その後、慰めます。";
    role += "応答の最大文字数は" + String(config_word_count) + "文字です。";
}

ChatGPT::~ChatGPT() {
}

String ChatGPT::completions(String text) {
    M5.Log.println("ChatGPT：開始");
    uint32_t start_time = millis();

    https.setTimeout(https_timeout);
    if (!https.begin(url, root_ca_openai)) {
        M5.Log.println("ChatGPT：接続失敗");
        return "";
    }

    chat_history.push_back(text);
    if (chat_history.size() > chat_history_max) {
        chat_history.pop_front();
        chat_history.pop_front();
    }

    https.addHeader("Authorization", "Bearer " + openai_apikey);
    https.addHeader("Content-Type", "application/json");

    String send_data = "{\"model\": \"" + model + "\", \"messages\": [";
    send_data += "{\"role\": \"system\", \"content\":\"" + role + "\"}";
    M5.Log.printf("  system：%s\n", role.c_str());
    for (int i=0; i<=chat_history.size()-1; i++) {
        String role_type = (i % 2 == 0) ? "user" : "assistant";
        send_data += ", {\"role\": \"" + role_type + "\", \"content\":\"" + chat_history[i] + "\"}";
        M5.Log.printf("  %s：%s\n", role_type, chat_history[i].c_str());
    }
    send_data += "]}";

    int http_code = https.POST(send_data.c_str());
    if (!(http_code == HTTP_CODE_OK)) {
        M5.Log.printf("ChatGPT：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
        avatar.setSpeechText("たいむあうと");
        avatar.setExpression(m5avatar::Expression::Doubt);
        chat_history.pop_back();
        delay(2000);
        return "";
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        M5.Log.println("ChatGPT：JSONシリアル化エラー");
        return "";
    }

    const String r_model = doc["model"];
    const String content = doc["choices"][0]["message"]["content"];
    const String prompt_tokens = doc["usage"]["prompt_tokens"];
    const String completion_tokens = doc["usage"]["completion_tokens"];
    const String content_2 = remove_newline(content);
    chat_history.push_back(content_2);
    doc.clear();

    M5.Log.printf("ChatGPT：%s(%.1f秒)\n",content_2.c_str(), (millis() - start_time) / 1000.0); 
    M5.Log.printf("ChatGPT：(モデル：%s 入力トークン数：%s 出力トークン数：%s)\n", r_model.c_str(), prompt_tokens.c_str(), completion_tokens.c_str());
    M5.Log.println("ChatGPT：終了"); 
    return content_2;
}

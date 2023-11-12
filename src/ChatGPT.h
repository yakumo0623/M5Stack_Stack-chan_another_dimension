#ifndef _CHATGPT_H
#define _CHATGPT_H

#include <M5Unified.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <deque>

class ChatGPT {
public:
    ChatGPT();
    ~ChatGPT();
    String completions(String text);
private:
    HTTPClient https;
    const String url = "https://api.openai.com/v1/chat/completions";
    const String model = "gpt-3.5-turbo-1106";
    String role;
};

#endif

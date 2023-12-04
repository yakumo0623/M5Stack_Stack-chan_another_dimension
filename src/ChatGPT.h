#ifndef _CHATGPT_H
#define _CHATGPT_H

#include <M5Unified.h>
#include <Avatar.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <deque>
#include "RootCA.h"

extern m5avatar::Avatar avatar;
extern String openai_apikey;
extern uint8_t config_word_count;
extern String config_tone;
extern String config_age;
extern String config_first_person;
extern String config_second_person;
extern uint8_t config_history_count;
extern std::deque<String> chat_history;
extern uint16_t https_timeout;

class ChatGPT {
public:
    ChatGPT();
    ~ChatGPT();
    String completions(String text);
private:
    const String url = "https://api.openai.com/v1/chat/completions";
    const String model = "gpt-3.5-turbo-1106";
    String role;
};

#endif

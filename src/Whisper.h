#ifndef _WHISPER_H
#define _WHISPER_H

#include <M5Unified.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Whisper {
public:
    Whisper();
    ~Whisper();
    void record();
    String transcriptions();
private:
    const String url = "https://api.openai.com/v1/audio/transcriptions";
    byte* form_data;
};

#endif

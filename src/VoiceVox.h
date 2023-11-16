#ifndef _VOICEVOX_H
#define _VOICEVOX_H

#include <M5Unified.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include "AudioOutputM5Speaker.h"
#include "AudioFileSourceHTTPSStream.h"

class VoiceVox {
public:
    VoiceVox();
    ~VoiceVox();
    String synthesis(String text);
    void talk(String url);
    AudioGeneratorMP3 *mp3;
    AudioOutputM5Speaker *out;
    AudioFileSourceHTTPSStream *file;
    AudioFileSourceBuffer *buff;
    bool isEnd = false;
private:
    HTTPClient https;
    const String url = "https://api.tts.quest/v3/voicevox/synthesis";
};

#endif

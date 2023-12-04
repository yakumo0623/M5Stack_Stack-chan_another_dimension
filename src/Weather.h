#ifndef _WEATHER_H
#define _WEATHER_H

#include <M5Unified.h>
#include <Avatar.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "RootCA.h"

extern m5avatar::Avatar avatar;
extern String config_weather;
extern String today_weather;
extern String tomorrow_weather;

class Weather {
public:
    Weather();
    ~Weather();
    void report();
private:
    const String url = "https://www.jma.go.jp/bosai/forecast/data/forecast";
};

#endif

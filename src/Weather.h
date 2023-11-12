#ifndef _WEATHER_H
#define _WEATHER_H

#include <M5Unified.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Weather {
public:
    Weather();
    ~Weather();
    void report();
private:
    HTTPClient https;
    const String url = "https://www.jma.go.jp/bosai/forecast/data/forecast";
};

#endif

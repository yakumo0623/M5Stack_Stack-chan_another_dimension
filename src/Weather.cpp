#include <Avatar.h>
#include "Weather.h"
#include "rootCA.h"

extern m5avatar::Avatar avatar;
extern String config_weather;
extern String today_weather;
extern String tomorrow_weather;

String weather_code = R"({"100": "晴れ", "101": "晴れ時々くもり", "102": "晴れ一時雨", "103": "晴れ時々雨", "104": "晴れ一時雪", "105": "晴れ時々雪", "106": "晴れ一時雨か雪", "107": "晴れ時々雨か雪", "108": "晴れ一時雨か雷雨", "110": "晴れのち時々くもり", "111": "晴れのちくもり", "112": "晴れのち一時雨", "113": "晴れのち時々雨", "114": "晴れのち雨", "115": "晴れのち一時雪", "116": "晴れのち時々雪", "117": "晴れのち雪", "118": "晴れのち雨か雪", "119": "晴れのち雨か雷雨", "120": "晴れ朝夕一時雨", "121": "晴れ朝の内一時雨", "122": "晴れ夕方一時雨", "123": "晴れ山沿い雷雨", "124": "晴れ山沿い雪", "125": "晴れ午後は雷雨", "126": "晴れ昼頃から雨", "127": "晴れ夕方から雨", "128": "晴れ夜は雨", "129": "晴れ夜半から雨", "130": "朝の内霧後晴れ", "131": "晴れ明け方霧", "132": "晴れ朝夕くもり", "140": "晴れ時々雨で雷", "160": "晴れ一時雪か雨", "170": "晴れ時々雪か雨", "181": "晴れのち雪か雨", "200": "くもり", "201": "くもり時々晴れ", "202": "くもり一時雨", "203": "くもり時々雨", "204": "くもり一時雪", "205": "くもり時々雪", "206": "くもり一時雨か雪", "207": "くもり時々雨か雪", "208": "くもり一時雨か雷雨", "209": "霧", "210": "くもりのち時々晴れ", "211": "くもりのち晴れ", "212": "くもりのち一時雨", "213": "くもりのち時々雨", "214": "くもりのち雨", "215": "くもりのち一時雪", "216": "くもりのち時々雪", "217": "くもりのち雪", "218": "くもりのち雨か雪", "219": "くもりのち雨か雷雨", "220": "くもり朝夕一時雨", "221": "くもり朝の内一時雨", "222": "くもり夕方一時雨", "223": "くもり日中時々晴れ", "224": "くもり昼頃から雨", "225": "くもり夕方から雨", "226": "くもり夜は雨", "227": "くもり夜半から雨", "228": "くもり昼頃から雪", "229": "くもり夕方から雪", "230": "くもり夜は雪", "231": "くもり海上海岸は霧か霧雨", "240": "くもり時々雨で雷", "250": "くもり時々雪で雷", "260": "くもり一時雪か雨", "270": "くもり時々雪か雨", "281": "くもりのち雪か雨", "300": "雨", "301": "雨時々晴れ", "302": "雨時々止む", "303": "雨時々雪", "304": "雨か雪", "306": "大雨", "307": "風雨共に強い", "308": "雨で暴風", "309": "雨一時雪", "311": "雨のち晴れ", "313": "雨のちくもり", "314": "雨のち時々雪", "315": "雨のち雪", "316": "雨か雪のち晴れ", "317": "雨か雪のちくもり", "320": "朝の内雨のち晴れ", "321": "朝の内雨のちくもり", "322": "雨朝晩一時雪", "323": "雨昼頃から晴れ", "324": "雨夕方から晴れ", "325": "雨夜は晴", "326": "雨夕方から雪", "327": "雨夜は雪", "328": "雨一時強く降る", "329": "雨一時みぞれ", "340": "雪か雨", "350": "雨で雷", "361": "雪か雨のち晴れ", "371": "雪か雨のちくもり", "400": "雪", "401": "雪時々晴れ", "402": "雪時々止む", "403": "雪時々雨", "405": "大雪", "406": "風雪強い", "407": "暴風雪", "409": "雪一時雨", "411": "雪のち晴れ", "413": "雪のちくもり", "414": "雪のち雨", "420": "朝の内雪のち晴れ", "421": "朝の内雪のちくもり", "422": "雪昼頃から雨", "423": "雪夕方から雨", "424": "雪夜半から雨", "425": "雪一時強く降る", "426": "雪のちみぞれ", "427": "雪一時みぞれ", "450": "雪で雷" })";

Weather::Weather() {
}

Weather::~Weather() {
}

void Weather::report() {
    M5.Log.println("Weather：開始");
    uint32_t start_time = millis();

    https.setTimeout(30000);
    if (!https.begin(url + "/" + config_weather + ".json", root_ca_jma)) {
        M5.Log.println("Weather：接続失敗");
    }

    int http_code = https.GET();
    if (!(http_code == HTTP_CODE_OK)) {
        M5.Log.printf("Weather：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
        avatar.setSpeechText("たいむあうと");
        avatar.setExpression(m5avatar::Expression::Doubt);
        delay(2000);
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(32768);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        M5.Log.println("Weather：JSONシリアル化エラー");
    }

    DynamicJsonDocument doc_2(8192);
    DeserializationError error_2 = deserializeJson(doc_2, weather_code);
    if (error_2) {
        M5.Log.println("Weather：JSONシリアル化エラー");
    }

    struct tm timeinfo;
    String weather_code_1 = doc[0]["timeSeries"][0]["areas"][0]["weatherCodes"][0];
    String weather_code_2 = doc[0]["timeSeries"][0]["areas"][0]["weatherCodes"][1];
    String weather_code_3 = doc[0]["timeSeries"][0]["areas"][0]["weatherCodes"][2];
    String weather_1 = doc_2[weather_code_1.c_str()];
    String weather_2 = doc_2[weather_code_2.c_str()];
    String weather_3 = doc_2[weather_code_3.c_str()];
    doc.clear();
    doc_2.clear();
    
    getLocalTime(&timeinfo);
    if (timeinfo.tm_hour >= 0 && timeinfo.tm_hour <= 4) {
        today_weather = weather_2;
        tomorrow_weather = weather_3;
    } else {
        today_weather = weather_1;
        tomorrow_weather= weather_2;
    }

    M5.Log.printf("Weather：%s/%s(%.1f秒)\n",today_weather.c_str(), tomorrow_weather.c_str(), (millis() - start_time) / 1000.0); 
    M5.Log.println("Weather：終了");
}

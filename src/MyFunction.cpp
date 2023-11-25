#include "MyFunction.h"

// 読み込み時にAPIKEYを全て表示しないためのマスキング
String mask(String value) {
    if (value.length() < 4) { return value; }
    String prefix = value.substring(0, 2);
    String suffix = value.substring(value.length() - 2);
    String asterisk = "";
    for (int i = 0; i < (value.length() - 4); i++) { asterisk += "*"; }
    return prefix + asterisk + suffix;
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
    M5.Log.printf("%s メモリ残/最大ブロック残（DEFAULT->DMA->SPIRAM）：%4dKB/%4dKB %3dKB/%3dKB %4dKB/%4dKB\n", text, 
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) / 1024, 
        heap_caps_get_free_size(MALLOC_CAP_DMA) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DMA) / 1024,
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024);
}

String get_speaker_name(String speaker) {
    String name_list = R"({"2": "四国めたん", "0": "四国めたん", "6": "四国めたん", "4": "四国めたん", "36": "四国めたん", "37": "四国めたん", "3": "ずんだもん", "1": "ずんだもん", "7": "ずんだもん", "5": "ずんだもん", "22": "ずんだもん", "38": "ずんだもん", "8": "春日部つむぎ", "10": "雨晴はう", "9": "波音リツ", "65": "波音リツ", "11": "玄野武宏", "39": "玄野武宏", "40": "玄野武宏", "41": "玄野武宏", "12": "白上虎太郎", "32": "白上虎太郎", "33": "白上虎太郎", "34": "白上虎太郎", "35": "白上虎太郎", "13": "青山龍星", "14": "冥鳴ひまり", "16": "九州そら", "15": "九州そら", "18": "九州そら", "17": "九州そら", "19": "九州そら", "20": "もち子", "66": "もち子", "21": "剣崎雌雄", "23": "WhiteCUL", "24": "WhiteCUL", "25": "WhiteCUL", "26": "WhiteCUL", "27": "後鬼", "28": "後鬼", "29": "No.7", "30": "No.7", "31": "No.7", "42": "ちび式じい", "43": "櫻歌ミコ", "44": "櫻歌ミコ", "45": "櫻歌ミコ", "46": "小夜/SAYO", "47": "ナースロボ＿タイプＴ", "48": "ナースロボ＿タイプＴ", "49": "ナースロボ＿タイプＴ", "50": "ナースロボ＿タイプＴ", "51": "†聖騎士紅桜†", "52": "雀松朱司", "53": "麒ヶ島宗麟", "54": "春歌ナナ", "55": "猫使アル", "56": "猫使アル", "57": "猫使アル", "58": "猫使ビィ", "59": "猫使ビィ", "60": "猫使ビィ", "61": "中国ウサギ", "62": "中国ウサギ", "63": "中国ウサギ", "64": "中国ウサギ", "67": "栗田まろん", "68": "あいえるたん", "69": "満別花丸", "70": "満別花丸", "71": "満別花丸", "72": "満別花丸", "73": "満別花丸", "74": "琴詠ニア"})";
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, name_list);
    String speaker_name = doc[speaker];
    doc.clear();
    return speaker_name;
}

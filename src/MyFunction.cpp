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
    M5.Log.printf("%s メモリ残/最大ブロック残（DEFAULT->DMA）：%4dKB/%4dKB %3dKB/%3dKB\n", text, 
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) / 1024, 
        heap_caps_get_free_size(MALLOC_CAP_DMA) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DMA) / 1024);
}

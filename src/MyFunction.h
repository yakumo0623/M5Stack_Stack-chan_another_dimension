#ifndef _MYFUNCTION_H
#define _MYFUNCTION_H

#include <M5Unified.h>

String mask(String value);
void hex_to_dec(String hexString, uint8_t* red, uint8_t* green, uint8_t* blue);
void log_free_size(const char* text);

#endif

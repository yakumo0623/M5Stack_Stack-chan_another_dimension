#ifndef _HTML_H
#define _HTML_H

#include<M5Unified.h>

extern String config_machine_name;
extern uint8_t config_volume;
extern uint8_t config_brightness;
extern uint8_t config_word_count;
extern uint8_t config_speaker;
extern uint8_t config_color1_red;
extern uint8_t config_color1_green;
extern uint8_t config_color1_blue;
extern uint8_t config_color2_red;
extern uint8_t config_color2_green;
extern uint8_t config_color2_blue;
extern uint8_t config_color3_red;
extern uint8_t config_color3_green;
extern uint8_t config_color3_blue;
extern String config_tone;
extern String config_age;
extern String config_first_person;
extern String config_second_person;
extern String config_weather;

String html_root();
String html_not_found();
String rgb_to_string(uint8_t red, uint8_t green, uint8_t blue);
String html_config();
String html_apikey();
String html_janken();
String html_hoi();
String html_ok();

#endif

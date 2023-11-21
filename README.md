# M5Stack_Stack-chan_another_dimension

![ｽﾀｯｸﾁｬﾝ](/image/stackchan.jpg) 

## はじめに

　ｽﾀｯｸﾁｬﾝは、[ししかわさん](https://protopedia.net/prototype/2345)が開発＆公開している、 手乗りサイズのスーパーカワイイ・コミュニケーションロボットです。ｽﾀｯｸﾁｬﾝの最大の特徴は、すべてがオープンであること。（オープンな仕様／プロセス）すべてをオープンにした結果、なんと40名を超えるMakerの手によって「作ってみた」「他のロボットに組み込んでみた」などの作例が続々とアップロードされています。

　本プログラムは、[robo8080さん](https://github.com/robo8080/AI_StackChan2)が公開している「AI_StackChan2」を教本として、C++の勉強をしながら、自分が使わないものを除外して、自分が欲しい機能を入れたりして再構築した別世界のｽﾀｯｸﾁｬﾝです。

　気軽に部屋で持ち歩いたり、散歩に一緒に行ったりできるように、頭部分だけでいかに本家の可愛らしさを維持できるかを考えながら取り組んでます。

## ライセンス

　ｽﾀｯｸﾁｬﾝ および ライブラリのライセンスは下記となります。

- Apache-2.0 license
  - [ｽﾀｯｸﾁｬﾝ](https://protopedia.net/prototype/2345)
  - [nvs](https://github.com/espressif/esp-idf/blob/master/components/nvs_flash/include/nvs.h)
- MIT license
  - [M5Unified](https://github.com/m5stack/M5Unified)
  - [m5stack-avatar](https://github.com/meganetaaan/m5stack-avatar)
  - [ESPmDNS](https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS/src/ESPmDNS.h)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- GPL-3.0 license
  - [SD](https://github.com/arduino-libraries/SD)
  - [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
- LGPL-2.1 license
  - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [HTTPClient](https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h)

## 必要なAPIキー

　音声認識（Whisper）、チャット（ChatGPT）、音声合成（WEB版VOICEVOX）を使っています。そのため、利用にあたって、APIキーを取得する必要があります。

　WEB版VOICEVOXは、個人利用を想定したAPIとなっています。SNS等に動画を投稿する際は、音声ライブラリの利用規約を守る必要があります。（例えば、X(旧Twitter)であれば、「#VOICEVOX：ずんだもん(話者名)」 を入れる。など。）

- [OpenAI（音声認識、チャット）](https://openai.com/)
- [WEB版VOICEVOX（音声合成）](https://voicevox.su-shiki.com/su-shikiapis/)
  - [APIキー取得](https://su-shiki.com/api/)
  - [VOICEVOX ソフトウェア 利用規約](https://voicevox.hiroshiba.jp/term/)
  - [キャクターや音声ライブラリなどの利用規約等](https://voicevox.hiroshiba.jp/)

## 利用の流れ

1. SDカードのルートに「wifi.txt」を作成します。1行目に「SSID」、2行目に「PASSWORD」を記載します。
改行コードは、「CRLF」「CR」「LF」どれでもOKですが、「BOMなし」形式で保存する必要があります。
![BOMなし](/image/wifi_bomなし.jpg) 　![BOMあり](/image/wifi_bomあり.jpg) 

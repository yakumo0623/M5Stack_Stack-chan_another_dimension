# M5Stack_Stack-chan_another_dimension

 ![ｽﾀｯｸﾁｬﾝ](/image/stackchan.jpg)

## はじめに

　ｽﾀｯｸﾁｬﾝは、[ししかわさん](https://protopedia.net/prototype/2345)が開発＆公開している、 手乗りサイズのスーパーカワイイ・コミュニケーションロボットです。ｽﾀｯｸﾁｬﾝの最大の特徴は、すべてがオープンであること。（オープンな仕様／プロセス）すべてをオープンにした結果、40名を超えるMakerの手によって「作ってみた」「他のロボットに組み込んでみた」などの作例が続々とアップロードされています。

　本プログラムは、[robo8080さん](https://github.com/robo8080/AI_StackChan2)が公開している「AI_StackChan2」を教本として、C++の勉強をしながら、自分が使わないものを除外して、自分が欲しい機能を入れたりして再構築した別世界のｽﾀｯｸﾁｬﾝです。

　気軽に部屋で持ち歩いたり、散歩に一緒に行ったりできるように、頭部分だけでいかに本家の可愛らしさを維持できるかを考えながら取り組んでます。

　本プログラムのバグについては、あくまで自分のミスなので、本家への問い合わせは、なしでお願いします。(=^・^=)

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
   
   ![BOMなし](/image/wifi_bomなし.jpg)   ![BOMあり](/image/wifi_bomあり.jpg) 


2. 画面の右上のバッテリーアイコン付近を押すと、マシン名、IPアドレスが表示されます。
   
3. ブラウザで「http://（マシン名.local or IPアドレス）」を打つとメニュー画面が表示されます。

   ![apikey](/image/menu.jpg) 
   
4. ブラウザで「http://（マシン名.local or IPアドレス）/apikey」を打つとAPIキーの登録画面が表示されます。APIキーを入力して「登録」ボタンを押します。

   ![apikey](/image/apikey.jpg) 


4. ブラウザで「http://（マシン名.local or IPアドレス）/config」を打つと設定画面が表示されます。APIキーを入力して「登録」ボタンを押します。
   
   ![config](/image/config.jpg) 
   
   
   - マシン名：同一LAN内で複数のｽﾀｯｸﾁｬﾝを使う場合は「_001」「_002」のようにしてください。
   - 音量：スピーカーの音量です。
   - 明るさ：画面の明るさです。
   - 応答文字数：ChatGPTの応答文の文字数です。（※この制限は、守ってくれたりくれなかったり…。）
   - チャット履歴数：ChatGPTの履歴数です。（※履歴数×2-1：あなた、GPT、あなた、GPT、あなた）
   - 声：VOICEVOXの声の種類です。
   - 口調、年代、一人称、二人称：この組み合わせで疑似的に性格を変えます。
   - 色：背景、目・口、ほっぺの色です。
- 天気：天気予報の対象地域です。（※[出典：気象庁天気予報](https://www.jma.go.jp/jma/kishou/info/coment.html)）
  
5. 画面の上半分を押すと「ｽﾀｯｸﾁｬﾝとの会話」ができます。画面の左下を押すと「日付・曜日・時間」を表示します。画面の右下を押すと「今日の天気・明日の天気」を表示します。

## 付加機能

- ChatGPTの応答が、ポジティブの場合：笑顔、ネガティブの場合：困惑の表情をする。
- 90秒放置すると居眠りをする。
- シェイクすると驚く。
- ChatGPTを直接実行する。
  - 「http://（マシン名.local or IPアドレス）/chatgpt?text=テキスト文」
- VOICEVOXを直接実行する。
  - 「http://（マシン名.local or IPアドレス）/voicevox?text=テキスト文」
- テキストを表示する。
  - 「http://（マシン名.local or IPアドレス）/text?text=テキスト文&expression=(Happy/Angry/Doubt/Sad/Sleepy/Neutral)」
- 「じゃんけん」をする。
  - 「http://（マシン名.local or IPアドレス）/janken」
- 「あっちむいてほい」をする。
  - 「http://（マシン名.local or IPアドレス）/hoi」

## 特記事項

- ほっぺを付けるために、「m5stack-avatar」の「Mouth.cpp」の26行目に処理を追記しています。（※libの下にあります。）
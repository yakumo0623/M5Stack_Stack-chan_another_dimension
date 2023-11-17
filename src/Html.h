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

String html_root() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>こんにちは、スタックチャンの世界へ</title>
      </head>
      <body>こんにちは、スタックチャンの世界へ</body>
    </html>)";
}

String html_not_found() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ページが見つかりません</title>
      </head>
      <body>ページが見つかりません</body>
    </html>)";
  }

String rgb_to_string(uint8_t red, uint8_t green, uint8_t blue) {
  char c_red[3], c_green[3], c_blue[3];
  snprintf(c_red, sizeof(c_red), "%02X", red);
  snprintf(c_green, sizeof(c_green), "%02X", green);
  snprintf(c_blue, sizeof(c_blue), "%02X", blue);
  return "#" + String(c_red) + String(c_green) + String(c_blue);
}

const String html_config_string_1 = R"(
  <!DOCTYPE html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>スタックチャンの設定</title>
      <style>
        #container { position: relative; width: 160px; height: 120px; border: 1px solid black;}
        .circle { position: absolute; width: 15px; height: 15px; border-radius: 50%; }
        #circle1 { top: 40px; left: 40px; }
        #circle2 { top: 40px; right: 40px; }
        #circle3 { top: 70px; left: 20px; }
        #circle4 { top: 70px; right: 20px; }
        #rectangle { position: absolute; top: 80px; left: 50px; width: 60px; height: 10px; }
        #color-picker {margin-top: 5px; margin-left: 0px; margin-bottom: 10px; }
        #color1 {width: 40px; height: 30px; background-color: white; border: 1px solid black;}
        #color2 {width: 40px; height: 30px; background-color: white; border: 1px solid black;}
        #color3 {width: 40px; height: 30px; background-color: white; border: 1px solid black;}
      </style>
    </head>
    <body>
      <form id="settingsForm">
        マシン名：<input type="text" id="machine_name" name="machine_name" minlength="4"></select><br>
        <div>
          <label for="volumeSlider">音量：</label>
          <input type="range" id="volumeSlider" name="volume" min="0" max="255" step="5">
          <output for="volumeSlider" id="selectedVolume"></output>
        </div>
        <div>
          <label for="brightnessSlider">明るさ：</label>
          <input type="range" id="brightnessSlider" name="brightness" min="10" max="255" step="5">
          <output for="brightnessSlider" id="selectedBrightness"></output>
        </div>
        <div>
          <label for="wordCountSlider">応答文字数：</label>
          <input type="range" id="wordCountSlider" name="word_count" min="10" max="100" step="10">
          <output for="wordCountSlider" id="selectedWordCount"></output>
        </div>
        声(VOICEVOX)：<select id="speaker" name="speaker"></select><br>
        口調：<select id="tone" name="tone"></select><br>
        年代：<select id="age" name="age"></select><br>
        一人称：<select id="first_person" name="first_person"></select><br>
        二人称：<select id="second_person" name="second_person"></select><br>
        色：<br>
        <div id="container">
          <div class="circle" id="circle1"></div>
          <div class="circle" id="circle2"></div>
          <div id="rectangle"></div>
          <div class="circle" id="circle3"></div>
          <div class="circle" id="circle4"></div>
        </div>
        <div id="color-picker">
          <input type="color" id="color1" name="color1">
          <input type="color" id="color2" name="color2">
          <input type="color" id="color3" name="color3">
        </div>
        天気：<select id="weather" name="weather"></select><br>
        <button type="submit">登録</button>
      </form>
    </body>
    <script>
      const machine_name = document.getElementById("machine_name");
      const volumeSlider = document.getElementById("volumeSlider");
      const selectedVolume = document.getElementById("selectedVolume");
      const brightnessSlider = document.getElementById("brightnessSlider");
      const selectedBrightness = document.getElementById("selectedBrightness");
      const wordCountSlider = document.getElementById("wordCountSlider");
      const selectedWordCount = document.getElementById("selectedWordCount");
      const color1 = document.getElementById("color1");
      const color2 = document.getElementById("color2");
      const color3 = document.getElementById("color3");
      const container = document.getElementById("container");
      const circle1 = document.getElementById("circle1");
      const circle2 = document.getElementById("circle2");
      const circle3 = document.getElementById("circle3");
      const circle4 = document.getElementById("circle4");
      const rectangle = document.getElementById("rectangle");
      const optionsObject  = [{"code":"2", "name":"四国めたん（ノーマル）"}, {"code":"0", "name":"四国めたん（あまあま）"}, {"code":"6", "name":"四国めたん（ツンツン）"}, {"code":"4", "name":"四国めたん（セクシー）"}, {"code":"36", "name":"四国めたん（ささやき）"}, {"code":"37", "name":"四国めたん（ヒソヒソ）"}, {"code":"3", "name":"ずんだもん（ノーマル）"}, {"code":"1", "name":"ずんだもん（あまあま）"}, {"code":"7", "name":"ずんだもん（ツンツン）"}, {"code":"5", "name":"ずんだもん（セクシー）"}, {"code":"22", "name":"ずんだもん（ささやき）"}, {"code":"38", "name":"ずんだもん（ヒソヒソ）"}, {"code":"8", "name":"春日部つむぎ（ノーマル）"}, {"code":"10", "name":"雨晴はう（ノーマル）"}, {"code":"9", "name":"波音リツ（ノーマル）"}, {"code":"65", "name":"波音リツ（クイーン）"}, {"code":"11", "name":"玄野武宏（ノーマル）"}, {"code":"39", "name":"玄野武宏（喜び）"}, {"code":"40", "name":"玄野武宏（ツンギレ）"}, {"code":"41", "name":"玄野武宏（悲しみ）"}, {"code":"12", "name":"白上虎太郎（ふつう）"}, {"code":"32", "name":"白上虎太郎（わーい）"}, {"code":"33", "name":"白上虎太郎（びくびく）"}, {"code":"34", "name":"白上虎太郎（おこ）"}, {"code":"35", "name":"白上虎太郎（びえーん）"}, {"code":"13", "name":"青山龍星（ノーマル）"}, {"code":"14", "name":"冥鳴ひまり（ノーマル）"}, {"code":"16", "name":"九州そら（ノーマル）"}, {"code":"15", "name":"九州そら（あまあま）"}, {"code":"18", "name":"九州そら（ツンツン）"}, {"code":"17", "name":"九州そら（セクシー）"}, {"code":"19", "name":"九州そら（ささやき）"}, {"code":"20", "name":"もち子（ノーマル）"}, {"code":"66", "name":"もち子（セクシー／あん子）"}, {"code":"21", "name":"剣崎雌雄（ノーマル）"}, {"code":"23", "name":"WhiteCUL（ノーマル）"}, {"code":"24", "name":"WhiteCUL（たのしい）"}, {"code":"25", "name":"WhiteCUL（かなしい）"}, {"code":"26", "name":"WhiteCUL（びえーん）"}, {"code":"27", "name":"後鬼（人間ver.）"}, {"code":"28", "name":"後鬼（ぬいぐるみver.）"}, {"code":"29", "name":"No.7（ノーマル）"}, {"code":"30", "name":"No.7（アナウンス）"}, {"code":"31", "name":"No.7（読み聞かせ）"}, {"code":"42", "name":"ちび式じい（ノーマル）"}, {"code":"43", "name":"櫻歌ミコ（ノーマル）"}, {"code":"44", "name":"櫻歌ミコ（第二形態）"}, {"code":"45", "name":"櫻歌ミコ（ロリ）"}, {"code":"46", "name":"小夜/SAYO（ノーマル）"}, {"code":"47", "name":"ナースロボ＿タイプＴ（ノーマル）"}, {"code":"48", "name":"ナースロボ＿タイプＴ（楽々）"}, {"code":"49", "name":"ナースロボ＿タイプＴ（恐怖）"}, {"code":"50", "name":"ナースロボ＿タイプＴ（内緒話）"}, {"code":"51", "name":"†聖騎士紅桜†（ノーマル）"}, {"code":"52", "name":"雀松朱司（ノーマル）"}, {"code":"53", "name":"麒ヶ島宗麟（ノーマル）"}, {"code":"54", "name":"春歌ナナ（ノーマル）"}, {"code":"55", "name":"猫使アル（ノーマル）"}, {"code":"56", "name":"猫使アル（おちつき）"}, {"code":"57", "name":"猫使アル（うきうき）"}, {"code":"58", "name":"猫使ビィ（ノーマル）"}, {"code":"59", "name":"猫使ビィ（おちつき）"}, {"code":"60", "name":"猫使ビィ（人見知り）"}, {"code":"61", "name":"中国ウサギ（ノーマル）"}, {"code":"62", "name":"中国ウサギ（おどろき）"}, {"code":"63", "name":"中国ウサギ（こわがり）"}, {"code":"64", "name":"中国ウサギ（へろへろ）"}, {"code":"67", "name":"栗田まろん（ノーマル）"}, {"code":"68", "name":"あいえるたん（ノーマル）"}, {"code":"69", "name":"満別花丸（ノーマル）"}, {"code":"70", "name":"満別花丸（元気）"}, {"code":"71", "name":"満別花丸（ささやき）"}, {"code":"72", "name":"満別花丸（ぶりっ子）"}, {"code":"73", "name":"満別花丸（ボーイ）"}, {"code":"73", "name":"琴詠ニア（ノーマル）"}];
      const settingsForm = document.getElementById("settingsForm");
      const optionsObject_tone = ["やさしい", "フレンドリー", "好奇心旺盛", "引っ込み思案", "ツンデレ", "お調子者", "ナルシスト", "乱暴", "高貴"];
      const optionsObject_age = ["子供", "若者", "中年", "老人"];
      const optionsObject_first_person = ["あたい", "あたし", "あっし", "うち", "おいどん", "おいら", "おら", "おれ", "おれさま", "おれっち", "じぶん", "せっしゃ", "それがし", "ちん", "ぼく", "ぼくちん", "まろ", "みー", "よ", "わい", "わがはい", "わし", "わたくし", "わたし", "わて", "わらわ", "われ"];
      const optionsObject_second_person = ["あなた", "あなたさま", "あんさん", "あんた", "あーた", "うぬ", "おぬし", "おまえ", "おまえさん", "きこう", "きさま", "きでん", "きみ", "ごしゅじんさま", "そち", "そなた", "そのほう", "なんじ", "ますたー", "ゆー"];
      const optionsObject_weather  = [{"code":"011000", "name":"宗谷地方"}, {"code":"012000", "name":"上川・留萌地方"}, {"code":"013000", "name":"網走・北見・紋別地方"}, {"code":"014100", "name":"釧路・根室地方"}, {"code":"015000", "name":"胆振・日高地方"}, {"code":"016000", "name":"石狩・空知・後志地方"}, {"code":"017000", "name":"渡島・檜山地方"}, {"code":"020000", "name":"青森県"}, {"code":"030000", "name":"岩手県"}, {"code":"040000", "name":"宮城県"}, {"code":"050000", "name":"秋田県"}, {"code":"060000", "name":"山形県"}, {"code":"070000", "name":"福島県"}, {"code":"080000", "name":"茨城県"}, {"code":"090000", "name":"栃木県"}, {"code":"100000", "name":"群馬県"}, {"code":"110000", "name":"埼玉県"}, {"code":"120000", "name":"千葉県"}, {"code":"130000", "name":"東京都"}, {"code":"140000", "name":"神奈川県"}, {"code":"150000", "name":"新潟県"}, {"code":"160000", "name":"富山県"}, {"code":"170000", "name":"石川県"}, {"code":"180000", "name":"福井県"}, {"code":"190000", "name":"山梨県"}, {"code":"200000", "name":"長野県"}, {"code":"210000", "name":"岐阜県"}, {"code":"220000", "name":"静岡県"}, {"code":"230000", "name":"愛知県"}, {"code":"240000", "name":"三重県"}, {"code":"250000", "name":"滋賀県"}, {"code":"260000", "name":"京都府"}, {"code":"270000", "name":"大阪府"}, {"code":"280000", "name":"兵庫県"}, {"code":"290000", "name":"奈良県"}, {"code":"300000", "name":"和歌山県"}, {"code":"310000", "name":"鳥取県"}, {"code":"320000", "name":"島根県"}, {"code":"330000", "name":"岡山県"}, {"code":"340000", "name":"広島県"}, {"code":"350000", "name":"山口県"}, {"code":"360000", "name":"徳島県"}, {"code":"370000", "name":"香川県"}, {"code":"380000", "name":"愛媛県"}, {"code":"390000", "name":"高知県"}, {"code":"400000", "name":"福岡県"}, {"code":"410000", "name":"佐賀県"}, {"code":"420000", "name":"長崎県"}, {"code":"430000", "name":"熊本県"}, {"code":"440000", "name":"大分県"}, {"code":"450000", "name":"宮崎県"}, {"code":"460100", "name":"鹿児島県"}, {"code":"471000", "name":"沖縄本島地方"}, {"code":"472000", "name":"大東島地方"}, {"code":"473000", "name":"宮古島地方"}, {"code":"474000", "name":"八重山地方"}];

      volumeSlider.addEventListener("input", function() {
        selectedVolume.textContent = volumeSlider.value;
      });

      brightnessSlider.addEventListener("input", function() {
        selectedBrightness.textContent = brightnessSlider.value;
      });

      wordCountSlider.addEventListener("input", function() {
        selectedWordCount.textContent = wordCountSlider.value;
      });

      color1.addEventListener("input", () => {
        container.style.backgroundColor = color1.value;
      });

      color2.addEventListener("input", () => {
        circle1.style.backgroundColor = color2.value;
        circle2.style.backgroundColor = color2.value;
        rectangle.style.backgroundColor = color2.value;
      });

      color3.addEventListener("input", () => {
        circle3.style.backgroundColor = color3.value;
        circle4.style.backgroundColor = color3.value;
      });

      settingsForm.addEventListener("submit", function (e) {
        e.preventDefault();
      
        const formData = new FormData(this);

        fetch('/update_config', {
          method: 'POST',
          body: formData
        })
        .then(response => {
          if (response.ok) {
          console.log('POST成功');
          } else {
          console.error('POSTエラー');
          }
        })
        .catch(error => {
          console.error('POSTエラー:', error);
        });
      });)";

String html_config() {
  String color1 = rgb_to_string(config_color1_red, config_color1_green, config_color1_blue);
  String color2 = rgb_to_string(config_color2_red, config_color2_green, config_color2_blue);
  String color3 = rgb_to_string(config_color3_red, config_color3_green, config_color3_blue);
  String html_config_string_2 = R"(
        window.onload = function() {
          machine_name.value = ")" + String(config_machine_name) + R"(";
          volumeSlider.value = )" + String(config_volume) + R"(;
          selectedVolume.textContent = )" + String(config_volume) + R"(;
          brightnessSlider.value = )" + String(config_brightness) + R"(;
          selectedBrightness.textContent = )" + String(config_brightness) + R"(;
          wordCountSlider.value = )" + String(config_word_count) + R"(;
          selectedWordCount.textContent = )" + String(config_word_count) + R"(;
          container.style.backgroundColor = ")" + color1 + R"(";
          color1.value = ")" + color1 + R"(";
          circle1.style.backgroundColor = ")" + color2 + R"(";
          circle2.style.backgroundColor = ")" + color2 + R"(";
          rectangle.style.backgroundColor = ")" + color2 + R"(";
          color2.value = ")" + color2 + R"(";
          circle3.style.backgroundColor = ")" + color3 + R"(";
          circle4.style.backgroundColor = ")" + color3 + R"(";
          color3.value = ")" + color3 + R"(";
          var speaker = document.getElementById("speaker");
          for (var key in optionsObject) {
            var option = document.createElement("option");
            option.value =  optionsObject[key]["code"];
            option.text =  optionsObject[key]["name"];
            speaker.add(option);
            if (optionsObject[key]["code"] === ")" + config_speaker + R"(") {
              option.selected = true;
            }
          }
          var tone = document.getElementById("tone");
          for (var key in optionsObject_tone) {
            var option = document.createElement("option");
            option.value =  optionsObject_tone[key];
            option.text =  optionsObject_tone[key];
            tone.add(option);
            if (optionsObject_tone[key] === ")" + config_tone + R"(") {
              option.selected = true;
            }
          }
          var age = document.getElementById("age");
          for (var key in optionsObject_age) {
            var option = document.createElement("option");
            option.value =  optionsObject_age[key];
            option.text =  optionsObject_age[key];
            age.add(option);
            if (optionsObject_age[key] === ")" + config_age + R"(") {
              option.selected = true;
            }
          }
          var first_person = document.getElementById("first_person");
          for (var key in optionsObject_first_person) {
            var option = document.createElement("option");
            option.value =  optionsObject_first_person[key];
            option.text =  optionsObject_first_person[key];
            first_person.add(option);
            if (optionsObject_first_person[key] === ")" + config_first_person + R"(") {
              option.selected = true;
            }
          }
          var second_person = document.getElementById("second_person");
          for (var key in optionsObject_second_person) {
            var option = document.createElement("option");
            option.value = optionsObject_second_person[key];
            option.text = optionsObject_second_person[key];
            second_person.add(option);
            if (optionsObject_second_person[key] === ")" + config_second_person + R"(") {
              option.selected = true;
            }
          }
          var weather = document.getElementById("weather");
          for (var key in optionsObject_weather) {
            var option = document.createElement("option");
            option.value = optionsObject_weather[key]["code"];
            option.text = optionsObject_weather[key]["name"];
            weather.add(option);
            if (optionsObject_weather[key]["code"] === ")" + config_weather + R"(") {
              option.selected = true;
            }
          }
        }
      </script>
    </html>
  )";
  return html_config_string_1 + html_config_string_2;
}

String html_update_config() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>スタックチャンの設定をおこないました</title>
      </head>
      <body>スタックチャンの設定をおこないました</body>
    </html>)";
}

String html_apikey() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>API KEYの登録</title>
      </head>
      <body>
        <form id="settingsForm">
          <label for="openai_apikey">OpenAI API Key：</label>
          <input type="text" id="openai_apikey" name="openai_apikey">
          <br>
          <label for="voicevox_apikey">VOICEVOX API Key：</label>
          <input type="text" id="voicevox_apikey" name="voicevox_apikey">
          <br>
          <button type="submit">登録</button>
        </form>
      </body>
      <script>
        const openai_apikey = document.getElementById("openai_apikey");
        const voicevox_apikey = document.getElementById("voicevox_apikey");
        const settingsForm = document.getElementById("settingsForm");

        settingsForm.addEventListener("submit", function (e) {
          e.preventDefault();
      
          const formData = new FormData(this);

          fetch('/update_apikey', {
            method: 'POST',
            body: formData
          })
          .then(response => {
            if (response.ok) {
            console.log('POST成功');
            } else {
            console.error('POSTエラー');
            }
          })
          .catch(error => {
            console.error('POSTエラー:', error);
          });
        })
      </script>
    </html>)";
}

String html_update_apikey() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>スタックチャンの設定をおこないました</title>
      </head>
      <body>スタックチャンの設定をおこないました</body>
    </html>)";
}

String html_chatgpt() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ChatGPTにテキストを送りました</title>
      </head>
      <body>ChatGPTにテキストを送りました</body>
    </html>)";
}

String html_voicevox() {
  return R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>VOICEVOXにテキストを送りました</title>
      </head>
      <body>VOICEVOXにテキストを送りました</body>
    </html>)";
}

#endif

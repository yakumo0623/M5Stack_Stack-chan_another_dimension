#ifndef _HTML_H
#define _HTML_H

#include<M5Unified.h>

extern uint8_t config_volume;
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
        <div>
          <label for="volumeSlider">音量：</label>
          <input type="range" id="volumeSlider" name="volume" min="0" max="255">
          <output for="volumeSlider" id="selectedVolume"></output>
        </div>
        声：<select id="speaker" name="speaker"></select><br>
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
        <button type="submit">登録</button>
      </form>
    </body>
    <script>
      const volumeSlider = document.getElementById("volumeSlider");
      const selectedVolume = document.getElementById("selectedVolume");
      const color1 = document.getElementById("color1");
      const color2 = document.getElementById("color2");
      const color3 = document.getElementById("color3");
      const container = document.getElementById("container");
      const circle1 = document.getElementById("circle1");
      const circle2 = document.getElementById("circle2");
      const circle3 = document.getElementById("circle3");
      const circle4 = document.getElementById("circle4");
      const rectangle = document.getElementById("rectangle");
      const optionsObject  = {2:"四国めたん（ノーマル）",0:"四国めたん（あまあま）",6:"四国めたん（ツンツン）",4:"四国めたん（セクシー）",36:"四国めたん（ささやき）",37:"四国めたん（ヒソヒソ）",3:"ずんだもん（ノーマル）",1:"ずんだもん（あまあま）",7:"ずんだもん（ツンツン）",5:"ずんだもん（セクシー）",22:"ずんだもん（ささやき）",38:"ずんだもん（ヒソヒソ）",8:"春日部つむぎ（ノーマル）",10:"雨晴はう（ノーマル）",9:"波音リツ（ノーマル）",65:"波音リツ（クイーン）",11:"玄野武宏（ノーマル）",39:"玄野武宏（喜び）",40:"玄野武宏（ツンギレ）",41:"玄野武宏（悲しみ）",12:"白上虎太郎（ふつう）",32:"白上虎太郎（わーい）",33:"白上虎太郎（びくびく）",34:"白上虎太郎（おこ）",35:"白上虎太郎（びえーん）",13:"青山龍星（ノーマル）",14:"冥鳴ひまり（ノーマル）",16:"九州そら（ノーマル）",15:"九州そら（あまあま）",18:"九州そら（ツンツン）",17:"九州そら（セクシー）",19:"九州そら（ささやき）",20:"もち子（ノーマル）",66:"もち子（セクシー／あん子）",21:"剣崎雌雄（ノーマル）",23:"WhiteCUL（ノーマル）",24:"WhiteCUL（たのしい）",25:"WhiteCUL（かなしい）",26:"WhiteCUL（びえーん）",27:"後鬼（人間ver.）",28:"後鬼（ぬいぐるみver.）",29:"No.7（ノーマル）",30:"No.7（アナウンス）",31:"No.7（読み聞かせ）",42:"ちび式じい（ノーマル）",43:"櫻歌ミコ（ノーマル）",44:"櫻歌ミコ（第二形態）",45:"櫻歌ミコ（ロリ）",46:"小夜/SAYO（ノーマル）",47:"ナースロボ＿タイプＴ（ノーマル）",48:"ナースロボ＿タイプＴ（楽々）",49:"ナースロボ＿タイプＴ（恐怖）",50:"ナースロボ＿タイプＴ（内緒話）",51:"†聖騎士 紅桜†（ノーマル）",52:"雀松朱司（ノーマル）",53:"麒ヶ島宗麟（ノーマル）",54:"春歌ナナ（ノーマル）",55:"猫使アル（ノーマル）",56:"猫使アル（おちつき）",57:"猫使アル（うきうき）",58:"猫使ビィ（ノーマル）",59:"猫使ビィ（おちつき）",60:"猫使ビィ（人見知り）",61:"中国ウサギ（ノーマル）",62:"中国ウサギ（おどろき）",63:"中国ウサギ（こわがり）",64:"中国ウサギ（へろへろ）",67:"栗田まろん（ノーマル）",68:"あいえるたん（ノーマル）",69:"満別花丸（ノーマル）",70:"満別花丸（元気）",71:"満別花丸（ささやき）",72:"満別花丸（ぶりっ子）",73:"満別花丸（ボーイ）",73:"琴詠ニア（ノーマル）"};
      const settingsForm = document.getElementById("settingsForm");
      const optionsObject_tone = ["やさしい", "フレンドリー", "ツンデレ", "上から目線", "命令口調", "神様仏様口調"];
      const optionsObject_age = ["幼児", "若者", "中年", "老人"];
      const optionsObject_first_person = ["あたい", "あたし", "あっし", "うち", "おいどん", "おいら", "おら", "おれ", "おれさま", "おれっち", "じぶん", "せっしゃ", "それがし", "ちん", "ぼく", "ぼくちん", "まろ", "みー", "よ", "わい", "わがはい", "わし", "わたくし", "わたし", "わて", "わらわ", "われ"];
      const optionsObject_second_person = ["あなた", "あなたさま", "あんさん", "あんた", "あーた", "うぬ", "おぬし", "おまえ", "おまえさん", "きこう", "きさま", "きでん", "きみ", "ごしゅじんさま", "そち", "そなた", "そのほう", "なんじ", "ますたー", "ゆー"];

      volumeSlider.addEventListener("input", function() {
        selectedVolume.textContent = volumeSlider.value;
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
          volumeSlider.value = )" + String(config_volume) + R"(;
          selectedVolume.textContent = )" + String(config_volume) + R"(;
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
            option.value = key;
            option.text = optionsObject[key];
            speaker.add(option);
            if (key === ")" + String(config_speaker) + R"(") {
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

#endif

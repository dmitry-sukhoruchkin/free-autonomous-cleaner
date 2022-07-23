 /*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "********";
const char* password = "********";
const char* ssid2 = "********";
const char* password2 = "********";

const int output = 2;

String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

  char temp[600];

const int led = 13;
const int lp = 16;
const int lm = 17;
const int rp = 18;
const int rm = 19;

const int output1 = lp;
const int output2 = rp;

String sliderValue1 = "0";
String sliderValue2 = "0";

// setting PWM properties
//const int freq = 5000;
const int ledChannel1 = 1;
const int ledChannel2 = 2;
//const int resolution = 8;

void setup(void) {
  snprintf(temp, 600,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Cleaner</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <a href=\"/stepleft\"><h1>Step Left</h1></a>\
    <a href=\"/stepforward\"><h1>Step Froward</h1></a>\
    <a href=\"/stepright\"><h1>Step Right</h1></a>\
    <a href=\"/moveleft\"><h1>Move Left</h1></a>\
    <a href=\"/moveforward/\"><h1>Move Froward</h1></a>\
    <a href=\"/moveright\><h1>Move Right</h1></a>\
    <a href=\"/stop\"><h1>Stop</h1></a>\
  </body>\
</html>");
  
  pinMode(led, OUTPUT);
  pinMode(lp, OUTPUT);
  pinMode(lm, OUTPUT);
  pinMode(rp, OUTPUT);
  pinMode(rm, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(lp, 0);
  digitalWrite(lm, 0);
  digitalWrite(rp, 0);
  digitalWrite(rm, 0);

  Serial.begin(115200);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output, ledChannel);
  
  ledcWrite(ledChannel, sliderValue.toInt());

  // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output1, ledChannel1);
  
  ledcWrite(ledChannel1, sliderValue1.toInt());

  // configure LED PWM functionalitites
  ledcSetup(ledChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output2, ledChannel2);
  
  ledcWrite(ledChannel2, sliderValue2.toInt());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  int reconnect=0;
//  Serial.println("");
  int connection_fail=0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    connection_fail=connection_fail+1;
    if(connection_fail%10==0)
    {
      if(connection_fail/10%2==0)
      {
        WiFi.begin(ssid, password);
      }
      else
      {
        WiFi.begin(ssid2, password2);
      }
    }
    reconnect=1;
  }
  if(reconnect)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

//  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Hi! This is a sample response.");
//  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA

  
  server.on("/stepright", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rp, 0);
    digitalWrite(rm, 0);
    digitalWrite(lp, 1);
    digitalWrite(lm, 1);
    request->send(200, "text/html", temp);
    delay(1000);
    digitalWrite(rp, 1);
    digitalWrite(rm, 1);
  });
  server.on("/stepleft", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(lp, 0);
    digitalWrite(lm, 0);
    digitalWrite(rp, 1);
    digitalWrite(rm, 1);
    request->send(200, "text/html", temp);
    delay(1000);
    digitalWrite(lp, 1);
    digitalWrite(lm, 1);
  });
  server.on("/stepforward", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rp, 0);
    digitalWrite(rm, 0);
    digitalWrite(lp, 0);
    digitalWrite(lm, 0);
    request->send(200, "text/html", temp);
    delay(1000);
    digitalWrite(rp, 1);
    digitalWrite(rm, 1);
    digitalWrite(lp, 1);
    digitalWrite(lm, 1);
  });
  server.on("/moveright", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rp, 0);
    digitalWrite(rm, 0);
    digitalWrite(lp, 1);
    digitalWrite(lm, 1);
    request->send(200, "text/html", temp);
  });
  server.on("/moveleft", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(lp, 0);
    digitalWrite(lm, 0);
    digitalWrite(rp, 1);
    digitalWrite(rm, 1);
    request->send(200, "text/html", temp);
  });
  server.on("/moveforward", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rp, 0);
    digitalWrite(rm, 0);
    digitalWrite(lp, 0);
    digitalWrite(lm, 0);
    request->send(200, "text/html", temp);
  });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rp, 1);
    digitalWrite(rm, 1);
    digitalWrite(lp, 1);
    digitalWrite(lm, 1);
    request->send(200, "text/html", temp);
  });


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel, sliderValue.toInt());

      //255*0/4 lc1=0 lc2=255
      //255*1/4 lc1=0 lc2=127
      //255*2/4 lc1=0 lc2=0
      //255*3/4 lc1=127 lc2=0
      //255*4/4 lc1=255 lc2=0
      if(sliderValue.toInt()<255/2)
      {
        ledcWrite(ledChannel1, 0);
        ledcWrite(ledChannel2, 255-sliderValue.toInt()*2);
      }
      if(sliderValue.toInt()==255/2)
      {
        ledcWrite(ledChannel1, 0);
        ledcWrite(ledChannel2, 0);
      }
      if(sliderValue.toInt()>255/2)
      {
        ledcWrite(ledChannel1, sliderValue.toInt()*2-255);
        ledcWrite(ledChannel2, 0);
      }
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  int reconnect=0;
//  Serial.println("");
  int connection_fail=0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    connection_fail=connection_fail+1;
    if(connection_fail%10==0)
    {
      if(connection_fail/10%2==0)
      {
        WiFi.begin(ssid, password);
      }
      else
      {
        WiFi.begin(ssid2, password2);
      }
    }
    reconnect=1;
  }
  if(reconnect)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

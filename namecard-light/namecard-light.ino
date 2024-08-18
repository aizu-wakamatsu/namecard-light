#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

const int PIN = 27;
const int NUMPIXELS = 5;
const char* SSID_WL = "meishi";
const char* PASS_WL = "namecard";

WebServer server(80);
const char* serverName = "meishi";
HTTPClient client;

void get();
void post();
void beginServ();
void light();

String r0 = "0";
String g0 = "0";
String b0 = "0";
String r1 = "0";
String g1 = "0";
String b1 = "0";

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  /*
  while (1) {
    r = "255";
    light();
    delay(500);
    r = "0";
    light();
    delay(500);
  }
  */
  beginServ();
  server.on("/", HTTP_GET, get);
  server.on("/", HTTP_POST, post);
  server.begin();
  pixels.begin();
}

void loop() {
  server.handleClient();
}

void beginServ() {
  Serial.println("[BOOT] BEGIN SERVER");
  WiFi.begin(SSID_WL, PASS_WL);
  Serial.println("[BOOT] BEGIN WLAN");
  delay(10);  //内部レジスタ待ち
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("[FAIL] FAILED -- BEGIN WLAN -> RETRY");
    delay(1000);
  }

  if (!MDNS.begin(serverName)) {
    //Serial.println("mDNS Failed");
    while (1)
      ;
  }
  MDNS.addService("http", "tcp", 80);  //Webサーバーを開始
  Serial.print("[SERV] MY NAME IS ");
  Serial.println(serverName);
  Serial.print("[SERV] IP ADDR: ");
  Serial.println(WiFi.localIP());
}

void get() {
  String HTML = "<!DOCTYPE html> \n<html lang=\"ja\">";
  HTML += "<HTML><HEAD><meta charset ='UTF-8'>";
  // HTML += "<meta http-equiv=\"refresh\" content=\"2\">";
  HTML += "<TITLE>名刺</TITLE><style>*{margin:0px;text-align:center;font-size:30pt;background-color:rgb(";
  HTML += r0 + "," + g0 + "," + b0;
  HTML += ")}input{width:80%;}input[type=\"range\"] {-webkit-appearance:none;appearance:none;cursor:pointer;outline:none;border-radius:20px;border:solid 10px #DDDDDD;}";
  HTML += "input[type=\"range\"]::-webkit-slider-thumb {-webkit-appearance: none;background: #111111;width: 20px;height: 50px;border-radius: 10%;border: solid 5px #FFFFFF;}";
  HTML += "input[type=\"range\"]::-moz-range-thumb  {background:#111111;width: 20px;height: 50px;border-radius: 10%;border: none;border: solid 5px #FFFFFF }";
  HTML += "input[type=\"button\"]{background:#FFFFFF;border-radius:20px;}";
  HTML += "input[type=\"submit\"]{background:#FFFFFF;border-radius:20px;}";
  HTML += ".red{background: #AA0000}";
  HTML += ".green{background: #00AA00}";
  HTML += ".blue{background: #0000AA}";
  HTML += "h1{background: #FFFFFF; width: 30%; text-align: center; margin-left:auto;margin-right:auto;border-radius:20px;}";
  HTML += "p{background: #FFFFFF; width: 30%; text-align: center;margin-left:auto;margin-right:auto;border-radius:20px;}";
  HTML += "</style>";
  HTML += "</HEAD>";
  HTML += "<BODY><br><br><h1>いろ</h1><br>";
  HTML += "<input type=\"button\" onclick=\"window.location.reload(true);\" value=\"[こうしん]\"><br><br>";
  HTML += "<div><form method=\"POST\" target=_self>";
  HTML += "<p>あか</p><br><input class=\"red\" type=\"range\" name=\"r0\" value=\"";
  HTML += r0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "<p>みどり</p><br><input class=\"green\"type=\"range\" name=\"g0\" value=\"";
  HTML += g0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "<p>あお</p><br><input class=\"blue\" type=\"range\" name=\"b0\" value=\"";
  HTML += b0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  /*
  HTML += "R2<br><input type=\"range\" name=\"r1\" value=\"";
  HTML += r1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "G2<br><input type=\"range\" name=\"g1\" value=\"";
  HTML += g1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "B2<br><input type=\"range\" name=\"b1\" value=\"";
  HTML += b1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  */
  HTML += "<input type=\"submit\" value=\"[せってい]\"></div></form>";
  HTML += "</BODY></HTML>";
  server.send(200, "text/html", HTML);
}

void post() {
  r0 = server.arg("r0");
  g0 = server.arg("g0");
  b0 = server.arg("b0");
  //r1 = server.arg("r1");
  //g1 = server.arg("g1");
  //b1 = server.arg("b1");
  light();
  //Serial.println("[DATA] R" + r0 + " G" + g0 + " B" + b0);
  get();
}

void light() {
  int i;
  pixels.clear();
  for (i = 0; i < 4; i++) {
    pixels.setPixelColor(i, pixels.Color(r0.toInt(), g0.toInt(), b0.toInt()));
    pixels.show();
  } /*
  for (i = 2; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r1.toInt(), g1.toInt(), b1.toInt()));
    pixels.show();
  }*/
}

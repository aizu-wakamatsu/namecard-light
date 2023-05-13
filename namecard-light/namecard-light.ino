#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

const int PIN = 27;
const int NUMPIXELS = 5;
const char* SSID_WL = "meishi";
const char* PASS_WL = "s1250039";

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
}

void get() {
  String HTML = "<!DOCTYPE html> \n<html lang=\"ja\">";
  HTML += "<HTML><HEAD><meta charset ='UTF-8'><TITLE>名刺</TITLE><style>*{text-align:center;font-size:30pt;}</style></HEAD>";
  HTML += "<BODY><p><B>colour</B></p>";
  HTML += "<p><form method=\"POST\" target=_self>";
  HTML += "R1<br><input type=\"range\" name=\"r0\" value=\"";
  HTML += r0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "G1<br><input type=\"range\" name=\"g0\" value=\"";
  HTML += g0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "B1<br><input type=\"range\" name=\"b0\" value=\"";
  HTML += b0;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "R2<br><input type=\"range\" name=\"r1\" value=\"";
  HTML += r1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "G2<br><input type=\"range\" name=\"g1\" value=\"";
  HTML += g1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "B2<br><input type=\"range\" name=\"b1\" value=\"";
  HTML += b1;
  HTML += "\" min=\"0\" max=\"255\"/><br><br>";
  HTML += "<input type=\"submit\" value=\"set\"></p></form>";
  HTML += "</BODY></HTML>";
  server.send(200, "text/html", HTML);
}

void post() {
  r0 = server.arg("r0");
  g0 = server.arg("g0");
  b0 = server.arg("b0");
  r1 = server.arg("r1");
  g1 = server.arg("g1");
  b1 = server.arg("b1");
  light();
  //Serial.println("[DATA] R" + r0 + " G" + g0 + " B" + b0);
  get();
}

void light() {
  int i;
  pixels.clear();
  for (i = 0; i < 2; i++) {
    pixels.setPixelColor(i, pixels.Color(r0.toInt(), g0.toInt(), b0.toInt()));
    pixels.show();
  }
  for (i = 2; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r1.toInt(), g1.toInt(), b1.toInt()));
    pixels.show();
  }
}

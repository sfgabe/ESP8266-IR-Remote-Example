// Tested to work with Arduino IDE 1.9.5 and IRremoteESP8266 2.5.4-2.5.5 (25aa772)
// Working hardware is Infrared Remote Decoder and reciever at https://www.ebay.com/itm/5V-IR-Infrared-Remote-Decoder-Encoding-Transmitter-Receiver-Wireless-Module/401092840926?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649
// and 50W+50W Portable Bluetooth Audio Receiver Amplifier Board TF Card Decoder with Remote Control DC 8-26V at https://amzn.to/2ZbyvtU
// Adapted from https://github.com/ultrakiller2/ESP8266-IR-Remote-Example/blob/master/BasicWebRemote/BasicWebRemote.ino
// Works with YS-IRTM module and WEMOS D1 mini with tx and rx crossed
//
//
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Add this library: https://github.com/markszabo/IRremoteESP8266
#include <IRremoteESP8266.h>
#include <IRsend.h> // small change to compile on IRremoteESP8266 2.0+

#define IR_SEND_PIN TX

#define DELAY_BETWEEN_COMMANDS 1000

IRsend irsend(IR_SEND_PIN);

const char* ssid = "RollinsRogue";
const char* password = "SonnyNights";

ESP8266WebServer server(80);

const int led = BUILTIN_LED;

String rowDiv = "    <div class=\"row\" style=\"padding-bottom:1em\">\n";
String endDiv = "    </div>\n";

// Buttons are using the bootstrap grid for sizing - http://getbootstrap.com/css/#grid
String generateButton(String colSize, String id, String text, String url) {

  return  "<div class=\"" + colSize + "\" style=\"text-align: center\">\n" +
          "    <button id=\"" + id + "\" type=\"button\" class=\"btn btn-default\" style=\"width: 100%\" onclick='makeAjaxCall(\"" + url + "\")'>" + text + "</button>\n" +
          "</div>\n";
}

void handleRoot() {
  digitalWrite(led, 0);
  String website = "<!DOCTYPE html>\n";
  website = website + "<html>\n";
  website = website + "  <head>\n";
  website = website + "    <meta charset=\"utf-8\">\n";
  website = website + "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  website = website + "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  website = website + "    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\n";
  website = website + "  </head>\n";
  website = website + "  <body>\n";
  website = website + "    <div class=\"container-fluid\">\n";
  // ------------------------- Power Controls --------------------------
  website = website + rowDiv;
  website = website + generateButton("col-xs-3", "power","Power", "power");
  website = website + generateButton("col-xs-3", "play","Play", "play");
  website = website + generateButton("col-xs-3", "stop","Stop", "stop");
  website = website + generateButton("col-xs-3", "mode","Mode", "mode");
  website = website + endDiv;
  // ------------------------- Channel Controls --------------------------
  website = website + rowDiv;
  website = website + generateButton("col-xs-4", "mute","Mute", "mute");
  website = website + generateButton("col-xs-4", "back","Back", "back");
  website = website + generateButton("col-xs-4", "eq","EQ", "eq");
  website = website + endDiv;
  // ------------------------- Direction Controls --------------------------
  website = website + rowDiv;
  website = website + generateButton("col-xs-12", "fwd","Fast Foreward", "fwd");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-12", "rwd","Rewind", "rwd");
  website = website + endDiv;
  // ------------------------- Volume Controls --------------------------
  website = website + rowDiv;
  website = website + generateButton("col-xs-12", "up","Vol Up", "up");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-12", "down","Vol Down", "down");
  website = website + endDiv;
  // ------------------------- Favorites Controls --------------------------
  website = website + rowDiv;
  website = website + generateButton("col-xs-4", "1","1", "1");
  website = website + generateButton("col-xs-4", "2","2", "2");
  website = website + generateButton("col-xs-4", "3","3", "3");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-4", "4","4", "4");
  website = website + generateButton("col-xs-4", "5","5", "5");
  website = website + generateButton("col-xs-4", "6","6", "6");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-4", "7","7", "7");
  website = website + generateButton("col-xs-4", "8","8", "8");
  website = website + generateButton("col-xs-4", "9","9", "9");
  website = website + endDiv;
  // ------------------------- End Bits --------------------------
  
  website = website + endDiv;
  website = website + "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>\n";
  website = website + "    <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>\n";
  website = website + "    <script> function makeAjaxCall(url){$.ajax({\"url\": url})}</script>\n";
  website = website + "  </body>\n";
  website = website + "</html>\n";

  server.send(200, "text/html", website);
  digitalWrite(led, 1);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 1);
}

void setup(void){
  irsend.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("BTRadio")) {
    Serial.println("MDNS Responder Started");
  }

  server.on("/", handleRoot);

  server.on("/down", [](){
    Serial.println("Volume Down");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x16};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Volume Down");
  });

  server.on("/up", [](){
    Serial.println("Volume Up");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x19};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Volume Up");
  });

  server.on("/stop", [](){
    Serial.println("Stop");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x46};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Stop");
  });

  server.on("/mute", [](){
    Serial.println("Mute");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x47};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Mute");
  });

  server.on("/back", [](){
    Serial.println("Back");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x40};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Back");
  });

  server.on("/eq", [](){
    Serial.println("EQ");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x43};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "EQ");
  });

  server.on("/rwd", [](){
    Serial.println("Rewind");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x07};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Rewind");
  });

  server.on("/fwd", [](){
    Serial.println("Fast Foreward");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x15};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Fast Foreward");
  });

  server.on("/power", [](){
    Serial.println("Power");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x45};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Power");
  });

  server.on("/play", [](){
    Serial.println("Play");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x09};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Play");
  });

  server.on("/0", [](){
    Serial.println("0");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x0D};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "0");
  });

  server.on("/mode", [](){
    Serial.println("Mode");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x44};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "Mode");
  });

  server.on("/1", [](){
    Serial.println("1");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x0C};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "1");
  });

  server.on("/2", [](){
    Serial.println("2");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x18};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "2");
  });

  server.on("/3", [](){
    Serial.println("3");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x5E};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "3");
  });

  server.on("/4", [](){
    Serial.println("4");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x08};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "4");
  });

  server.on("/5", [](){
    Serial.println("5");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x1C};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "5");
  });

  server.on("/6", [](){
    Serial.println("6");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x18};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "6");
  });

  server.on("/7", [](){
    Serial.println("7");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x5E};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "7");
  });

  server.on("/8", [](){
    Serial.println("8");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x08};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "8");
  });

  server.on("/9", [](){
    Serial.println("9");
    uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x00, 0xFF, 0x1C};
    Serial.write(my_serial_bytes,sizeof(my_serial_bytes));
    server.send(200, "text/plain", "9");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(void){
  server.handleClient();
}

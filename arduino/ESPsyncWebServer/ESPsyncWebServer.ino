
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

const char* ssid="Manu S21";
const char* password="xapp6304";
AsyncWebServer server(80);


void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);  
  Serial.begin(115200);
  conectarse();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int numParametros = request -> params();
    Serial.print(numParametros);
    if(numParametros == 0){
      request -> send(200,"text/html","<h1>hola mundo</h1>");
    }else{
            AsyncWebParameter *p = request -> getParam(0);
            String html = "<h1>Hola " + p->value() + " desde ESP32</h1>";
            request -> send(200,"text/html", html);
    }
  });
  server.on("/adios", HTTP_GET,[](AsyncWebServerRequest *r){
    r->send(200, "text/html", "<h1>adios</h1>");
  });
  server.on("/on", HTTP_GET,[](AsyncWebServerRequest *r){
    r->send(200, "text/html", "<h1>prendido</h1>");
    digitalWrite(2, HIGH);    
  });  
  server.on("/off", HTTP_GET,[](AsyncWebServerRequest *r){
    r->send(200, "text/html", "<h1>apagado</h1>");
    digitalWrite(2, LOW);
  });
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void conectarse(){
    Serial.print("Conectando");
    Serial.println(ssid);
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");      
    }
    Serial.print(WiFi.localIP());
  }

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

#include <ArduinoJson.h>

#include "DHTesp.h"

#define DHTpin 15

DHTesp dht;

const char* ssid="Megacable-5550";
const char* password="x2tSUSWuTR";
AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");
dht.setup(DHTpin, DHTesp::DHT11);

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

  server.on("/temperatura", HTTP_GET,[](AsyncWebServerRequest *r){
    float centigrados = dht.getTemperature();
    float farenheit = dht.toFahrenheit(centigrados);
    String html = "<h1>Centigrados: " + String(centigrados) + "</h1> <h1>Farenheit: " + String(farenheit) + "</h1>";
    r->send(200, "text/html", html);
  });

  server.on("/humedad", HTTP_GET,[](AsyncWebServerRequest *r){
    float humedad =  dht.getHumidity();
    String html = "<h1>Humedad: " + String(humedad) + "</h1>";
    r->send(200, "text/html", html);
  });

  server.on("/full", HTTP_GET,[](AsyncWebServerRequest *r){
    Serial.print("request recibido");
    AsyncResponseStream *response = r->beginResponseStream("application/json");    
    DynamicJsonDocument json(1024);
    float humedad =  dht.getHumidity();
    float centigrados = dht.getTemperature();
    float farenheit = dht.toFahrenheit(centigrados);
    float heatIndexC = dht.computeHeatIndex(centigrados, humedad, false);
    float heatIndexF = dht.computeHeatIndex(farenheit, humedad, true);
    json["temperaturaC"] = String(centigrados);
    json["temperaturaF"] = String(farenheit);
    json["humedad"] = String(humedad);
    json["heat_indexC"] = String(heatIndexC);
    json["heat_indexF"] = String(heatIndexF);
    serializeJson(json, *response);
    r->send(response);
  });

  server.on("/fullauto", HTTP_GET,[](AsyncWebServerRequest *r){
    float humedad =  dht.getHumidity();
    float centigrados = dht.getTemperature();
    float farenheit = dht.toFahrenheit(centigrados);
    float heatIndexC = dht.computeHeatIndex(centigrados, humedad, false);
    float heatIndexF = dht.computeHeatIndex(farenheit, humedad, true);
    String head = "<head><meta http-equiv=\"refresh\" content=\"5\"></head>";
    String humidity =  "<h1>Humedad: " + String(humedad) + "</h1>";
    String temp = "<h1>Centigrados: " + String(centigrados) + "</h1> <h1>Farenheit: " + String(farenheit) + "</h1>";
    String heatIndex = "<h1>HICentigrados: " + String(heatIndexC) + "</h1> <h1>HIFarenheit: " + String(heatIndexF) + "</h1>";
    String html = head + humidity + temp + heatIndex;
    r->send(200, "text/html", html);
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.begin();

}
void loop()
{
  delay(dht.getMinimumSamplingPeriod());
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();                            
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }
  Serial.print( dht.getStatusString() );
  Serial.print("\t");
  Serial.print(humedad, 1);
  Serial.print("\t\t");
  Serial.print(temperatura, 1);
  Serial.print("\t\t");
  Serial.print( dht.toFahrenheit(temperatura), 1);
  Serial.print("\t\t");
  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(2000);
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
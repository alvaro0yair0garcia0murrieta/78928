
#include<WiFi.h>
#include<ESPAsynWebServer.h>

const char* ssid="dr-rojano";
const char* password="78928-tw";
AsyncWebServer server(80);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  conectarse();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int numParametros = request -> params();
    Serial.print(numParametros);
    if(numParametros == 0){
      request -> send(200,"text/html","<h1>hola mundo</h1>");
    }else{
            AsyncWebServer *p = request -> getParam(0);
            String html = "<h1>Hola " + p->value() + " desde ESP32</h1>";
            request -> send(200,"text/html", html);
    }
  });
  server.on("/adios", HTTP_GET[](AsyncServerRequest *r){
    r->send(200, "text/html", "<h1>adios</h1>");
  });
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
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
}

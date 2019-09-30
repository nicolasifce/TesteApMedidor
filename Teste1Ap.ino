 #include <WiFiClient.h>
 #include <AsyncTCP.h>
 #include <ESPAsyncWebServer.h>
 #include "index.h"
// #include "SSD1306.h"

 //SSD1306 display(0x3c, 4, 15);

String rede ;
String senha_wifi;

const char* PARAM_INPUT_1 = "rede";
const char* PARAM_INPUT_2 = "senha_wifi";

const char* rede = "TELECENTRO";
const char* senha1 = "12#$56qwidear";
  
const char* ssid = "Nicolas";
const char* senha = "12345678";

AsyncWebServer server(80);

/*void Root(){
  String s = MAIN_page;
  server.send(200,"text/html",s);  
}

void handleSave(){
Serial.println(server.arg("text"));

}*/


void setup(){
Serial.begin(115200);
//display.init();
Serial.println();
Serial.printf("Configurando ponto de acesso '%s'n", ssid);
WiFi.mode(WIFI_MODE_APSTA);
WiFi.softAP(ssid,senha);
/*WiFi.begin(rede, senha1);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Conectando ao Wifi");
 }
 */
 Serial.println("Wifi Conectado");
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",MAIN_page);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      rede = inputMessage;
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_1;
      senha_wifi = inputMessage;
    }
    
   });
  
server.begin();
//display.drawString(0, 0, mensagem);
//display.display();
}

void loop(void){
Serial.println(WiFi.softAPIP());
Serial.println(rede);
Serial.println(senha_wifi);
delay(5000);

}

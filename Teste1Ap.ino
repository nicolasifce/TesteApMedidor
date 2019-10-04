 #include <WiFiClient.h>
 #include <AsyncTCP.h>
 #include <ESPAsyncWebServer.h>
 #include "index.h"
 #include <FS.h>
 #include "SPIFFS.h"

 const int PIN_AP = 2;
 

String rede ;
String senha_wifi; 

const char* PARAM_INPUT_1 = "rede";
const char* PARAM_INPUT_2 = "senha_wifi";
  
const char* ssid = "Nicolas";
const char* senha = "12345678";

AsyncWebServer server(80);

void setup(){
Serial.begin(115200);
pinMode(PIN_AP, INPUT);
openFS();
conectando();
}

void loop(void){
String leiturarede = readFile("/rede.txt");
String leiturasenha = readFile("/senha.txt");

Serial.println(leiturarede);
Serial.println(leiturasenha);

if(digitalRead(PIN_AP) == HIGH){
  AP();
  }
delay(5000);

}





//---------- Funções do Web Server --------------------
//Função que Faz conexão com a internet
void conectando(){
if(!SPIFFS.exists("/rede.txt")){
  Serial.println("não existe arquivo");
    AP();
  }else{
    Serial.println("existe arquivo");
    WiFi.mode(WIFI_STA);
    const String lr = readFile("/rede.txt");
    const String ls = readFile("/senha.txt");
    //const char* lr = "IOT";
    //const char* ls = "12#$56qwIOT";
    WiFi.begin((const char*)lr.c_str(),(const char*)ls.c_str());
    while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Conectando ao Wifi");
     }
     Serial.println("Wifi Conectado");
  }
}

//Função que entra no Web Server.
void AP(){
//Formata o Sistema de Arquivos
SPIFFS.format();
openFS();
Serial.printf("Configurando ponto de acesso '%s'n", ssid);
WiFi.mode(WIFI_AP);
WiFi.softAP(ssid,senha);

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",MAIN_page);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String redeA;
    String inputParam;
    String sA;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      redeA = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      rede = redeA;
      writeFile(redeA,"/rede.txt");
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      sA = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      senha_wifi = sA;
      writeFile(sA,"/senha.txt");
    }

    if(SPIFFS.exists("/rede.txt")){
    conectando();
  }
    
   });  
   
server.begin();



}

//--------------- Funções do Sistema de Arquivos ---------------
//Escrita de Dados no arquivo
void writeFile(String state, String path) { 
  //Abre o arquivo para escrita ("w" write)
  //Sobreescreve o conteúdo do arquivo
  File rFile = SPIFFS.open(path,"w+"); 
  if(!rFile){
    Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.println(state);
    Serial.print("gravou estado: ");
    Serial.println(state);
  }
  rFile.close();
}

//Leitura de dados do arquivos.
 String readFile(String path) {
  File rFile = SPIFFS.open(path,"r");
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  String content = rFile.readStringUntil('\r'); //desconsidera '\r\n'
  Serial.print("leitura de estado: ");
  Serial.println(content);
  rFile.close();
  return content;
}

//Abertura do sistema de arquivos
void openFS(void){
  //Abre o sistema de arquivos
  if(!SPIFFS.begin()){
    Serial.println("\nErro ao abrir o sistema de arquivos");
  } else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

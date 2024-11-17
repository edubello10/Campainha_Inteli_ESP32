#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "iPhone";
const char* password = "guarani10";

#define Botao 23 
bool botaoPressionado = false;

String phoneNumber = "+5519999342836";
String apiKey = "7046257";

void sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Header do POST
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.println("Mensagem enviada com sucesso.");
  } else {
    Serial.println("Erro no envio da mensagem.");
    Serial.print("Código de resposta HTTP: ");
    Serial.println(httpResponseCode);
  }

  // Libera recursos
  http.end();
}

void setup() {
  Serial.begin(115200);

  pinMode(Botao, INPUT_PULLUP); // Configurando o botão como entrada com PULL-UP interno
  
  Serial.println("Iniciando configuração...");
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) { 
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado ao Wi-Fi com sucesso!");
    Serial.print("IP do dispositivo: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique SSID e senha.");
  }
}


void loop() {
  int estadoBotao = digitalRead(Botao); // Lê o estado do botão

  // Verifica se o botão foi pressionado (ativo em nível baixo)
  if (estadoBotao == LOW && !botaoPressionado) {
    botaoPressionado = true; // Marca o botão como pressionado
    sendMessage("TOC,TOC ! A campainha foi apertada!"); // Envia a mensagem
    delay(500); 
  } 
  
  // Reseta o estado quando o botão é liberado
  if (estadoBotao == HIGH && botaoPressionado) {
    botaoPressionado = false;
  }
}

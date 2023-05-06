#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <stdlib.h>

#define LED_PIN D4
#define POT_PIN A0

const char* ssid = "BELL449";
const char* password = "E1557EAE";

int potValue;
bool ledState;

WiFiServer espServer(80);

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  ledState = LOW;
  digitalWrite(LED_PIN, ledState);

  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); /* Configure ESP8266 in STA Mode */
  WiFi.begin(ssid, password); /* Connect to Wi-Fi based on above SSID and Password */
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");
    delay(500);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  delay(100);

  Serial.print("\n");
  Serial.println("Starting ESP8266 Web Server...");
  espServer.begin(); /* Start the HTTP web Server */
  Serial.println("ESP8266 Web Server Started");
  Serial.print("\n");
  Serial.print("The URL of ESP8266 Web Server is: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  Serial.println("Use the above URL in your Browser to access ESP8266 Web Server\n");
}

void loop() {
  
  WiFiClient client = espServer.available(); /* Check if a client is available */
  if(!client)
  {
    return;
  }
  
  Serial.println("New Client!!!");
  String request = client.readStringUntil('\r'); /* Read the first line of the request from client */
  Serial.println(request);
  client.flush();
  client.setTimeout(300);
  
  if (request.indexOf("/LED") != -1) {
    // Read request body
    String requestStr = client.readString();
    Serial.println("done reading request");
  
    // Extract body from request
    int bodyStart = requestStr.indexOf("\r\n\r\n") + 4; // +4 to skip the \r\n\r\n
    String body = requestStr.substring(bodyStart);
    Serial.println("done getting the body");
  
    // Parse JSON payload
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, body);
    Serial.println("done deserializing the body");
  
    String state = jsonDoc["state"].as<String>();
    Serial.print("state: ");
    Serial.println(state);
  
    if (state == "on") {
      ledState = HIGH;
      digitalWrite(LED_PIN, ledState);
      sendJsonResponse(client, "success", "LED turned ON");
    } else if (state == "off") {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      sendJsonResponse(client, "success", "LED turned OFF");
    } else {
      sendJsonResponse(client, "error", "Invalid state value");
    }
    Serial.println("done everything");
  }
  else if(request.indexOf("/POT") != -1){
    potValue = analogRead(POT_PIN);
    if(potValue){
      char potValueStr[16];
      itoa(potValue, potValueStr, 10);
      sendJsonResponse(client, "success", potValueStr);
    }
    else{
      sendJsonResponse(client, "error", "no potentiometer value available");
    }
    
  }
  else if(request.indexOf("/DATA") != -1){
    potValue = analogRead(POT_PIN);
    char potValueStr[16];
    itoa(potValue, potValueStr, 10);
    char ledStateStr[2];
    if (ledState) {
      strcpy(ledStateStr, "1");
    } else {
      strcpy(ledStateStr, "0");
    }
//    char message[18];
//    strcpy(message, ledStateStr);
//    strcat(message, potValueStr);
    sendJsonResponse(client, "success", ledStateStr, potValueStr);
    //sendJsonResponse(client, "error", "no data available");
  }
  else {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<h1>Not Found</h1>");
  }

  client.stop();
  Serial.println("Client disconnected");
  delay(100);
}




void sendJsonResponse(WiFiClient client, const char* status, const char* message) {
  StaticJsonDocument<128> doc;
  doc["status"] = status;
  doc["message"] = message;
  const JsonObject& response = doc.as<JsonObject>();
  String jsonResponse;
  serializeJson(response, jsonResponse);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *"); // allow requests from any domain
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(jsonResponse.length());
  client.println();
  client.println(jsonResponse);
}


void sendJsonResponse(WiFiClient client, const char* status, const char* ledState, const char* potValue) {
  StaticJsonDocument<128> doc;
  doc["status"] = status;
  doc["led"] = ledState;
  doc["pot"] = potValue;
  const JsonObject& response = doc.as<JsonObject>();
  String jsonResponse;
  serializeJson(response, jsonResponse);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *"); // allow requests from any domain
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(jsonResponse.length());
  client.println();
  client.println(jsonResponse);
}

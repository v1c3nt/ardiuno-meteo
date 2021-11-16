#include "DHTesp.h"
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
DHTesp dht;
WiFiManager wm;

const char* ssid = "ESP32AP";
const char* password = "devkit1234";
WebServer server(80);

const char* place = "terrasse";
 
void handleRoot()
{
String page ="<!DOCTYPE html>";
page += "<html lang='en'>";
page +="<head>";
page +="    <meta charset='UTF-8'>";
page +="    <meta http-equiv='X-UA-Compatible' content='IE=edge'>";
page +="    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
page +="    <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css' integrity='sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3' crossorigin='anonymous'>";
page +="    <title>terrasse ESP32</title>";
page +="</head>";
page +="<body>";
page +="    <div class='card col-8 offset-2 mt-4'>";
page +="    <div class='card-hearder text-center'>";
page += "" + timeClient.getFormattedTime();
page +="    </div>";
page +="    <div class='card-body text-center'>";
page +="    <div>";
page +="        Temperature = " + String(dht.getTemperature())+" °C";
page +="    </div>";
page +="    <div>";
page +="        Huminidé = " + String(dht.getHumidity())+" %";
page +="    </div>";
page +="    </div>";
page +="</body>";
page +="</html>";

server.setContentLength(page.length());
server.send(200, "text/html", page);

}

void api()
{
String api ="{\"meteo\": {\"temperature\" : \""+ String(dht.getTemperature())+"\",\"humidite\" : \"" + String(dht.getHumidity())+"\"}}";

server.setContentLength(api.length());
server.send(200, "application/json", api);

}

void setup() {
 
  Serial.begin(9600);

  dht.setup(2, DHTesp::DHT11);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println("\n");

  if(!wm.autoConnect(ssid, password))
    Serial.println("erreur");
  else
    Serial.println("Ok");
  // Initialise la capteur DHT11

  server.on("/", handleRoot);
  server.on("/api", api);
  server.begin();
  timeClient.begin();
  pinMode(13, OUTPUT);

}
 
void loop() {

    server.handleClient();
  // if(touchRead(T0) < 50)
  // {
  //   Serial.println("reset");
  //   wm.resetSettings();
  //   ESP.restart();
  // }


  timeClient.update();
  Serial.println(dht.getTemperature());
  Serial.println(WiFi.localIP());
  delay(5000);
 
}
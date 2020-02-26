#include <MQ7.h>
#include <DHT.h>
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#define WIFI_SSID "Autobot"
#define WIFI_PASSWORD "d91402NB"

#include <PubSubClient.h>
const char* mqttServer = "10.42.0.185";
const int mqttPort = 1883;
//const char* mqttUser = "Hello";
//const char* mqttPassword = "World";
WiFiClient espClient;
PubSubClient client(espClient);

MQ7 mq7(A0,3.3);
int CO;
int R;
float t,h;
unsigned long lastMillis1 = 0;
unsigned long lastMillis2 = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(4,INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());  
  
  dht.begin();          

  client.setServer(mqttServer, mqttPort);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);


    
    // Attempt to connect
    //    if (client.connect(clientId.c_str(),mqttUser,mqttPassword)) {
    //      Serial.println("connected");
    //    }
 
      if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      } 
    
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 3 seconds");
        delay(3000);
      }
  }
}





void loop() 
{
  
   if(digitalRead(4)==HIGH)
   {
    //Serial.println("No Rain");
    R = 0;
   }

   if(digitalRead(4)==LOW)
   {
    //Serial.println("Raining");
    R = 1;
   }

 
    t = dht.readTemperature();
    h = dht.readHumidity();
    CO = mq7.getPPM();
//    Serial.print("Temperature: ");
//    Serial.println(t);
//    Serial.print("Humidity: ");
//    Serial.println(h);
//    Serial.print("Co gas:");
//    Serial.println(CO);
    reconnect();

      if(millis() - lastMillis1 > 3000)
      {
        lastMillis1 = millis();
        char _payload[50];
        String data = String(t) + "/" + String(h) + "/" + String(CO) + "/" + String(R);
        data.toCharArray(_payload,50);
        client.publish("/iot/realtime", _payload);
      }
	  
	    if(millis() - lastMillis2 > 60000)
      {
        lastMillis2 = millis();
        char _payload[50];
        String data = String(t) + "/" + String(h) + "/" + String(CO) + "/" + String(R);
        data.toCharArray(_payload,50);
        client.publish("/iot/log", _payload);
      }
    
}

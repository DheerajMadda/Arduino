#include <MQ7.h>
#include <DHT.h>
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#define WIFI_SSID "realmec2"
#define WIFI_PASSWORD "qwertyuiop"

MQ7 mq7(A0,3.3);
int CO;
int R;
float t,h;


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
}

void loop() 
{
  
  if(digitalRead(4)==HIGH)
  {
    Serial.println("No Rain");
    R = 0;
  }

  if(digitalRead(4)==LOW)
  {
    Serial.println("Raining");
    R = 1;
  }

    t = dht.readTemperature();
    h = dht.readHumidity();
    CO = mq7.getPPM();
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.print("Co gas:");
    Serial.println(CO);

  delay(3000);

}

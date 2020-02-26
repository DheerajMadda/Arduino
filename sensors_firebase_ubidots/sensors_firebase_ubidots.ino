#include <MQ7.h>
#include <DHT.h>
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "UbidotsMicroESP8266.h"

//Firebase Credentials
#define FIREBASE_HOST "xxxxxxxxxxx.firebaseio.com"
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

//Ubidots Credentials
#define TOKEN  "xxxxxxxxxxxxxxxxxxxxxxxxx"  
#define ID_1 "CO1" 

#define WIFI_SSID "xxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxx"

MQ7 mq7(A0,3.3);
int CO;
int R = 0;
float t,h;

Ubidots client(TOKEN);

void setup() // one time execute
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  dht.begin();          // Start DHT sensor
}

void loop() // infinite time execute
{
  
  if(digitalRead(4)==HIGH)
  {
    Serial.println("No Rain");
    Firebase.setFloat("rain1",0);
    Firebase.setFloat("/area1/rain1",0);
      R = 0;
  }

  if(digitalRead(4)==LOW)
  {
    Serial.println("Raining");
    Firebase.setFloat("rain1",1);
    Firebase.setFloat("/area1/rain1",1);
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
    Serial.println(" ");
    Firebase.setFloat("CO1",CO);
    Firebase.setFloat("/area1/CO1",CO);
    client.add(ID_1, CO);
    Firebase.setFloat("temp1",t);
    Firebase.setFloat("/area1/temp1",t);
    Firebase.setFloat("humi1",h);
    Firebase.setFloat("/area1/humi1",h);

    if(R == 1)
    {
      Firebase.setFloat("P1",0);
      Firebase.setFloat("/area1/P1",0);
    }
    if(R == 0)
    {
      if(t<40 && h<75)
      {
      Firebase.setFloat("P1",1);
      Firebase.setFloat("/area1/P1",1);
      }
      else if(t<40 && h>75)
      {
      Firebase.setFloat("P1",2);
      Firebase.setFloat("/area1/P1",2);
      }
      else if(t>40 && h<75)
      {
      Firebase.setFloat("P1",3);
      Firebase.setFloat("/area1/P1",3);
      }
    }

  client.sendAll(true);
  delay(10);//miliseconds

}

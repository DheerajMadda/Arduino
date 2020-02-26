#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include <DHT.h>

#define WIFI_SSID "RedmiNote4"
#define WIFI_PASSWORD "qwertyuiop"
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);


// Push data on this interval
// 15 minutes = 15 * 60 * 1000 = 900000
const int dataPostDelay = 60000;  

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const int httpsPort =  443;
HTTPSRedirect client(httpsPort);


const char *GScriptId = "AKfycbwqy_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-DupOAyfNrLF";
// Prepare the url
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";


float t,h;

void setup() // one time execute
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());  

  Serial.print(String("Connecting to ")); 
  Serial.println(host);
  
  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpsPort);
    if (retval == 1) {
    flag = true;
    break;
    }
    else
       Serial.println("Connection to host failed. Retrying...");
  }

// Connection Status, 1 = Connected, 0 is not.
  Serial.println("Connection Status: " + String(client.connected()));
  Serial.flush();

  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    Serial.flush();
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }
  
  dht.begin();

}


// This is the main method where data gets pushed to the Google sheet
//void postData(String tag, float value){
 // if (!client.connected()){
 //   Serial.println("Connecting to client again..."); 
 //   client.connect(host, httpsPort);
 // }
  
// String urlFinal = url + "tag=" + tag + "&value=" + String(value);
// client.printRedir(urlFinal, host, googleRedirHost);

//}



void loop() {

 if (!client.connected()){
    Serial.println("Connecting to client again..."); 
    client.connect(host, httpsPort);
 }

 t = dht.readTemperature();
 h = dht.readHumidity();
 Serial.print("Temperature: ");
 Serial.println(t);
 Serial.print("Humidity: ");
 Serial.println(h);

 String urlFinal = url + "temp=" + t + "&humi=" + h;
 client.printRedir(urlFinal, host, googleRedirHost);

 delay (dataPostDelay);

}

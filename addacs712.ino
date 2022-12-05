#include <WiFi.h>
#include <HTTPClient.h>
#include "ACS712.h"
 ACS712  ACS(25, 5.0, 4095, 185);

const char* ssid     = "Captain elec";
const char* password = "Mi639338##";

const char* SERVER_NAME = "http://jrc.captainelectronicsbd.com/projects/solardata/gpsdata.php";

String ESP32_API_KEY = "Ad5F10jkBM0";
unsigned long previousMillis = 0;
long interval = 30000;
float voltdata;
float ampdata;


void setup() {
  
  Serial.begin(115200);
  
  Serial.println("esp32 serial initialize");
  ACS.autoMidPoint();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  
  if(WiFi.status()== WL_CONNECTED){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
       previousMillis = currentMillis;
       //Send an HTTP POST request every 30 second
    HTTPClient http;
    float voltdata = (float)analogRead(26) / 4096 * 15 * 28205 / 27000;
    
     int madata = ACS.mA_DC();
     ampdata = madata/1000;
     
     float wattdata = voltdata*ampdata;
     
    http.begin(SERVER_NAME);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String volt, amp, watt;
 
     volt = String(voltdata,2);
     amp = String(ampdata,2);
     watt = String(wattdata,2);
    String gps_data;
    gps_data = "api_key="+ESP32_API_KEY;
    gps_data += "&volt="+volt;
    gps_data += "&amp="+amp;
    gps_data += "&watt="+watt;
    
    
    Serial.print("gps_data: ");
   Serial.println(gps_data);
    int httpResponseCode = http.POST(gps_data);
    String httpResponseString = http.getString();
  
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }

    else {
      Serial.print("Error on HTTP request - Error code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }
    http.end();
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(1000);  
}

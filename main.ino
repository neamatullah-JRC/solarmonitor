#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "JRC222";
const char* password = "juniourrobotics222club";

const char* SERVER_NAME = "http://jrc.captainelectronicsbd.com/projects/solardata/gpsdata.php";

String ESP32_API_KEY = "Ad5F10jkBM0";
unsigned long previousMillis = 0;
long interval = 30000;

float volt = 12.00;
float amp = 2.5;
float watt = volt*amp;

void setup() {
  
  Serial.begin(115200);
  Serial.println("esp32 serial initialize");

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
    
    http.begin(SERVER_NAME);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
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


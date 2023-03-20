
// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Replace with your network details
const char* ssid = "ALTEC PLANTA BAJA";
const char* password = "Albarracin2021";

// Web Server on port 80
WiFiServer server(80);

// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
        strcpy(celsiusTemp,"Failed");
        strcpy(fahrenheitTemp, "Failed");
        strcpy(humidityTemp, "Failed");         
      }
      else{
        // Computes temperature values in Celsius + Fahrenheit and Humidity
        float hic = dht.computeHeatIndex(t, h, false);       
        dtostrf(hic, 6, 2, celsiusTemp);             
        float hif = dht.computeHeatIndex(f, h);
        dtostrf(hif, 6, 2, fahrenheitTemp);         
        dtostrf(h, 6, 2, humidityTemp);
      }
      String response = "{";
      response+= "\"estado\":\"Conectado\",";
      response+= "\"humedad\":\""+String(humidityTemp)+"\",";
      response+= "\"temperatura\":\""+String(celsiusTemp)+"\"";
      response+="}";
      char outBuf[50];   
      sprintf(outBuf,"{\"estado\":\"Conectado\",\"humedad\":\"%s\",\"temperatura\":\"%s\"}\n",String(humidityTemp),String(celsiusTemp));
      client.write(outBuf);
      delay(1);
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
*********/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
// Replace with your network credentials
const char* ssid = "ASUS_40_2G_2.4GEXT";
const char* password = "hernandez";

////////////////////////////////////////////////////////////////////////////////////////////////////////
const int selectPins[4] = {D3,D2, D1, D0};
const int analogInput = A0;
int sensorData[15];

const int AirValue = 952;   //you need to replace this value with Value_1
const int WaterValue = 544;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
String stringToReturn;
int numOfSensors;
char *mySensors[] = {"moisture", "moisture", "photoresistor","photoresistor",
                     "empty", "empty", "empty","empty",
                     "empty", "empty", "empty","empty",
                     "empty", "empty", "empty","empty",
                    };


/* y0 - 1 - mositure
 * y1 - 9 - temp
 * y2 - 5 
 * y3 - 13
 * y4 - 3 - photoresistor
 * y5 - 11
 * y6 - 7
 * y7 - 15
 * y8 - 2 - mositure
 * y9 - 10
 * y10 - 6
 * y11 - 14
 * y12 - 4 - photoresistor
 * y13 - 12
 * y14 - 8
 * y15 - 16
 
 */
//////////////////////////////////////////////






#define DHTPIN D4     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
String temp;
String hum;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  

/////////////////////////////////////////





void setupDHT()
{
   // Set up the select pins as outputs:
    for (int i=0; i < 4; i++)
    {
      pinMode(selectPins[i], OUTPUT);
      digitalWrite(selectPins[i], HIGH);
    }
    
    // Create header table
    Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
    Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
}


int getIndex(char sensorName[]){

  int index = 0;
  for (int i = 0; i < 6; i++) {
    if(mySensors[i] == sensorName)
    {
      index = i;
    }
   
  }

  return index;

}

int getSensorData(int index){

  int data = sensorData[index];

  return data;


  
}
void returnDataVoid(int i){

      String Name = String(mySensors[i]);
     
      stringToReturn = String(sensorData[i]);
     
      Serial.println("Returning senors: ");
      Serial.println(Name);
      Serial.println("With value:");
      Serial.println(stringToReturn);
      
      
}

String returnData(){
  return stringToReturn;
}
void readSensors(){
 
  
  t = readDHT11Temperature();
  h = readDHT11Humidity();
  for (byte pin=0; pin <= 15; pin++)
  {
    Serial.println("pin:");
    Serial.print(pin);
      for (int i=0; i < 4; i++) {
          digitalWrite(selectPins[i], pin & (1 << i)?HIGH:LOW);
      }

      //skips saving data if no senors are assigned
      if(mySensors[pin] == "empty")
      {
         continue;
      }

     
      sensorData[pin] = analogRead(analogInput);
      int inputValue = analogRead(analogInput);
      Serial.print(String(mySensors[pin]) + "\t");
      Serial.print(String(": "));
      Serial.print(String(inputValue));
      Serial.println();
  
  }

  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.println();
  Serial.print(F("%  Hum: "));
  Serial.print(hum);
  Serial.println();
  Serial.println();

  
}
  
float readDHT11Temperature() {
  
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT11 sensor!");
    return 0;
  }
  else {
    
    return t;
  }
}

float readDHT11Humidity() {
 
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return 0;
  }
  else {
    
    return h;
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  setupDHT();

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    temp = String(t);
    request->send_P(200, "text/plain", temp.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    hum = String(h);
    request->send_P(200, "text/plain",  hum.c_str());
  });

  server.on("/moist", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("returnDataVoid:");
    
     returnDataVoid(0);
     Serial.println(returnData());
    request->send_P(200, "text/plain", returnData().c_str());
 });

 server.on("/moist2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("returnDataVoid:");
    
     returnDataVoid(1);
     Serial.println(returnData());
    request->send_P(200, "text/plain", returnData().c_str());
 });
 server.on("/photo", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("returnDataVoid:");
    
     returnDataVoid(2);
     Serial.println(returnData());
    request->send_P(200, "text/plain", returnData().c_str());
 });
 server.on("/photo2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("returnDataVoid:");
    
     returnDataVoid(3);
     Serial.println(returnData());
    request->send_P(200, "text/plain", returnData().c_str());
 });
  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readSensors();
  }
}


#define USE_SECRET  1

/*
 * Used board setting:
 * WeMos D1 mini
 */ 



//#define SKIP_KAAIOT_SEND

// 1 "TP-Link_BB"    
// 2 "BB_Home2"  
// 3 "P20 Pro"    
// 4 "BB_guest"


#define VER                      "v0.1"
#define BUILDNUM                      1
/*
 * Add device_name tag for RSSI
 */

#define SERIAL_BOUND_RATE         115200

// Pinout: https://circuits4you.com/2017/12/31/nodemcu-pinout/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

#include <OneWire.h> 
#include <DallasTemperature.h>

#include "secrets.h"
#include "params.h"

#include "webUI2.h"

#define ONE_WIRE_BUS 0  // 0 means D3 on Wemos Mini and NodeMCU boards.. 

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);


//==================================
// WIFI
//==================================
const char* ssid =           S_SSID;
const char* password =       S_PASS;



// =================================
// HTTP
// =================================
ESP8266WebServer server(SERVER_PORT);
HTTPClient http;


// EEPROM
//==================================
int eepromAddr =                   1;
int eepromAddr2 =                  4;


//==================================
// "Data" variables
//==================================
float valC = NAN;

//==================================
// Other variables
//==================================
long timeOfLastMeasurement = 0;      //time of the last measurement
long lastWiFiCheck = 0;
int errorCount = 0;

unsigned long elapsedTime = 0;


// ==========================================================================================================================
//                                                 Hw feedback function
// ==========================================================================================================================

void turnLED(int state){
  if(state > 0){
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }else{
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}


// ==========================================================================================================================
//                                                     Other methods
// ==========================================================================================================================

void doRestart(){
  Serial.println("Restart now...");
  ESP.restart();
}


void saveToEEPROM(int addr, short value){
  EEPROM.write(addr, value);
  EEPROM.commit();
}

short loadFromEEPROM(int addr){
  return EEPROM.read(addr);
}

// ==========================================================================================================================
//                                                  Web server functions
// ==========================================================================================================================

void HandleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}



void actionSet(){
  String m = "";
  for (uint8_t i=0; i<server.args(); i++){
    if( server.argName(i) == String("param") ){
        // TEMP_SET
        String value = server.arg(i);
        int set = value.toInt();

        // save into eeprom
        saveToEEPROM(eepromAddr, set);

        Serial.println("Try to SET: " + String(set) );
        m += String("<br>Target param set:\n") + String(set) + "C\n<br>";
    }
  }
//  m+= getHTMLAutomaticGoBack(DELAY_BEFORE_REFRESH_PAGE_AFTER_SET_ENDPOINT_IN_MS);
  server.send(200, "text/html", m );
}



void HandleNotRstEndpoint(){
  doRestart();
}


void HandleRoot(){
  //Serial.println("HandleRoot called...");
  String message = "";
  message = sendHTML(valC, WiFi.RSSI());
  server.send(200, "text/html", message );
}



// ==========================================================================================================================
//                                                        WiFi handling
// ==========================================================================================================================

int connectToWiFi(String ssid, String pw){
  // Setup WIFI
  WiFi.begin(ssid, password);
  Serial.println("Try to connect (SSID: " + String(ssid) + ")");
  
  // Wait for WIFI connection
  int tryCount = 0;
  while ( (WiFi.status() != WL_CONNECTED) && (tryCount < WIFI_TRY_COUNT)) {
    delay(WIFI_WAIT_IN_MS);
    tryCount++;
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    return 1;
  }else{
    return 0;
  }
}

int initWiFi(){
  
  if(connectToWiFi(ssid, password)){
    Serial.println("");
    Serial.println("Connected");
    String ipMessage = "I" + WiFi.localIP().toString();
    //Serial.println("SENT: " + ipMessage );
    
    // for reconnecting feature
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.hostname(String(SOFTWARE_NAME) + " [" + String(IOT_DEVICE_ID) + "] " + String(VER) + " b" + String(BUILDNUM));
  }else{
    Serial.println("");
    Serial.print("ERROR: Unable to connect to ");
    Serial.println(ssid);
    doRestart();
  }
}


void wifiConnectionCheck(long now){
  //TODO: need to add optional macro for Serial prints of WiFi state
  if( (now - lastWiFiCheck) >= DELAY_BETWEEN_WIFI_CONNECTION_STATUS_CHECKS_IN_MS ){
    lastWiFiCheck = now;
    
    switch (WiFi.status()){
      case WL_NO_SSID_AVAIL:
        Serial.println("Configured SSID cannot be reached");
        turnLED(ON);
        break;
      case WL_CONNECTED:
        turnLED(OFF);
        //Serial.println("Connection successfully established");
        break;
      case WL_CONNECT_FAILED:
        turnLED(ON);
        Serial.println("Connection failed");
        //initWiFi();
        break;
    }
    /*
    Serial.printf("Connection status: %d\n", WiFi.status());
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    /**/
  }
}

// ==========================================================================================================================
//                                                        Init
// ==========================================================================================================================
 
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  elapsedTime = millis();
  
  turnLED(ON);

  errorCount = 0;
  
  Serial.begin(SERIAL_BOUND_RATE);
  Serial.println();
  Serial.println("\n");
  Serial.print(String(TITLE) + " ");
  Serial.println(VER);

  // EEPROM
  EEPROM.begin(128);
  //tempSet = loadFromEEPROM(eepromAddr);


  initWiFi();

  server.on("/", HandleRoot);
  server.on("/rst", HandleNotRstEndpoint);
  server.on("/set", actionSet);
  server.onNotFound( HandleNotFound );
  server.begin();
  Serial.println("HTTP server started at ip " + WiFi.localIP().toString() + String("@ port: ") + String(SERVER_PORT) );

  //Serial.println("short: " + String(sizeof(short)));
  
  turnLED(OFF);

  Serial.println("Temperature (C)");
  timeOfLastMeasurement = -100000;

  delay(5000);
  
}


// ==========================================================================================================================
//                                                        Loop
// ==========================================================================================================================


int updateCounter = 0;



void sensorLoop(long now){
  if( (now - timeOfLastMeasurement) > DELAY_BETWEEN_ITERATIONS_IN_MS ){ //Take a measurement at a fixed time (durationTemp = 5000ms, 5s)
    elapsedTime = now;
  
    turnLED(ON);
    delay(100);
    sensors.requestTemperatures(); // Send the command to get temperature readings 
    valC = sensors.getTempCByIndex(0) + TEMPERATURE_CORRECTION;
    turnLED(OFF);

    
    Serial.print(valC, 2);
    Serial.print(" C\t\t");
    Serial.println(String(WiFi.RSSI()));

    timeOfLastMeasurement = now;
  
    if(errorCount >= ERROR_COUNT_BEFORE_RESTART){
      Serial.println("\nDefined error count (" + String(ERROR_COUNT_BEFORE_RESTART) + ") reched!");
      doRestart();
    }
  }
  
}
 
void loop() {
  long now = millis();
  server.handleClient();
  wifiConnectionCheck(now);
  sensorLoop(now);
}

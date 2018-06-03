/*
 * Try send both Temperature and Humidity using Node-Red, Node-Red UI and UDP server
 * 03 June 2018
 */

#include "AIS_NB_BC95.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

String serverIP = "172.245.105.173"; // Your Server IP
String serverPort = "5656"; // Your Server Port

char jsonData[] = "{\"temperature\":%d, \"humidity\":%d }\0";
char buff[48];
int jsonData_len = 0;

//DHT11 config
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
String payload;
int Humidity;  //Stores humidity value
int Temperature; //Stores temperature value

AIS_NB_BC95 AISnb;

const long interval = 20000;  //millisecond
unsigned long previousMillis = 0;

void setup(){ 
  AISnb.debug = true;
  
  Serial.begin(9600);
  dht.begin();
  
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();  
  delay(1000);
  
  pingRESP pingR = AISnb.pingIP(serverIP);
  previousMillis = millis();

}

void loop(){ 
  unsigned long currentMillis = millis();

  //Read data and store it to variables hum and temp
  Temperature= dht.readTemperature();
  Humidity = dht.readHumidity();

  if (currentMillis - previousMillis >= interval) {   
    //Print temp and humidity values to serial monitor
    Serial.print("Temperature: ");
    Serial.print(Temperature);
    Serial.print(" Celsius, Humidity: ");
    Serial.print(Humidity);
    Serial.println(" %");
    
    sprintf(buff, jsonData, (int)Temperature, (int)Humidity);
    //sprintf(buff, jsonData, Temperature, Humidity );
    //Serial.println(buff);
    
      // Send data in String 
      UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, String(buff));
   
      //Send data in HexString     
      //udpDataHEX = AISnb.str2HexStr(udpData);
      //UDPSend udp = AISnb.sendUDPmsg(serverIP, serverPort, udpDataHEX);
      previousMillis = currentMillis;
  
    }
  UDPReceive resp = AISnb.waitResponse();
     
}




//    __   _ __                    
//   / /  (_) /  _______ _______ __
//  / /__/ / _ \/ __/ _ `/ __/ // /
// /____/_/_.__/_/  \_,_/_/  \_, / 
//                          /___/  
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <Wire.h>
#include <SPI.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>


#define BOTtoken "" // add Token
#define SEALEVELPRESSURE_HPA (1013.25)

// SCL geht auf Pin A5 auf der Analog Seite und SDA geht auf Pin A4
// Vin ist Strom rein und Gnd ist Ground
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
unsigned long delayTime;

// DeepSleep
const int sleepSeconds = 900;

#define USE_SERIAL Serial

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


//    ____    __          
//   / __/__ / /___ _____ 
//  _\ \/ -_) __/ // / _ \
// /___/\__/\__/\_,_/ .__/
//                 /_/    
//////////////////////////////////////////////////////////////////////////

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    while(!Serial);    // time to get serial running
    Serial.println(F("Wemos_D1_mini_BME280"));

    unsigned status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("Ich bin Verbunden");


    //DeepSleep
    pinMode(D0, WAKEUP_PULLUP);
}



//    __  ___     _      __                
//   /  |/  /__ _(_)__  / /  ___  ___  ___ 
//  / /|_/ / _ `/ / _ \/ /__/ _ \/ _ \/ _ \
// /_/  /_/\_,_/_/_//_/____/\___/\___/ .__/
//                                  /_/    
//////////////////////////////////////////////////////////////////////////
void loop() {
    //delayTime = 2000;
    // put your main code here, to run repeatedly:
    //printValues();
    //delay(delayTime);
    telegramSendTemp();

// DeepSleep
    ESP.deepSleep(sleepSeconds * 1000000);
}





//    ___  __  _______  ___  ___  ___    ___      _      __ 
//   / _ )/  |/  / __/ |_  |( _ )/ _ \  / _ \____(_)__  / /_
//  / _  / /|_/ / _/  / __// _  / // / / ___/ __/ / _ \/ __/
// /____/_/  /_/___/ /____/\___/\___/ /_/  /_/ /_/_//_/\__/ 
//////////////////////////////////////////////////////////////////////////

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

//  ______    __                      
// /_  __/__ / /__ ___ ________ ___ _ 
//  / / / -_) / -_) _ `/ __/ _ `/  ' \
// /_/  \__/_/\__/\_, /_/  \_,_/_/_/_/ Temperatur
//               /___/   
//////////////////////////////////////////////////////////////////////////

void telegramSendTemp(){

  String chat_id = "259572712";
  String Temperatur = "";
  String Luftfeuchtigkeit = "";
  String Luftdruck = "";

  
  Temperatur = String(bme.readTemperature());
  Luftfeuchtigkeit = String(bme.readHumidity());
  Luftdruck = String(bme.readPressure()/ 100.0F);

  // Sendet Nachrichten
  bot.sendMessage(chat_id,"MM|VR BA. MA. Raum\nTemperatur = " + Temperatur + " *C\n" + "Luftfeuchtigkeit = " + Luftfeuchtigkeit + " %\n" + "Luftdruck = " + Luftdruck + " hPa\n");
  //bot.sendMessage(chat_id, "Temperatur = " + Temperatur + " *C");
  //bot.sendMessage(chat_id, "Luftfeuchtigkeit = " + Luftfeuchtigkeit + " %");
  //bot.sendMessage(chat_id, "Luftdruck = " + Luftdruck + " hPa");


  }

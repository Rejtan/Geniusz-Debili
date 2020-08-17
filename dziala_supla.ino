/**
 * Supla.org NodeMCU WiFi minimal example
 * Author: Programistyk - Kamil Kaminski <kamil@programistyk.pl>
 * 
 * This example shows how to configure SuplaDevice for building for NodeMCU within Arduino IDE
 */

int button1;  //wartość dla przycisku1
int Step = 4; 
int Dir  = 5; 


#include <srpc.h>
#include <log.h>
#include <eh.h>
#include <proto.h>
#include <IEEE754tools.h>
// We define our own ethernet layer
#define SUPLADEVICE_CPP

#include <SuplaDevice.h>
#include <lck.h>


#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiUdp.h>

WiFiClient client;

// Setup Supla connection
const char* ssid     = "dom"; //np. const char* ssid     = "to wcale nie jest CBA";
const char* password = "54123456"; //np. const char* password = "1jednaktak1";



void setup() {
  pinMode(Step, OUTPUT); //Step pin as output
 pinMode(Dir,  OUTPUT); //Direcction pin as output
 digitalWrite(Step, LOW); // Currently no stepper motor movement
 digitalWrite(Dir, LOW); 
  Serial.begin(115200);
  delay(10);


  // Replace the falowing GUID
  char GUID[SUPLA_GUID_SIZE] = {0x81,0x24,0x11,0x0A,0x78,0xF4,0x15,0x36,0x79,0xE5,0x69,0xA8,0xA1,0x39,0x34,0x99};
  // with GUID that you can retrieve from https://www.supla.org/arduino/get-guid

  // Ethernet MAC address
  uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Nasze przekaźniki
 SuplaDevice.addRelay(D6);




 

  SuplaDevice.begin(GUID,              
                    mac,               
                    "svr11.supla.org",  // adres serweru SUPLI
                    4143,                 // ID lokacji
                    "df62");               // Hasło lokacji

}

void loop() {
  SuplaDevice.iterate();
  
TSD_SuplaChannelNewValue value1; 
value1.SenderID = 0; 
value1.ChannelNumber = 0; // Kanał naszego przekaźnika
value1.DurationMS = 0; 

button1 = digitalRead(D3); //zczytanie stanu przycisku podłączonego pod D3 do funkcji button1
if(digitalRead(D3)==LOW){ //jeśli stan jest niski
value1.value[0] = !value1.value[0]; //sprawdzenie stanu przekaźnika
SuplaDevice.channelSetValue(&value1); //zmiana stanu
while(digitalRead(D3)==LOW); //przycisk długo wciśnięty
delay(20);
}


  int i=0;

  
  if (digitalRead(D6)==HIGH)  { //Move 50 steps forward
    digitalWrite(Dir, HIGH); //Rotate stepper motor in clock wise direction
          for( i=1;i<=150;i++){
          digitalWrite(Step, HIGH);
          delay(10);
          digitalWrite(Step, LOW);
          delay(10);}

  }
  
  if (digitalRead(D6)==LOW)  { //Move 50 steps backwards
    digitalWrite(Dir, LOW); //Rotate stepper motor in anti clock wise direction
          for( i=1;i<=150;i++){
          digitalWrite(Step, HIGH);
          delay(10);
          digitalWrite(Step, LOW);
          delay(10);}
    
  }


}

// Supla.org ethernet layer
    int supla_arduino_tcp_read(void *buf, int count) {
        _supla_int_t size = client.available();
       
        if ( size > 0 ) {
            if ( size > count ) size = count;
            return client.read((uint8_t *)buf, size);
        };
    
        return -1;
    };
    
    int supla_arduino_tcp_write(void *buf, int count) {
        return client.write((const uint8_t *)buf, count);
    };
    
    bool supla_arduino_svr_connect(const char *server, int port) {
          return client.connect(server, 2015);
    }
    
    bool supla_arduino_svr_connected(void) {
          return client.connected();
    }
    
    void supla_arduino_svr_disconnect(void) {
         client.stop();
    }
    
    void supla_arduino_eth_setup(uint8_t mac[6], IPAddress *ip) {

       // Serial.println("WiFi init");
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        //    Serial.print(".");
        }

        //Serial.print("\nlocalIP: ");
        //Serial.println(WiFi.localIP());
        //Serial.print("subnetMask: ");
        //Serial.println(WiFi.subnetMask());
        //Serial.print("gatewayIP: ");
        //Serial.println(WiFi.gatewayIP());
    }

SuplaDeviceCallbacks supla_arduino_get_callbacks(void) {
          SuplaDeviceCallbacks cb;
          
          cb.tcp_read = &supla_arduino_tcp_read;
          cb.tcp_write = &supla_arduino_tcp_write;
          cb.eth_setup = &supla_arduino_eth_setup;
          cb.svr_connected = &supla_arduino_svr_connected;
          cb.svr_connect = &supla_arduino_svr_connect;
          cb.svr_disconnect = &supla_arduino_svr_disconnect;
          cb.get_temperature = NULL;
          cb.get_temperature_and_humidity = NULL;
          cb.get_rgbw_value = NULL;
          cb.set_rgbw_value = NULL;
          
          return cb;
}

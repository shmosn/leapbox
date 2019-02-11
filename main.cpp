#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> //MQTT Broker Server
#include <iostream>
#include<string>
#include<sstream>

const char* ssid = "FRITZ!Box 6490 Cable";
const char* pass = "honisoitquimalypense!";
const char* brokerUser = "boxleap@gmail.com"; //login info on dioty.co
const char* brokerPass = "32ec3f29";
const char* broker = "mqtt.dioty.co";
const char* inTopic = "/boxleap@gmail.com/esptest";
const char* outTopic = "/boxleap@gmail.com/esptestreply";

const int lockPin(3);
const int wifiIndicatorPin(19);
const int loginToDioty(18);
const int ledPin25(5); //pin 8 = GPIO05
const int ledPin50(17); //pin 9 = GPIO17
const int ledPin75(16); //pin 10 = GPIO16
const int ledPin100(4); // pin 11 = GPIO04

char messages[50];

WiFiClient espClient; // NOT sure what that does
PubSubClient client(espClient); // Nor this.., after this: reconnect function for server connection

int getValueFromMessage(std::string msg, std::string id);
int percentageToNumberOfLeds(int value);
void lightProcessLEDs(int numberOfLeds, int ledPin1, int ledPin2, int ledPin3, int ledPin4);

void identifyMessage(std::string msg){
    // see what kind of message we receive
    
    std::string substring = msg.substr(0,2);
    if(substring == "pe"){
      const int value(getValueFromMessage(msg, "pe"));
      const int numberOfLeds = percentageToNumberOfLeds(value);
      lightProcessLEDs(numberOfLeds, ledPin25, ledPin50, ledPin75, ledPin100);

      const char * b = (char*)value;
      client.publish(outTopic, "Process LEDs updated to:");
      // SEEMS TO CRASH client.publish(outTopic, b);
    }
    else if(msg.substr(0,4) == "lock"){
        digitalWrite(lockPin, LOW);
        client.publish(outTopic, "box now locked.");
    }

    else if(msg.substr(0,6) == "unlock") {
        digitalWrite(lockPin, HIGH);
        client.publish(outTopic, "box now unlocked.");
    }

}


void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

    while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
    
    digitalWrite(loginToDioty, LOW);
    delay(100);
    digitalWrite(loginToDioty, HIGH);
    }

  Serial.print("\nConnected to ");
  Serial.println(ssid);
}

void reconnect(){
  while(!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.println(broker);
    if(client.connect("dev0001", brokerUser, brokerPass)){
      Serial.print("\nConnected to ");
      Serial.println(broker);
      digitalWrite(wifiIndicatorPin, HIGH);
      client.subscribe(inTopic);
    } else {
      Serial.println("\nTrying to connect again...");
      //delay(5000); // instead: for-loop for flashing LED as long as not connected..
      for (int a = 0; a < 33; a++) {
        digitalWrite(wifiIndicatorPin, HIGH);
        delay(50);
        digitalWrite(wifiIndicatorPin, LOW);
        delay(100);
      }
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){ //callback function to receive data from the server
  Serial.print("Received message: ");
  //Serial.print(topic);
  char message_buff[100]; //http://blue-pc.net/2014/10/15/mqtt-nachrichten-mit-dem-arduino-empfangen-senden/
  int i = 0;
  Serial.println("Message arrived: topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));
  
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff); // Converting message to string
  //Serial.println("Payload: " + msgString);
  //Serial.println(msgString[0]);
 
  // ------- MESSAGE PARSE --------
  
  identifyMessage(msgString.c_str());

  if(msgString[0]=='t' || msgString[0]=='f')
    client.publish(outTopic, "try either 'pex', where x is a value between 0 and 100, or 'lock' / 'unlock'");

}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); //ESP32 WROOM board: blaue SMD LED
  pinMode(wifiIndicatorPin, OUTPUT);
  pinMode(loginToDioty, OUTPUT); // server connection status: flashing = connecting, on = connected
  pinMode(lockPin, OUTPUT); // controlled Light by MQTT message
  pinMode(ledPin25, OUTPUT);
  pinMode(ledPin50, OUTPUT);
  pinMode(ledPin75, OUTPUT);
  pinMode(ledPin100, OUTPUT);
  
  setupWifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!client.connected()){
    reconnect();
  }
  client.loop();

}

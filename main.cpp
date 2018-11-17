#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> //MQTT Broker Server

const char* ssid = "YOUR WIFI SSID";
const char* pass = "YOUR WIFI PASSWORD";
const char* brokerUser = "USE YOUR BROKER USER MAIL"; //login info on dioty.co
const char* brokerPass = "USE YOUR BROKER PASS";
const char* broker = "mqtt.dioty.co";
const char* inTopic = "/YOURBROKERUSERADDRESS/EspTest";

WiFiClient espClient; // NOT sure what that does
PubSubClient client(espClient); // Nor this.., after this: reconnect function for server connection


void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

    while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
    
    digitalWrite(18, HIGH);
    delay(100);
    digitalWrite(18, LOW);
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
      digitalWrite(19, HIGH);
      client.subscribe(inTopic);
    } else {
      Serial.println("\nTrying to connect again...");
      //delay(5000); // instead: for-loop for flashing LED as long as not connected..
      for (int a = 0; a < 33; a++) {
        digitalWrite(19, HIGH);
        delay(50);
        digitalWrite(19, LOW);
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
  Serial.println("Payload: " + msgString);
  Serial.println(msgString[1]);
  /*for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
  */
  // filter out true false

  if(msgString[0]=='t'){
    digitalWrite(21, HIGH);
  } else {
    digitalWrite(21, LOW);
  }
} 

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); //ESP32 WROOM board: blaue SMD LED
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT); // server connection status: flashing = connecting, on = connected
  pinMode(21, OUTPUT); // controlled Light by MQTT message
  setupWifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()){
    reconnect();
  }
  client.loop();
}

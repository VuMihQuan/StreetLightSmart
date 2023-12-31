// Khai bao cac thu vien can thiet
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FirebaseESP8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <stdio.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(D2, D1); 
SoftwareSerial sim800(D4,D3);
SoftwareSerial mySerial(D7, D8);

String phoneNumber = "84388309938";
String smsStatus = "";
String senderNumber = "";
String receivedNumber = "";
String receivedDate = "";
String mess = "";
bool accept = false;
void extractSms(String buff);
void parseData(String buff);
int lightBroken(float *arr);
void Reply(String text, String Phone);
void sendLocation(String Phone);
float CurrentArr[4];


#define FIREBASE_HOST "test-sensor-98303-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyA4IGoOmWq0-UIlNxq38sMBUciiXi3oEjg"

// Update these with values suitable for your network.
// khai bao dia chi ket noi wifi, mqtt server
const char* ssid = "MSI 9153";
const char* password = "MSI189202";
// const char* ssid = "VNU LIC 501";
// const char* password = "vnulic@2023";
const char* mqtt_server = "test.mosquitto.org";

// Ket noi va giao tiep thong qua mqtt
// Tao doi tuong de quan li ket noi wifi tren esp8266
WiFiClient espClient;
PubSubClient client(espClient);

FirebaseData firebaseData;
FirebaseData fbdo;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)  
char msg[MSG_BUFFER_SIZE];
int value = 0;
int count = 0;
int controlValue= 0;
bool sm = false;
void setup_wifi() {
  delay(10);
  //  connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
// Thiet lap trang thai hoat dong(ket noi den diem cung vap wifi va password )
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   

  Firebase.reconnectWiFi(true);

}

void callback(char* topic, byte* payload, unsigned int length) {
  // unsigned long now = millis();
  // if (now - lastMsg > 2000) {
  //   lastMsg = now;
  //   ++value;    
  //   String receivedJsonControlLed = "";
  // for (int i = 0; i < length; i++) {
  //   receivedJsonControlLed += (char)payload[i];
  // }
  // DynamicJsonDocument jsonDoc(256);
  // Serial.println("Received JSON control: " + receivedJsonControlLed);
  // deserializeJson(jsonDoc, receivedJsonControlLed);
  // controlValue = jsonDoc["control_led"];
  // Serial.println(controlValue);
  
  // }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("dataToDisplay", "hello IoT Gateway ...");
      // ... and resubscribe
      client.subscribe("controlData");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(9600);
  SerialGPS.begin(9600);
  sim800.begin(9600);
  mySerial.begin(9600);
  sim800.listen();
  delay(1000);
  sim800.print("ATE0\r");
  delay(1000);
  sim800.println("AT+CMGF=1"); //SMS text mode
  delay(1000);
  sim800.println("AT+CLIP=1"); //Enable Caller ID
  delay(500);
    pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
 
}
// khoi tao file json
void Json (){
    if(mySerial.available() > 1)
  {
    String vst = mySerial.readString();
    char *buf = (char*)vst.c_str();
    char *sensor1_str = strtok(buf, ":");
    char *sensor2_str = strtok(NULL, ":");
    char *sensor3_str = strtok(NULL, ":");
    char *sensor4_str = strtok(NULL, ":");
    float sensor1 = atof(sensor1_str);
    float sensor2 = atof(sensor2_str);
    float sensor3 = atof(sensor3_str);
    float sensor4 = atof(sensor4_str);
    CurrentArr[0] = sensor1;
    CurrentArr[1] = sensor2;
    CurrentArr[2] = sensor3;
    // CurrentArr[3] = sensor4;
    if(sensor4 > 0)
    {
    CurrentArr[3] = 1;
    }
    
    if(CurrentArr[2] == 0)
    {
      sm = true;
    }
  }
  while(sim800.available()){
  parseData(sim800.readString());
  }
  // if(sm)
  // {
  //   if(!accept)
  //   {
  //     sendLocation(phoneNumber);
  //   }
    
  // }
  // sm = false; 
  DynamicJsonDocument jsonDoc(256);
  jsonDoc["voltage_value_1"] = CurrentArr[0];
  jsonDoc["voltage_value_2"] = CurrentArr[1];
  jsonDoc["voltage_value_3"] = 0;
  jsonDoc["status_led"] = CurrentArr[3];
  serializeJson(jsonDoc, Serial);

  // Serialize the JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.publish("dataToDisplay", jsonString.c_str());
   for (int i = 0; i < 4; ++i) {
        CurrentArr[i] = 0;
    }
}



void loop() {

   mySerial.write("1");
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;    
    Serial.print("Json sending: ");
  Json();
  Serial.println();
  }
    client.subscribe("controlData");
  String controlLedFromWebsite;
  Firebase.getString(firebaseData, "/control-led/signalControlLed");
  controlLedFromWebsite = firebaseData.stringData();
  if(controlLedFromWebsite =="01"){
  //  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
  //  Serial.println(controlLedFromWebsite);
    // mySerial.print("1");
  }else{
  // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
  //  Serial.println(controlLedFromWebsite);
    // mySerial.print("0");

  }


}
void extractSms(String buff)
{
  unsigned int index;
  index = buff.indexOf(",");
smsStatus = buff.substring(1, index-1);
  buff.remove(0, index+2);
  senderNumber = buff.substring(0,11);
  buff.remove(0,17);
  receivedDate  = buff.substring(0,20);
  buff.remove(0,buff.indexOf("\r"));
  buff.trim();

  index =buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  mess = buff;
  buff = "";
  mess.toLowerCase();
}

void parseData(String buff)
{
  //buff = +CMTI: "SM",1
  //buff = +CMGR: "REC UNREAD","+84354417607","","23/12/17,21:25:35+28" :))
  Serial.println(buff);
  unsigned int len, index;
  buff.trim();
  if(buff != "OK")
  {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    buff.remove(0, index + 2);
    if(cmd == "+CMTI")
    {
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length());
      temp = "AT+CMGR=" + temp + "\r";
      sim800.println(temp);
    }
    else if(cmd == "+CMGR")
    { 
      extractSms(buff);
      Serial.println(mess);
      if(mess == "confirm")
      {
        accept = true;
        Reply("Thank you", phoneNumber);
      }
      else if(mess == "List")
      {

      }
      else Reply("Incorrect syntax, please send \"Confirm\" to confirm information or \"List\" to see the list of broken light locations.", phoneNumber);
    }
  }
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  mess="";
}
// int lightBroken(float *arr)
// {
//   for(int i = 0; i < sizeof(arr); i++)
//   {
//     if(arr[i] < 0)
//     {return 1;}
//   }
//   return 0;
// }
void Reply(String text, String Phone)
{
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\""+Phone+"\"\r");
  delay(1000);
  sim800.print(text);
  delay(100);
  sim800.write(26); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(1000);
  Serial.println("SMS Sent Successfully.");
}  

void sendLocation(String Phone) 
{
  boolean newData = false;
  for(unsigned long start = millis(); millis() - start < 20000;)
  {

    // String text = String("The light in area ") + String("http://maps.google.com/maps?q=loc:") + "21.0369512" + "," + "105.7805897" + " is broken.";
    // text += "\n\r";
    // text += String("You can confirm the information by responding \"Confirm\" to let us know.");
    // Reply(text, Phone);
    
    while(SerialGPS.available())
    {
      if(gps.encode(SerialGPS.read()))
      {
        newData = true;
        break;
      }
    }
  }
  if(newData)
  {
    String latitude = String(gps.location.lat(),6);
    String longitude = String(gps.location.lng(),6);
    String text = String("The light in area ") + String("http://maps.google.com/maps?q=loc:") + latitude + "," + longitude + " is broken.";
    text += "\n\r";
    text += String("You can confirm the information by responding \"Confirm\" to let us know.");
    Reply(text, Phone);
  }
   
}
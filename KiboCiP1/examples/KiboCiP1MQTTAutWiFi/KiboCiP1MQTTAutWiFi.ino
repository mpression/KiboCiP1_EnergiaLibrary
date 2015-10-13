/*
 Kibo CiP-1 MQTT example 
 */

#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <KiboCiP1.h>

// your network name also called SSID
char ssid[] = "energia1";
// your network password
char password[] = "launch  pad";
// MQTT Broker to use
char broker[] = "example.com";

WiFiClient wifiClient;
PubSubClient client(broker, 1883, callback, wifiClient);

char deviceId[] = "000000000000";
char pubtopic[30] = "Mpression/Kibo/";
char subtopic[40] = "Mpression/Kibo/";

KiboCiP1 kiboCip1;

uint8_t isLed = 0;
uint8_t isSensor = 1;

void callback(char* topic, byte* recmsg, unsigned int length) {
  Serial.print("Received message for topic ");
  Serial.print(topic);
  Serial.print(" with length ");
  Serial.println(length);
  Serial.println("Message:");
  Serial.write(recmsg, length);
  Serial.println();

  char data[30];
  memcpy(data,recmsg,length);

  if( !strcmp(data,"LED")) {
    isLed = 1 - isLed;
    if (isLed) {
      // turn the LED on (HIGH is the voltage level)
      digitalWrite(RED_LED, HIGH);
    }
    else{
      // turn the LED off by making the voltage LOW
      digitalWrite(RED_LED, LOW);
    }
  }

  if( !strcmp(data,"SENSOR")) {
    isSensor = 1 - isSensor;
  }
}

void setup()
{
  uint8_t macOctets[6];

  // initialize the digital pin as an output.
  pinMode(RED_LED, OUTPUT);

  Serial.begin(115200);

  // Start Ethernet with the build in MAC Address
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();

  MACAddress mac = WiFi.macAddress(macOctets);
  Serial.print("MAC Address: ");
  Serial.println(mac);

  // Use MAC Address as deviceId
  sprintf(deviceId, "%02x%02x%02x%02x%02x%02x", macOctets[0], macOctets[1], macOctets[2], macOctets[3], macOctets[4], macOctets[5]);
  Serial.print("deviceId: ");
  Serial.println(deviceId);

  // Use deviceId as subtopic
  Serial.print("subtopic: ");
  strcat(subtopic,deviceId);
  strcat(subtopic,"/toggle");
  Serial.println(subtopic);

  // Use deviceId as pubtopic
  Serial.print("pubtopic: ");
  strcat(pubtopic,deviceId);
  Serial.println(pubtopic);

  // MQTT Broker to use
  Serial.print("MQTT Broker: ");
  Serial.println(broker);

  // Kibo CiP-1
  kiboCip1.begin();
}

void loop()
{ 
  char payload[128];

  // Reconnect if the connection was lost
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");

    if( !client.connect(deviceId)) {
      Serial.println("Connection failed");
    } 
    else {
      Serial.println("Connection success");
      if(client.subscribe(subtopic)) {
        Serial.println("Subscription successfull");
      }
    }
  }

  if (isSensor) {

    // turn the LED on (HIGH is the voltage level)
    digitalWrite(RED_LED, HIGH);

    // get sensor data
    String json = buildJson();
    json.toCharArray(payload,128);

    Serial.print("pubtopic: ");
    Serial.println(pubtopic);
    Serial.print("payload: ");
    Serial.println(payload);

    // Publish
    if(client.publish(pubtopic,payload)) {
      Serial.println("Publish success");
    } 
    else {
      Serial.println("Publish failed");
    }

    // turn the LED off by making the voltage LOW
    digitalWrite(RED_LED, LOW);

  }
  // Check if any message were received
  // on the topic we subsrcived to
  client.poll();
  delay(1000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

String buildJson() {
  char workstr[10];
  double sensortData;

  String json = "{";

  // ldc1612_inductive_ch0
  sensortData = kiboCip1.getLdc1612InductiveCh0();
  Serial.print("\tLDC1612_inductive_ch0\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,3, workstr);
  json += "\"inductive_0\":";
  json += workstr;
  json += ",";

  // ldc1612_inductive_ch1
  sensortData = kiboCip1.getLdc1612InductiveCh1();
  Serial.print("\tLDC1612_inductive_ch1\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,3, workstr);
  json += "\"inductive_1\":";
  json += workstr;
  json += ",";

  // hdc1000_temperature
  sensortData = kiboCip1.getHdc1000Temperature();
  Serial.print("\tHDC1000_temperature\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"temp\":";
  json += workstr;
  json += ",";

  // hdc1000_humidity
  sensortData = kiboCip1.getHdc1000Humidity();
  Serial.print("\tHDC1000_humidity\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"humidity\":";
  json += workstr;
  json += ",";

  // opt3000_ambientLight
  sensortData = kiboCip1.getOpt3001AmbientLight();
  Serial.print("\tOPT3001_ambientLight\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"light\":";
  json += workstr;
  json += ",";

  // tmp007_irTemperature
  sensortData = kiboCip1.getTmp007IrTemperature();
  Serial.print("\tTMP007_irTemperature\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"irTemp\":";
  json += workstr;
  json += "}";

  return json;
}

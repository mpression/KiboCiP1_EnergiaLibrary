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
char pubtopic[30] = "Macnica/Mpression/Kibo/";

KiboCiP1 kiboCip1;

void callback(char* topic, byte* payload, unsigned int length) {
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
  char payload[255];

  // Reconnect if the connection was lost
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");

    if( !client.connect(deviceId)) {
      Serial.println("Connection failed");
    } 
    else {
      Serial.println("Connection success");
    }
  }

  // turn the LED on (HIGH is the voltage level)
  digitalWrite(RED_LED, HIGH);

  // get sensor data
  String json = buildJson();
  json.toCharArray(payload,255);

  // Publish
  if(client.publish(pubtopic,payload)) {
    Serial.println("Publish success");
  } 
  else {
    Serial.println("Publish failed");
  }

  // turn the LED off by making the voltage LOW
  digitalWrite(RED_LED, LOW);

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
  char workstr[255];
  double sensortData;

  String json = "{";

  // ldc1612_inductive_ch0
  sensortData = kiboCip1.getLdc1612InductiveCh0();
  Serial.print("\tldc1612_inductive_ch0\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,3, workstr);
  json += "\"ldc1612_inductive_ch0\":";
  json += workstr;
  json += ",";

  // ldc1612_inductive_ch1
  sensortData = kiboCip1.getLdc1612InductiveCh1();
  Serial.print("\tldc1612_inductive_ch1\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,3, workstr);
  json += "\"ldc1612_inductive_ch1\":";
  json += workstr;
  json += ",";

  // hdc1000_temperature
  sensortData = kiboCip1.getHdc1000Temperature();
  Serial.print("\thdc1000_temperature\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"hdc1000_temperature\":";
  json += workstr;
  json += ",";

  // hdc1000_humidity
  sensortData = kiboCip1.getHdc1000Humidity();
  Serial.print("\thdc1000_humidity\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"hdc1000_humidity\":";
  json += workstr;
  json += ",";

  // opt3000_ambientLight
  sensortData = kiboCip1.getOpt3001AmbientLight();
  Serial.print("\topt3000_ambientLight\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"opt3000_ambientLight\":";
  json += workstr;
  json += ",";

  // tmp007_irTemperature
  sensortData = kiboCip1.getTmp007IrTemperature();
  Serial.print("\ttmp007_irTemperature\t: ");
  Serial.println(sensortData);
  dtostrf(sensortData,1,1, workstr);
  json += "\"tmp007_irTemperature\":";
  json += workstr;
  json += "}";

  return json;
}

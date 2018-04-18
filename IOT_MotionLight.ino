#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>


#define MQTT_BASE   "MOTION/EXTERIOR/"
#define DEVICE_NAME   "DRIVEWAY"
int lastread = 0;
int inPin = A0; 
char MQTT_TOPIC[80];
char MQTT_LASTWILL_TOPIC[80];
String mqttTOPIC = String(MQTT_BASE) + String(DEVICE_NAME);
String mqttLASTWILL_TOPIC = String (MQTT_BASE);

AsyncMqttClient mqttClient;
void onMqttConnect(bool sessionPresent) {
  Serial.println("** Connected to the broker **");
//use if you want to send to esp device
}
void DoCheck()
{
  mqttTOPIC.toCharArray(MQTT_TOPIC,80);

  int light = analogRead(inPin);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
  //Serial.print("Raw reading: ");
  //Serial.println(raw);


  if (light < 400 ) {
    if (lastread != 1){
      lastread = 1;
      mqttClient.publish(MQTT_TOPIC, 1, false, "1");
      Serial.print (light);
    }

    }
  else if (lastread != 0){
    lastread = 0;
    mqttClient.publish(MQTT_TOPIC, 1, false, "0");
    }
  }
 

void setup() {
  mqttLASTWILL_TOPIC.toCharArray(MQTT_LASTWILL_TOPIC,80);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin("<SSID>", "<Password>");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" OK");
  mqttClient.setServer(IPAddress(192, 168, 0, 200), 1883);
  mqttClient.setKeepAlive(5).setCleanSession(false).setWill(MQTT_LASTWILL_TOPIC, 1, true, "something has gone wrong").setCredentials("username", "password").setClientId(DEVICE_NAME);
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void loop() {
      delay(500);
    DoCheck();
}

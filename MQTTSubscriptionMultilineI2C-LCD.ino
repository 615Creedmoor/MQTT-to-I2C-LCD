#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

// WiFi settings
const char *ssid = "<SSID>";             // Replace with your WiFi name
const char *password = "<PASSWORD>";   // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "192.168.1.1";  
const char *mqtt_topic_line1 = "serial/line1";     // MQTT topic for Line 1
const char *mqtt_topic_line2 = "serial/line2";     // MQTT topic for Line 2
const char *mqtt_topic_line3 = "serial/line3";     // MQTT topic for Line 3
const char *mqtt_topic_line4 = "serial/line4";     // MQTT topic for Line 4
const char *mqtt_username = "test";  // MQTT username for authentication
const char *mqtt_password = "test";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void connectToWiFi();

void connectToMQTTBroker();

void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.clear();         
    lcd.backlight();      // Make sure backlight is on

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic_line1);
            mqtt_client.subscribe(mqtt_topic_line2);
            mqtt_client.subscribe(mqtt_topic_line3);
            mqtt_client.subscribe(mqtt_topic_line4);
            // Publish message upon successful connection
            // mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    //lcd.clear();
    
  if (strcmp(topic,mqtt_topic_line1)==0){
      lcd.setCursor(0,0);
      lcd.print("                    ");
      for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        lcd.setCursor(i,0);
        lcd.print((char) payload[i]);
    }

  }

  if (strcmp(topic,mqtt_topic_line2)==0) {
      lcd.setCursor(0,1);
      lcd.print("                    ");
       for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        lcd.setCursor(i,1);
        lcd.print((char) payload[i]);
    }
  }

  if (strcmp(topic,mqtt_topic_line3)==0) {
      lcd.setCursor(0,2);
      lcd.print("                    ");
      for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        lcd.setCursor(i,2);
        lcd.print((char) payload[i]);
    }

  }  

  if (strcmp(topic,mqtt_topic_line4)==0) {
      lcd.setCursor(0,3);
      lcd.print("                    ");
      for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        lcd.setCursor(i,3);
        lcd.print((char) payload[i]);
    }

  }  
    
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
}

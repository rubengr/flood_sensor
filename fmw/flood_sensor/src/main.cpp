#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#ifdef ARDUINO
  #include <Arduino.h>
#endif


void ledBlink();
void floodSensor();
void app();

const char* ssid = "ASUS";
const char* password = "Salvador";
const int ledPin =  D0;// the number of the LED pin

int ledState = HIGH;             // ledState used to set the LED
int sensorValue = 0;  // value read from the sensor
bool flood_sensor = false; //flood sensor state
unsigned long previousLedMillis = 0, previousStateMillis = 0;        // will store last time LED was updated


const long ledBlinkTime = 1000;           // interval at which to blink (milliseconds)
const long readIdleTime = 1000;

enum e_states{
      e_idle,
      e_read_flood_sensor,
      e_read_temp,
      e_error
}state;

e_states current_state = e_idle;
e_states next_state = e_idle;

WiFiClient client;
//WiFiClientSecure sclient;

void setup() {
  // put your setup code here, to run once:
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Flood Sensor");
}

void loop() {
  // put your main code here, to run repeatedly:
  ledBlink();
  app();
  
}

void app(){
    unsigned long currentMillis = millis();

    switch (current_state)
    {
      case e_idle:
          if (currentMillis - previousStateMillis >= readIdleTime) {
              previousStateMillis = currentMillis;
              next_state = e_read_flood_sensor;
          }
          break;
      case e_read_flood_sensor:
          floodSensor();
          next_state = e_idle;
          break;
      default:
          break;
    }
    current_state = next_state;

    
}

void ledBlink()
{
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousLedMillis >= ledBlinkTime) {
    // save the last time you blinked the LED
    previousLedMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void floodSensor()
{
    // read the analog in value
    sensorValue = analogRead(A0);

    // print the readings in the Serial Monitor
    Serial.print("sensor = ");
    Serial.print(sensorValue);
    Serial.print("\n");

    if(sensorValue >= 200)
    {
      //Alarm
      flood_sensor = true;
    }
    else
    {
      flood_sensor = false;
    }
}
#include <Arduino.h>


void ledBlink();
void floodSensor();
void app();

// constants won't change. Used here to set a pin number:
const int ledPin =  D0;// the number of the LED pin

// Variables will change:
int ledState = HIGH;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

int sensorValue = 0;  // value read from the pot

bool flood_sensor = false; //flood sensor state

enum e_states{
      e_idle,
      e_read_flood_sensor,
      e_read_temp,
      e_error
}state;



void setup() {
  // put your setup code here, to run once:
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

    switch (state)
    {
      case e_idle:
          break;
      case e_read_flood_sensor:
          floodSensor();
          break;
      default:
          break;
    }

    
}

void ledBlink()
{
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

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
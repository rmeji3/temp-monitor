#include <SoftwareSerial.h>
#include <DHT11.h>

SoftwareSerial receiver1(10, 11); // RX, TX for Receiver 1
SoftwareSerial receiver2(8, 9);  // RX, TX for Receiver 2
const int increaseThresholdButton = 2;
const int decreaseThresholdButton = 3;
int tempThreshold = 25; // Default temperature threshold in Celsius
unsigned long previousMillis = 0;
const long interval = 5000;
DHT11 dht11(13); // TODO: Change to the correct pin
void setup() {
  receiver1.begin(9600);
  receiver2.begin(9600);
  Serial.begin(9600); // Debugging via Serial Monitor
  pinMode(increaseThresholdButton, INPUT);
  pinMode(decreaseThresholdButton, INPUT);
}

void loop() {
  int temperature = 0;
  int humidity = 0;
  unsigned long currentMillis = millis();
  
  // Read temperature and humidity
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  // Check if Button 1 is pressed
  // change temp threshold 
  if (digitalRead(increaseThresholdButton) == HIGH) {
    if (tempThreshold < 100){
      tempThreshold = tempThreshold + 1;
      receiver1.println(temperature);
      receiver1.println(tempThreshold);
      receiver2.println(temperature);
      receiver2.println(tempThreshold);
    }
  }

  // Check if Button 2 is pressed
  // change temp threshold
  if (digitalRead(decreaseThresholdButton) == HIGH) {
    if (tempThreshold > 0){
      tempThreshold = tempThreshold - 1;
      receiver1.println(temperature);
      receiver1.println(tempThreshold);
      receiver2.println(temperature);
      receiver2.println(tempThreshold);
    }
  }
  if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }

  // every 5 seconds, send the temperature threshold to Receiver 1
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    receiver1.println(temperature);
    receiver1.println(tempThreshold);
    receiver2.println(temperature);
    receiver2.println(tempThreshold);
    Serial.print("Sent to Receiver 1: Temperature: ");
    Serial.println(temperature);
    Serial.print("Sent to Receiver 1: Max: ");
  }
}

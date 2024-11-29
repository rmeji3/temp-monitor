#include <SoftwareSerial.h>

SoftwareSerial sender(10, 11); // RX, TX from Sender Arduino

String message = "";
String thresholdMessage = "";
int threshold = 25;
int temp = 0;
const int buzzerPin = 13;
const int ledPin = 7;

const int input2 = 2;
const int input1 = 3;
const int enablePin = 5;

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    sender.begin(9600);
    pinMode(input1, OUTPUT);
    pinMode(input2, OUTPUT);
    pinMode(enablePin, OUTPUT);
    analogWrite(enablePin, 150);
}

void loop() {
    if (sender.available() > 0) {
        message = sender.readStringUntil('\n'); // Read the message
        thresholdMessage = sender.readStringUntil('\n');
        threshold = thresholdMessage.toInt();
        temp = message.toInt();
        if (temp > threshold) {
            do {
                digitalWrite(ledPin, HIGH);
                //tone(buzzerPin, 1000, 1000);
                analogWrite(enablePin, 150);
                digitalWrite(input1, HIGH);
                digitalWrite(input2, LOW);
            } while (sender.available() == 0);
        }
        analogWrite(enablePin, 0);
        digitalWrite(input1, LOW);
        digitalWrite(input2, HIGH);
        digitalWrite(ledPin, LOW);
    }
}


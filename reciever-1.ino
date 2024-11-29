#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// Define RX and TX for the receiver
SoftwareSerial sender(10, 11); // RX, TX from Sender Arduino
const int rs = 12, en = 13, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int buttonOne = 7;
bool farenheitMode = false;
double temperature;
int finalTemp;
String message = "";
String thresholdMessage = "";

// Debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce time

void setup() {
  sender.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2); // Initialize a 16x2 LCD
  lcd.print("Waiting...");
  pinMode(buttonOne, INPUT);
}


void convertTemp(String message){
    lcd.setCursor(0, 0);
    //celsius
    temperature = message.toInt();
    String finalMessage;
    if(!farenheitMode){
        finalMessage = "Cur Temp: " + String((int)temperature) + " C";
        lcd.print(finalMessage); // Display on LCD
    //farenheit
    }else{
        finalTemp = (temperature * 9/5) + 32;
        finalMessage = "Cur Temp: " + String(finalTemp) + " F";
        lcd.print(finalMessage); // Display on LCD
    }
    
}

int convertMaxTemp(String message){
    int t = message.toInt();
    if(farenheitMode){
        double temp = (t * 9/5) + 32;
        return (int)temp;
    }
    return t;
}
void setMaxTemp(String thresholdMessage){
    lcd.setCursor(0, 1);
    lcd.print("Max Temp: ");
    lcd.print(convertMaxTemp(thresholdMessage));
    if(farenheitMode){
        lcd.print(" F");
    }else{
        
        lcd.print(" C");
    }

}
void loop() {

    int buttonState = digitalRead(buttonOne);
    if (buttonState == HIGH) {
        unsigned long currentTime = millis();

        // Check if enough time has passed since the last press
        if (currentTime - lastDebounceTime > debounceDelay) {
        farenheitMode = !farenheitMode; // Toggle mode
        lastDebounceTime = currentTime; // Update debounce time

            if (message.length() != 0) {
                lcd.clear();
                convertTemp(message);
                setMaxTemp(thresholdMessage);
            }
        }
    }

    if (sender.available()) {
        message = sender.readStringUntil('\n'); // Read the message
        thresholdMessage = sender.readStringUntil('\n');
        message.trim(); // Remove extra spaces, newlines, etc.
        thresholdMessage.trim();
        lcd.clear();
        
        convertTemp(message);
        setMaxTemp(thresholdMessage);
    
    }

}

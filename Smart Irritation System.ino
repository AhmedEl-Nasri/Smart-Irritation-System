#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <SoftwareSerial.h>

#define pump 13 
#define pump2 12 
#define fan 11
#define fan2 10
#define flame 5
#define buz 6
#define Led 7
#define pbpump 2
#define pbfan 1
#define dht A0
#define lm A1

// SIM800L TX and RX pins
//#define SIM800_TX 10
//#define SIM800_RX 11

LiquidCrystal_I2C lcd(0x27, 16, 2);

// software serial for SIM800L
//SoftwareSerial sim800(SIM800_TX, SIM800_RX);

void setup() {
  pinMode(pump, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(flame, INPUT);
  pinMode(pbpump, INPUT);
  pinMode(pbfan, INPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  //lcd.setCursor(0, 0);
  //lcd.print("smart farm");
 // delay(500);
  lcd.clear();

  // Initialize SIM800L
  //sim800.begin(9600); // Start communication with SIM800L at 9600 baud rate
  delay(1000);
}

void loop() {
  // Read sensor values
  int h = analogRead(dht);
    h= map(h,0,1023,0,100);
  float volt = analogRead(lm) * 5.0 / 1023.0; 
    byte t = (volt / 0.01);
  bool flameVal = digitalRead(flame);

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hdt: ");
  lcd.print(h);
  lcd.setCursor(8, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.setCursor(8, 1);
  lcd.print("C");

  // Automatic control for pump based on humidity
 if (h < 40) { 
  digitalWrite(pump, HIGH);
  digitalWrite(pump2, LOW);

 //lcd.setCursor(7, 0);
  //lcd.print("Pump1: ON");
} else {
  digitalWrite(pump, LOW);
  digitalWrite(pump2, LOW);
 // lcd.setCursor(7, 0);
  //lcd.print("Pump1: OFF");
}

  // Automatic control for fan based on temperature
  if (t >= 30) { 
    digitalWrite(fan, HIGH);
    digitalWrite(fan2, LOW);
    //lcd.setCursor(7, 1);
    //lcd.print("Fan: ON");
  } else {
    digitalWrite(fan, LOW);
    digitalWrite(fan2, LOW);
    //lcd.setCursor(7, 1);
   // lcd.print("Fan: OFF");
  }

  // Manual control for pump
 if(digitalRead(pbpump) == HIGH){
    digitalWrite(pump, HIGH);
    //lcd.setCursor(7, 0);
    //lcd.print("Pump: ON");
  }

  // Manual control for fan
  if(digitalRead(pbfan) == HIGH){
    digitalWrite(fan, HIGH);
    lcd.setCursor(7, 1);
    //lcd.print("Fan: ON");
  }
  // Check for fire detection
  if (flameVal == HIGH) {
    // Activate buzzer and LED
    digitalWrite(buz, HIGH);
    digitalWrite(fan, LOW);
    digitalWrite(Led, HIGH);

    // Display fire alert on LCD
    lcd.clear();
   for (int pos = 0; pos - 1 < 16; pos++) {
    lcd.clear();
    lcd.setCursor(0 + pos, 0);
    lcd.print("FIRE");
    lcd.setCursor(7 - pos, 1);
    lcd.print("FIRE");
    delay(20);
  }
    // Call the user using SIM800L
    //sim800.println("01022140420"); // Replace with the phone number to call
    delay(500); // Wait for  the call to go through
    //sim800.println("ATH"); // Hang up the call
  } else {
    // Deactivate buzzer and LED if no fire is detected
    digitalWrite(buz, LOW);
    digitalWrite(Led, LOW);
  }

  delay(1000); // Wait before the next loop iteration
}
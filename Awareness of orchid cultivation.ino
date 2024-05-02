#include <Servo.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 4       // Define the pin for DHT sensor
#define DHTTYPE DHT11

int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value;
Servo s1;
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor object
int rpin = 5;
int spin = 6;
int value;

void setup() {
  pinMode(2, OUTPUT); // water motor
  Serial.begin(9600);
  dht.begin();
  Serial.println("Reading From the Sensor ...");
  pinMode(rpin, INPUT); // Setting pin 5 as input
  s1.attach(spin); // Attaching servo to pin 6

  lcd.init();  // Change lcd.begin() to lcd.init()

  // Turn on the backlight and print a message.
  lcd.backlight();
}

void loop() {
  output_value = analogRead(sensor_pin);
  output_value = map(output_value, 550, 10, 0, 100); // Mapping sensor value to percentage
  Serial.print("Moisture : ");
  Serial.print(output_value);
  Serial.println("%");
  
  // Checking if moisture level is less than 0 (which is not possible), if so, turn on pin 2 (water motor)
  if (output_value < 0) {
    digitalWrite(2, HIGH); // on water motor
  } else {
    digitalWrite(2, LOW);
  }

  delay(1000);

  float temperature = dht.readTemperature();
  value = digitalRead(rpin);

  if(temperature >= 33.00 || value == LOW){
    s1.write(90); // Servo position for cooling or low moisture
  }
  else {
    s1.write(180); // Servo position for normal conditions
  }
  Serial.print("Temperature (C): ");
  Serial.println(temperature, 2);
  lcd.clear(); // Clear the LCD screen before printing new values
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Temp=(C)");
  lcd.print(temperature);
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("Mos=%");
  lcd.print(output_value);
}
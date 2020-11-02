/*
Sets a servo at 90 degree to calibrate whatever is on the head for 
calibrated sweeping. Responsibility of user to set servo to whatever needs to point at 90 degrees. 

Displays results on Serial Monitor

Arduino/sensor/button/servo with external 4.8V battery) set up:
 Arduino pin    ->  sensor pin (volitage taken from Power pin side of Arduino
 5V Analog side     VCC usonic sensor
 12                 TRIG usonic sensor
 11                 ECHO usonic sensor
 GND Analog side    Btn side A common with usonic GND on breadboard
 2                  Btn side B
 9                  Servo signal Pin 3 for older Airtronics
 
 Servo GND          GND 4.8V Battery, common on breadboard with GND Digital
 Usonic GND         GND Arduino Analog side
 Usonic VCC         5V Arduino Analog
 Servo Pin 1        5V battery (red wire)
 Servo Pin 2        Black wore to battery, and breadboard common with GND Digital
 
 This also inclues a button to trigger a read. EAch time the button is pressed 5 reads are taken.
 The button uses the internal PULLUP resistor related to pin 2
*/
 
# include <Servo.h>
// Hook up HC-SR04 with TRIG to Arduino Pin 12, ECHO to Arduino pin 11
// Maximum Distance is 400 cm (4m)
 
#define SERVO_PIN 9
#define BTN_PIN 2
Servo servo;          //create a servo object
int pos = 90;       //default intial position of servo;

void setup() {
  Serial.begin (9600);
  // Setup Input Pin for button:
  pinMode(BTN_PIN, INPUT_PULLUP);
  servo.attach(SERVO_PIN);
  pos = 90;
  servo.write(pos); // tell servo to go to 'pos'; in this case 90 deg
  delay(15);
}

void loop() {
  //read the pushbutton value into a variable
  int pinVal = digitalRead(BTN_PIN);
  //print out the value of the pushbutton
  //Serial.println(pinVal);
  //Because using internal PULL_UP on btn input pin 2, do something when button pressed, i.e., goes LOW
  if (pinVal == LOW){
    servo.write(pos);
    Serial.print(pos);
    Serial.println(" degree angle set.");
   delay(500); //useconds
    }
}

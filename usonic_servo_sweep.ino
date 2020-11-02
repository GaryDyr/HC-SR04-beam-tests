/*
 * This sketch uses the ultrasonic sensor (HC-SR04), a button, 
 * and an externally (battery) powered servo * to sweep through 
 * an angle in 1 degree increments and then reverses the seep 
 * process to end up back at the starting point. 
 * The process is triggered by clicking the button.
 * At each position the ultrasonic sensor is read 5 times and 
 * the median calculated.
 * The position, the useconds, the distance in cm, and the distance
 * in inches are then printed the serial monitor.
 * The data can be copied via Ctrl-C. for further analysis
 /*

  Displays results on Serial Monitor

 Arduino/sensor/button/servo with external 4.8V battery) set up:
 Arduino pin    ->  sensor pin (volitage taken from Power pin side of Arduino
  5V Analog side     VCC usonic sensor
  12                 TRIG usonic sensor
  11                 ECHO usonic sensor
  GND Analog side    Usonic GND 
 Button setup
  GND Analog side    Btn side A common with usonic GND on breadboard
  2                  Btn side B; NOTE MUST USE INTERNAL PULLUP RESISTOR VIA "INPUT_PULLUP"
  9                  Servo signal Pin 3 for older Airtronics
 Servo Setup
  Servo GND to breadboard with 4.8V Battery GND + to Arduino GND Digital
  Servo Pin 1 to 4.8V battery red wire (+)
  Servo Pin 2 to 4.8V battery black wire (-), + common with GND Digital through breadboard.
  Refereences
  http://dronebotworkshop.com
  https://dronebotworkshop.com/hc-sr04-ultrasonic-distance-sensor-arduino/#1
 This also inclues a button to trigger a read. EAch time the button is pressed 5 reads are taken.
 The button uses the internal PULLUP resistor related to pin 2
*/
 
// This uses Serial Monitor to display Range Finder distance readings
// but the data is also on the usb port.
 
// Include NewPing Library for HC-SR04 Ultrasonic Range Finder
# include <NewPing.h>
# include <Servo.h>
// Hook up HC-SR04 with HC-SR04 TRIG to Arduino Pin 12, ECHO to Arduino pin 11
// Maximum Distance is 400 cm (4m)
 
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 400
#define BTN_PIN 2
#define SERVO_PIN 9
Servo servo;          //create a servo object
//Initialize the sensor pin setup and distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance_cm, distance_inches;
float sound_vel =  0.034445;  //CHANGE THIS (AS CM/USEC) DEPENDING ON TEMP. AND HUMIDITY
int iterations = 5; //times to read ultrasonic sensor for averaging
int pos = 0;        //default intial position of servo in degrees;

void setup() {
  Serial.begin (9600);
  // Setup Input Pin for button:
  pinMode(BTN_PIN, INPUT_PULLUP);
  servo.attach(SERVO_PIN);
  pos = 20;
  servo.write(pos); // tell servo to go to 'pos'; in this case starting at 20 deg
  delay(15); //delays are always in milliseconds
}

//One advantage of using NewPing, it defines a median, based on iterations
//Median gets delay time between pings, based on either MAX_DISTANCE, or a test ping before iterating.
//Have to be careful with the loop delay to accomodate. With 4 m distance echo_time would be 
//~12 ms; 12,000 us
void loop() {
  //read the pushbutton value into a variable
  int btnVal = digitalRead(BTN_PIN);
  //print out the value of the pushbutton
  //Serial.println(pinVal);
  //Because using internal PULL_UP on btn input pin 2, do something when button pressed, i.e., goes LOW
  if (btnVal == LOW){
     for (pos = 20; pos <= 160; pos += 1) { // goes from 20 degrees to 160 degrees in steps of 1 degree
       servo.write(pos);              // tell servo to go to position in variable 'pos'
       delay(15);// waits 15ms for the servo to reach the position
       //ping_median fcn waits 29 ms between pings, so takes about 150 ms to complete.
       // 29 ms is ping and return of 9.9 meters = 32 ft or 1/2 time return of 4.9 m; 16 ft.
       duration = sonar.ping_median(iterations); //fcn must set sonar TRIG pin to HIGH. part of ping(), ping_median fcn.
       //below not needed at this time
       //duration = sonar.ping() return if iterations = 1
       //Determine distance from usec duration
        distance_cm = (duration / 2) * sound_vel;
       // Send results to Serial Monitor
       //Serial.print("Distance = ");
       //Room dimensions forced a sensor to target distance limit of 300 cm
       if (distance_cm >= 300 || distance_cm <= 2) {
         Serial.print(pos);
         Serial.println(",Out of range");
          }
       else {
         distance_inches = distance_cm * 0.393701;
         Serial.print(pos);
         Serial.print(",");
         Serial.print(duration); 
         Serial.print(",");
         Serial.print(distance_cm);
         Serial.print(",");
         Serial.println(distance_inches);
         delay(500); //millseconds
       } //end if
     } // end for
 
    for (pos = 160; pos >= 20; pos -= 1) { // go from 160 degrees to 20 degrees
      servo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(15);                           // waits 15ms for the servo to reach the position
      duration = sonar.ping_median(iterations); //fcn must set sonar TRIG pin to HIGH. part of ping(), ping_median fcn.
      //duration = sonar.ping() return if iterations =1
      // Determine distance from duration
      distance_cm = (duration / 2) * sound_vel;
      // Send results to Serial Monitor
      //Serial.print("Distance = ");
      if (distance_cm >= 300 || distance_cm <= 2) {
        Serial.print(pos);
        Serial.println(",Out of range");
        }
      else {
        distance_inches = distance_cm * 0.393701;
        Serial.print(pos);
        Serial.print(",");
        Serial.print(duration); 
        Serial.print(",");
        Serial.print(distance_cm);
        Serial.print(",");
        Serial.println(distance_inches);
        delay(500); //useconds
        } //end if
    } //end for
  } //end if btn
} // end loop

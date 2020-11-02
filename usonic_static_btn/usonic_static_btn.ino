/*
 * This sketch should be run after the sevo_90_calibration_btn.ino, which
 * sets the servo to 90 degrees, unless using a different servo configuration.
 * This uses the primary example frome author of NewPing to drive sonic sensor.
 * For test decided to use the simple convert_cm function.
 * The set up includes a button to trigger a read. EAch time the button is
 * pressed, 5 reads are taken and averaged, using ping_median().
 * The button uses the internal PULLUP resistor related to pin 2
 
  Arduino/sensor set up
  Arduino pin -> sinsor pin (volitage taken from Power pin side of Arduino)
     5V               VCC
     12               TRIG
     11               ECHO
     GND              GND
*/
 
// This uses Serial Monitor to display Range Finder distance readings
// but the data is also on the usb port.
// Include NewPing Library for HC-SR04 Ultrasonic Range Finder
#include <NewPing.h>
 
// Hook up HC-SR04 with Trig to Arduino Pin 12, Echo to Arduino pin 11
// Maximum Distance is 400 cm (4m)
 
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 400
#define BTN_PIN 2

 //Initialize the sensor pin setup and distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance_cm, distance_inches;
int iterations = 5;

void setup() {
  Serial.begin (9600);
  // Setup Input Pin for button:
  pinMode(BTN_PIN, INPUT_PULLUP);
}

//this is one advantage of using NewPing, it defines a median, based on iterations
//median gets delay time between pings, based on either MAX_DISTANCE, or a test ping before iterating.
//Have to be careful with the loop delay to accomodate. With 4 m distance echo_time would be 
//~12 ms; 12,000 us
void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(BTN_PIN);
  //print out the value of the pushbutton
  //Serial.println(sensorVal);
  //Because using internal PULL_UP on btn input pin 2, do something when button pressed, i.e., goes LOW
  if (sensorVal == LOW){
    duration = sonar.ping_median(iterations); //fcn must set sonar TRIG pin to HIGH. part of ping(), ping_median fcn.
    //duration = sonar.ping() return if iterations =1
    // Determine distance from duration
    // Use 344.45 metres per second as speed of sound; change as needed.
    distance_cm = (duration / 2) * 0.034445;
    // Send results to Serial Monitor
    //Serial.print("Distance = ");
    if (distance_cm >= 400 || distance_cm <= 2) {
      Serial.println("Out of range");
    }
    else {
      distance_inches = distance_cm * 0.393701;
      Serial.print(duration); 
      Serial.print(",");
      Serial.print(distance_cm);
      Serial.print(",");
      Serial.println(distance_inches);
      //Serial.println(" cm");
      delay(500); //useconds
    }
  delay(500);
  }
}

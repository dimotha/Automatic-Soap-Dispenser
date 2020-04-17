/* 
program by dimotha 17/4/2020
controls the SG90 servo motor based on the U/S sensor trigger input
The SG90 is assumed to be mounted on the ASD_base and the U/S sensor
on the ASD_lever. The whole assembly shall be mounted on the soap pump. 
see project instructions here:
https://dimotha.wordpress.com/automatic-soap-dispenser/

This is the dish washing liquid version of the device
upon trigger:
- 3 doses by the soap pump
- LED blinks twice to confirm dosing operation completed

*/

#include <Servo.h>  // SG90 servomotor driving program

#define echoPin 7   // U/S sensor Echo Pin
#define trigPin 8   // U/S sensor Trigger Pin
#define LEDPin 13   // Onboard LED


Servo servo;
int angle = 145;          // servo test
int maximumRange = 200;   // Maximum range needed U/S sensor
int minimumRange = 0;     // Minimum range needed U/S sensor
long duration, distance;  // Duration used to calculate distance -  U/S sensor

void setup() {

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator

  servo.attach(9);
  servo.write(angle);
  Serial.begin(9600);
  delay(2000);
}


void loop() {
  digitalWrite(LEDPin, LOW);  // LED starts OFF
  
  angle=150;                  // servo up angle
  Serial.println(angle);
  servo.write(angle);         // servo/lever up
  delay(2000);  

/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound in air
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol
 to indicate successful reading. */
 Serial.println(distance);

/* when hand is at distance less than 7 cm from the U/S sensor
 * the servomotor will drive 3 pump soap doses. After this sequence 
 * the LED will blink for 2 seconds on a 0.5 sec ON / 0.5 sec OFF pattern. 
 */
if (distance<7) {         // condition trigger for soap dispensing

for(int i=1; i<4; i++){  // initiate soap dispensing
  angle=16;               // servo down angle
  Serial.println(angle);
  servo.write(angle);     // servo/lever down
  delay(1000);

  angle=150;              
  Serial.println(angle); 
  servo.write(angle);     // servo/lever back to up position
  delay(700);
       
}                         //end of soap dispensing

   
  for(int i=1; i<3; i++){      // Start blinking pattern
   digitalWrite(LEDPin, HIGH);  // LED ON
   delay(500);
   digitalWrite(LEDPin, LOW);   // LED OFF
   delay(500);
}
}  
}

}

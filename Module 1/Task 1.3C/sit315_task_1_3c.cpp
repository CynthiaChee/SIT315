//initializing pin values and initial state
int led = 4;                 //led pin
int val = 0;                 //variable for storing sensor status
int trigPin = 11;
int echoPin = 12;
volatile boolean motionDetected = false;

//initializing variables
long duration;
int distance;

void setup() {
  pinMode(led, OUTPUT);      //initialize led as output
  //initializing echo and trig pints
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(9600);        //sets the data rate in bits per second (baud) for serial data transmission

  //attach interrupt to pin 2 (PIR sensor)
  attachInterrupt(digitalPinToInterrupt(2), interruptAction, CHANGE);
}

void loop(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //get duration of sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  //distance = (traveltime/2) * speed of sound
  //speed of sound is 1/29.1 cm/microsecs
  //distance result is in cm
  distance = (duration/2) /29.1;
  
  //if motion detected from PIR sensor or distance in ultrasonic sensor
  //is less than 75cm, activate LED and print relevant message
  if (motionDetected || distance < 75) {
    digitalWrite(led, HIGH);
    
    if (motionDetected) {
      Serial.println("Interrupt!");
      motionDetected = false;
    }
    else if (distance < 75) {
      Serial.print("Close distance! ");
      Serial.print(distance);
      Serial.println(" cm.");
    }
    digitalWrite(led, LOW);
    delay(750);
  }
  //if distance 75cm or more
  else {
    Serial.println("Measuring distance...");
    delay(750);
  }
}

void interruptAction() {
  motionDetected = true;
}

/* References:
Arduino (n.d.), attachInterrupt(), Arduino, Available at <https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/>,
Accessed 16 July 2023

Santos, R. (2013), Complete Guide for Ultrasonic Sensor HC-SR04 with Arduino,
Random Nerd Tutorials, Available at <https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/>,
Accessed 17 July 2023

Reilly, N. (2015), Using Interrupts on Arduino, All About Circuits,
Available at <https://www.allaboutcircuits.com/technical-articles/using-interrupts-on-arduino/>,
Accessed 17 July 2023 */
//initializing pin values and initial state
int led = 4;                 //led pin
int sensor = 2;              //motion sensor pin

//volatile is used for variables that may change unexpectedly
//tells compiler to read current value from memory location
volatile boolean motionDetected = false;

void setup() {
  pinMode(led, OUTPUT);      //initialize led as output
  pinMode(sensor, INPUT);    //initialize motion sensor as input
  Serial.begin(9600);        //sets the data rate in bits per second (baud) for serial data transmission

  //attach interrupt to pin 2 (sensor pin)
  attachInterrupt(digitalPinToInterrupt(2), interruptAction, CHANGE);
}

void loop(){
  //if motion detected, interrupt triggered
  if(motionDetected) {
    digitalWrite(led, HIGH);
    Serial.println("Testing Interrupt");
    delay(800);
    digitalWrite(led, LOW);
    motionDetected = false;
    Serial.println("Reset");
  }
}

void interruptAction() {
  motionDetected = true;
}

/* References

Santos, R. (2014), Arduino with PIR Motion Sensor, Random Nerd Tutorials,
Available at <https://randomnerdtutorials.com/arduino-with-pir-motion-sensor/>,
Accessed 16 July 2023 */
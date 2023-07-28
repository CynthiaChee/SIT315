//Github repo: https://github.com/CynthiaChee/SIT315.git
//Video Demonstration: https://video.deakin.edu.au/media/t/1_rsnnw6pq
//Alternative Link: https://drive.google.com/file/d/11GKU4f5gF7nUBciZyu-L0hrAA3LWOWhk/view?usp=share_link

//initializing pin values and initial state
int led = 4;                 //led pin
int sensor = 2;              //motion sensor pin

void setup() {
  pinMode(led, OUTPUT);      //initialize led as output
  pinMode(sensor, INPUT);    //initialize motion sensor as input
  Serial.begin(9600);        //sets the data rate in bits per second (baud) for serial data transmission

  //attach interrupt to pin 2 (sensor pin)
  attachInterrupt(digitalPinToInterrupt(2), interruptAction, RISING);
}

//ISR for PIR interrupt
void interruptAction() {
  Serial.println("Interrupt from PIR");
  digitalWrite(led, HIGH);
  delay(800);
  digitalWrite(led, LOW);
}

void loop(){
  //No polling required
}

/* References

Santos, R. (2014), Arduino with PIR Motion Sensor, Random Nerd Tutorials,
Available at <https://randomnerdtutorials.com/arduino-with-pir-motion-sensor/>,
Accessed 16 July 2023 */
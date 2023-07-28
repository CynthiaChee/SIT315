//initializing pin values and initial state
int led = 4;                 //led pin
int sensor = 2;              //motion sensor pin
int button = 3;				 //button pin

void setup() {
  pinMode(led, OUTPUT);      //initialize led as output
  pinMode(sensor, INPUT);    //initialize motion sensor as input
  pinMode(button, INPUT_PULLUP); //initialize button as input pullup
  Serial.begin(9600);        //sets the data rate in bits per second (baud) for serial data transmission
  
  //attach interrupt to pin 2 (sensor pin) and pin 3 (button pin)
  attachInterrupt(digitalPinToInterrupt(2), motionInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(3), buttonInterrupt, RISING);
}

//ISR for PIR-triggered interrupt
void motionInterrupt() {
  Serial.println("Interrupt from PIR");
  digitalWrite(led, HIGH);
  delay(800);
  digitalWrite(led, LOW);
  
}

//ISR for button-triggered interrupt
void buttonInterrupt() {
  Serial.println("Interrupt from Button");
  digitalWrite(led, HIGH);
  delay(800);
  digitalWrite(led, LOW);
}

void loop(){
  //No polling required
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
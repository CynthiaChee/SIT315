#define TRIG_PIN 11
#define ECHO_PIN 12
#define PIR 2 
#define BUTTON 5
#define LED 4

//Variables
volatile bool motionDetected = false;
volatile bool buttonPressed = false;
long duration;
int distance;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  //Attach pin to interrupt (for hardware interrupt)
  attachInterrupt(digitalPinToInterrupt(PIR), motionInterrupt, CHANGE);
  
  //Turn on Port D on PCICR register (pin 5 is in port D)
  //Indicates the group of pins for pin change interrupt
  PCICR |= B00000100;
  
  //Set Pin Change Mask registers to the relevant bit (for D5)
  //Indicates that D5 is the pin that triggers the interrupt
  PCMSK2 |= B00100000;
  
  //Setup for timer interrupt
  setupTimer();
}

void loop() {
  
  //This portion of the program runs (ultrasonic sensor) unless interrupted
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  //get duration of sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
  //distance = (traveltime/2) * speed of sound
  //speed of sound is 1/29.1 cm/microsecs
  //distance result is in cm
  distance = (duration/2) /29.1;
  
  if (motionDetected || buttonPressed || distance < 75) {
    // Turn on the LED
    digitalWrite(LED, HIGH); 
    
    // turn on the LED for the button
    if(buttonPressed) {
      // Output to console
      Serial.println("Pin Change Interrupt (Button Click)"); 
      // Reset the button press flag
      buttonPressed = false; 
    }
    
    if(motionDetected) {
      Serial.println("Hardware interrupt (PIR)"); 
      // Reset the motion detection flag
      motionDetected = false; 
    }
    else if (distance < 75) {
      Serial.print("Close distance! ");
      Serial.print(distance);
      Serial.println(" cm.");
    }
    // Delay for 1 second
    delay(500); 
    // Turn off the LED
    digitalWrite(LED, LOW);
    //reset the button press flag
    buttonPressed = false;  
  }
    //if distance 75cm or more
  else {
    Serial.println("Measuring distance...");
    delay(750);
  }
}

//Executed when PIR triggered
void motionInterrupt() {
  motionDetected = true;  
}

//Interrupt Service Routine - executes when interruption triggered
ISR(PCINT2_vect) {
  if (digitalRead(BUTTON) == LOW) {
    buttonPressed = true;
  }
}

void setupTimer(){
  //Stops interrupts until settings done
  cli();
  
  //Calculate compare match value 
  //Clock (Hz)/(Prescaler * Desired Frequency)-1
  //In this case, timer interrupt occurs every 2 seconds
  uint16_t target = (16000000 / 1024 * 2) - 1;
  
  //Reset prescalar registers and reset Timer 1 value to 0
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  //Set compare register A to target value
  OCR1A = target;
  
  // Set prescalar 1024
  TCCR1B |= (1 << CS12 | 1 << CS10);
  
  // Enable output compare A match interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  // Set Timer 1 to CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);
  
  //Reenable interrupts
  sei();
}

// Timer interupt function (Compare match)
ISR(TIMER1_COMPA_vect){
  digitalWrite(LED, HIGH);
  delay(500); 
  digitalWrite(LED, LOW);
  Serial.println("Timer Interrupt");
}

/* References:
ElectroNoobs (2020), Pin Change Interruptions, ElectroNoobs, Available at <https://electronoobs.com/eng_arduino_tut132.php>,
Accessed 18 July 2023

ElectroNoobs (2021), Timer Interruptions, ElectroNoobs, Available at <https://electronoobs.com/eng_arduino_tut140.php>,
Accessed 18 July 2023

DroneBot Workshop (n.d.), Using Arduino Interrupts - Hardware, Pin Change and Timer, DroneBot Workshop, Available at <https://dronebotworkshop.com/interrupts/>,
Accessed 18 July 2023

mwwalk (2014), Arduino Pin Change Interrupts, The Wandering Engineer, Available at <https://thewanderingengineer.com/2014/08/11/arduino-pin-change-interrupts/>,
Accessed 18 July 2023 */
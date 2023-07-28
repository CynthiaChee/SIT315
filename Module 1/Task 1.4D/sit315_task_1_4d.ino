int pir = 2;
int button = 5;
int led = 4;
int prevButtonState = LOW;

void setup() {
  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  //Attach pin to interrupt (for hardware interrupt)
  attachInterrupt(digitalPinToInterrupt(pir), motionInterrupt, RISING);
  
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
	//No polling required
}

//Executed when PIR triggered
void motionInterrupt() {
  Serial.println("Interrupt from PIR (Hardware Interrupt)");
  digitalWrite(led, HIGH);
  delay(800);
  digitalWrite(led, LOW);
}

//Interrupt Service Routine - executes when interruption triggered
ISR(PCINT2_vect) {
  //read current button state
  int currButtonState = digitalRead(5);
  
  //if rising change detected, light up LED and print serial message
  if (currButtonState == HIGH && prevButtonState == LOW) {
    Serial.println("Interrupt from Button (Pin Change Interrupt)");
  	digitalWrite(led, HIGH);
  	delay(800);
  	digitalWrite(led, LOW);
  }
  
  //update button state
  prevButtonState = currButtonState;
}

void setupTimer(){
  //Stops interrupts until settings done
  cli();
  
  //Calculate compare match value 
  //Clock (Hz)/(Prescaler * Desired Frequency)-1
  //In this case, timer interrupt occurs every 2 seconds
  //uint16_t target = (16000000 / 0.5 /1024) - 1 = 31250
  uint16_t target = 31250;
  
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
  digitalWrite(led, HIGH);
  delay(800); 
  digitalWrite(led, LOW);
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
//initializing pin values and initial state
int led = 4;                 //led pin
int sensor = 2;              //motion sensor pin
int state = LOW;             //initial state (no motion)
int val = 0;                 //variable for storing sensor status

void setup() {
  pinMode(led, OUTPUT);      //initialize led as output
  pinMode(sensor, INPUT);    //initialize motion sensor as input
  Serial.begin(9600);        //sets the data rate in bits per second (baud) for serial data transmission
}

void loop(){
  val = digitalRead(sensor);   //read value from sensor
  if (val == HIGH) {           //turn led on if motion detected
    digitalWrite(led, HIGH);   
    delay(500);                
    
    //update state and print message to Serial Monitor
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       
    }
  } 
  else {
      digitalWrite(led, LOW); //else, turn led off and update state accordingly
      delay(500);             
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;   
    }
  }
}
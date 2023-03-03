/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
char incomingByte = ' ';
char estado = '2'; // 1 encendido, 2 apagado

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  Serial.begin(9600);  
}

// the loop routine runs over and over again forever:
void loop() {
  incomingByte = ' ';
  if (Serial.available() > 0) {
    incomingByte = char(Serial.read());    
  }
  if (incomingByte == '1') {
    digitalWrite(led, HIGH);
    estado = incomingByte;
  }
  if (incomingByte == '2') {
    digitalWrite(led, LOW); 
    estado = incomingByte;
  }
  if (incomingByte == '3') {
    Serial.write(estado); 
    Serial.flush();
  }
}

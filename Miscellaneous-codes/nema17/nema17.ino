// Notes:
//1. delay(20) works smoothly with 0.18+ voltage
//1. delay(10) works smoothly with 0.22+ voltage (currently exact 0.23)
//1. delay(7) works smoothly with 0.32+ voltage  (currently exact 0.34)
//1. delay(5) works smoothly with 0.20+ voltage (strangely!!)//
//1. delay(2) works smoothly with 0.41+ voltage (currently exact 0.42)
//1. delay(1) works smoothly with 0.18+ voltage (strangely @ 0.18)

// defines pins numbers
const int stepPin = 9; 
const int dirPin = 8; 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
}

void rotate(int degrees){
    
    int pulses = (5 * degrees)/9;
    for(int x = 0; x < 50; x++) {
        
        digitalWrite(stepPin,HIGH); 
        delay(10);
        digitalWrite(stepPin,LOW);
        delay(10);
  }
  
}


//void OFFtoON(long minutes){
//   digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
//  // Makes 200 pulses for making one full cycle rotation
//  
//  delay(1000); // One second delay
//}
//void ONtoOFF(long minutes){
//    
//}
//
//void ONtoSIM(long minutes){
//    
//}
//
//void SIMtoON(long minutes){
//       
//}
//
//void OFFtoSIM(long minutes){
//    
//}
//
//void OFFtoDIAG(long minutes){
//       
//}
//
//void OFFtoDIAG(long minutes){
//       
//}

void loop() {
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delay(10);
//    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
//    delayMicroseconds(500);
    delay(10);
  }
  delay(1000); // One second delay
  
//  digitalWrite(dirPin,LOW); //Changes the rotations direction
//  // Makes 400 pulses for making two full cycle rotation
//  for(int x = 0; x < 200; x++) {
//    digitalWrite(stepPin,HIGH);
////    delayMicroseconds(500);
//    delay(1);
//    digitalWrite(stepPin,LOW);
////    delayMicroseconds(500);
//    delay(1);
//  }
//  delay(1000);
}

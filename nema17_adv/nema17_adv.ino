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

int positions[] = {0, 45, 90, 135, 180};
String modes[] = {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};
//enum modes {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};

long previousMillis = 0;        //
 
void setup() {
    
    // Sets the two pins as Outputs
    Serial.begin(9600);
    pinMode(stepPin, OUTPUT); 
    pinMode(dirPin, OUTPUT);
}

void compareAndRotate(int prevIndex, String next){
    int dir;
    for(int i = 0; i < 5; i++){
        if(modes[i].equalsIgnoreCase(next)){
            dir = i > prevIndex? 1 : -1;
            rotate(abs(i-prevIndex)*45, dir);
            break;
        }
    }
}
int setDirAndRotate(String prev, String next){
//int setDirAndRotate(St prev, int next){

//    switch(prev){
//        case "OFF":
//            compareAndRotate(0, next);
//            break;
//        case "ON-OFF":
//            compareAndRotate(1, next);
////            for(int i = 0; i < 5; i++){
////                if(modes[i].equalsIgnoreCase(next){
////                    dir = i > 1? -1 : 1;
////                    rotate(i*45, dir);
////                    break;
////                }
////            }
//            break;
//        case "ON":
//            compareAndRotate(2, next);
//            break;
//        case "ON-SIM":
//            compareAndRotate(3, next);
//            break;
//        case "SIM":
//            compareAndRotate(4, next);
//            break;
//    }

    if(prev.equalsIgnoreCase("OFF")){
        compareAndRotate(0, next);
    } else if(prev.equalsIgnoreCase("ON-OFF")){
        compareAndRotate(1, next);
    } else if(prev.equalsIgnoreCase("ON")){
        compareAndRotate(2, next);
    } else if(prev.equalsIgnoreCase("ON-SIM")){
        compareAndRotate(3, next);
    } else if(prev.equalsIgnoreCase("SIM")){
        compareAndRotate(4, next);
    }
}

void rotate(int deg, int dir){
    
    int pulses = (5 * deg)/9;
    if(pulses == 0){
        return;
    }
    if(dir == 1){
        digitalWrite(dirPin, HIGH);
    } else if(dir == -1){
        digitalWrite(dirPin, LOW);
    }
    Serial.println(pulses);
    Serial.println(deg);
    Serial.println(dir);
    for(int x = 0; x < pulses; x++) {
        
        digitalWrite(stepPin,HIGH); 
        delay(10);
        digitalWrite(stepPin,LOW);
        delay(10);
  }
  
}


void OFFtoON(long minutes){
   digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  
  delay(1000); // One second delay
}
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

    unsigned long currentMillis = millis();

    String prev = "OFF";
    String next = "ON";

//    mode prev = OFF;

    
    setDirAndRotate(prev, next);
//    rotate(90, 1); 
    delay(1000);

    prev = "ON";
    next = "SIM";

    setDirAndRotate(prev, next);
//    rotate(90, 1);
    delay(2*1000);
//
    prev = "SIM";
    next = "OFF";
    setDirAndRotate(prev, next);
//    rotate(180,-1);
    delay(5*1000);

    unsigned long diff = currentMillis - previousMillis;
    if(diff > 3000 && diff < 4000){
        
        prev = "SIM";
        next = "OFF";
        setDirAndRotate(prev, next);    
    } else if(diff > 1000){
        
        prev = "ON";
        next = "SIM";
        setDirAndRotate(prev, next);
    } else {
        
        prev = "OFF";
        next = "ON";
        setDirAndRotate(prev, next);
    }



//    digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
//    // Makes 200 pulses for making one full cycle rotation
//    for(int x = 0; x < 200; x++) {
//    digitalWrite(stepPin,HIGH); 
//    delay(1);
//    //    delayMicroseconds(500);
//    digitalWrite(stepPin,LOW);
//    //    delayMicroseconds(500);
//    delay(1);
//    }
//delay(1000); // One second delay
//  
//  digitalWrite(dirPin,LOW); //Changes the rotations direction
//  // Makes 400 pulses for making two full cycle rotation
//  for(int x = 0; x < 400; x++) {
//    digitalWrite(stepPin,HIGH);
////    delayMicroseconds(500);
//    delay(1);
//    digitalWrite(stepPin,LOW);
////    delayMicroseconds(500);
//    delay(1);
//  }
//  delay(1000);
}

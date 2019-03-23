// Notes:
//1. delay(20) works smoothly with 0.18+ voltage
//1. delay(10) works smoothly with 0.22+ voltage (currently exact 0.23)
//1. delay(7) works smoothly with 0.32+ voltage  (currently exact 0.34)
//1. delay(5) works smoothly with 0.20+ voltage (strangely!!)//
//1. delay(2) works smoothly with 0.41+ voltage (currently exact 0.42)
//1. delay(1) works smoothly with 0.18+ voltage (strangely @ 0.18)

// defines pins numbers
const int stepPin = D8; 
const int dirPin = D7; 

int positions[] = {0, 45, 90, 135, 180};
String modes[] = {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};

long previousMillis = 0;        //
String prev, next;
boolean start = true;
 
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
//            dir = i > prevIndex? 1 : -1;
            dir = i > prevIndex? -1 : 1;
            rotate(abs(i-prevIndex)*45, dir);
            break;
        }
    }
}
int setDirAndRotate(String prev, String next){

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

    for(int x = 0; x < pulses; x++) {
        
        digitalWrite(stepPin,HIGH); 
        delay(10);
        digitalWrite(stepPin,LOW);
        delay(10);
  }
  
}



void loop() {

    unsigned long currentMillis = millis();
    unsigned long diff = currentMillis - previousMillis;

    

    if(start == true){
        
        Serial.println(3);
        prev = "OFF";
        next = "ON";
        setDirAndRotate(prev, next); 
        start = false;
        prev = "ON";
        
        previousMillis = millis();
        
    } else if(diff > 2000 && prev.equalsIgnoreCase("SIM")){
        
        Serial.println(1);
        next = "OFF";
        setDirAndRotate(prev, next); 
        prev = "OFF";
        
        previousMillis = millis();
        
    } else if(diff > 1000 && prev.equalsIgnoreCase("ON")){
        
        Serial.println(2);
        next = "SIM";
        setDirAndRotate(prev, next);
        prev = "SIM";
        
        previousMillis = millis();
    } 



}

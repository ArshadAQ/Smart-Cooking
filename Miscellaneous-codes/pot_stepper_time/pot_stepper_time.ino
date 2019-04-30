int potPin = A0;    // select the input pin for the potentiometer

int val = 0;       // variable to store the value coming from the sensor

int deg = 0;       // variable to store the value coming from the sensor
int prevDeg = 0;

int pos = 0;

// defines pins numbers
const int stepPin = D8; 
const int dirPin = D7; 

String prev = "OFF";
String next;

int positions[] = {0, 45, 90, 135, 180};
String modes[] = {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};

long previousMillis = 0;
boolean start = true;
    
void setup() {
    Serial.begin(9600);
    pinMode(stepPin, OUTPUT); 
    pinMode(dirPin, OUTPUT);
}

int getDegree(){
    val = analogRead(potPin);    // read the value from the sensor
    Serial.print("Raw value: ");
    Serial.println(val);
//    deg = map(val, 150, 820, 180, 0);
    deg = map(val, 136, 840, 180, 0);

    if(deg < 0){
        deg = 0;
    } else if(deg > 180){
        deg = 180;
    }

    return deg;
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

int onTIme = 15000;
int simTime = 2000;

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
        
    } else if(diff > simTime && prev.equalsIgnoreCase("SIM")){
        
        Serial.println(1);
        next = "OFF";
        setDirAndRotate(prev, next); 
        prev = "OFF";
        
        previousMillis = millis();
        
    } else if(diff > onTime && prev.equalsIgnoreCase("ON")){
        
        Serial.println(2);
        next = "SIM";
        setDirAndRotate(prev, next);
        prev = "SIM";
        
        previousMillis = millis();
    }
    
    deg = getDegree();

    if(abs(prevDeg - deg) > 40){
        // wait for 2 seconds and read again for a stable value
        delay(2000);

        deg = getDegree();
        prevDeg = deg;
        
        int minDiff = 180;
        int diff;
        int index = 4;
        for(int i = 0; i < 5; i++){
            diff = abs(deg - positions[i]);
            if(diff < minDiff){
                minDiff = diff;
                pos = positions[i];
                index = i;
            }
        }

        
        Serial.print("At degree: ");
        Serial.println(deg);

        next = modes[index];
        setDirAndRotate(prev, next);
        prev = next;
    }
    
//    delay(2000);
}

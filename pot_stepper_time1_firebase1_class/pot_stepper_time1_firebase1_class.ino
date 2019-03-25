#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcutest-641e9.firebaseio.com"
#define FIREBASE_AUTH "981Hw0F0SbDQnf3nr4AZnyzKPgYUeEjEiRTQk6cC"
#define WIFI_SSID "TP-Link_D322"
#define WIFI_PASSWORD "aqaiyoom123"
//#define WIFI_SSID "Moto G (5S) Plus 6296"
//#define WIFI_PASSWORD "ars123had"

#define MAX_MODES 6
#define MAX_BURNERS 6

int potPin = A0;    // select the input pin for the potentiometer

//int val = 0;       // variable to store the value coming from the sensor
//
//int deg = 0;       // variable to store the value coming from the sensor
//int prevDeg = 0;

int pos = 0;

// defines pins numbers
const int stepPin = D8;
const int dirPin = D7;

//String prev = "OFF";
//String next;

int positions[] = {0, 45, 90, 135, 180};
String modes[] = {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};

float getFlameFactor(String prev, String next){
//    float factor;
    if(prev.equalsIgnoreCase("ON") && next.equalsIgnoreCase("SIM")){
        return 2;
    }

//    return factor;
}

// stores object of duration for each mode for all burners
//FirebaseObject modesIntervalsNode[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
//
//// array of ignition status of each burner
//boolean ignitionStatus[6];
//
//long startMillis[6] = {0};
//boolean start = true;

int num_burners;


int onEndTime = 15000;
int simEndTime = 17000;

class StepperMotor{

    int stepPin;
    int dirPin;

    public:
        void initPins(int stepPin, int dirPin);
        void compareAndRotate(int prevIndex, String next);
        int setDirAndRotate(String prev, String next);
        void rotate(int deg, int dir);
        
};

void StepperMotor :: initPins(int stepPin, int dirPin){
    this -> stepPin = stepPin;
    this -> dirPin = dirPin;
}
void StepperMotor :: compareAndRotate(int prevIndex, String next) {
    int dir;
    for (int i = 0; i < 5; i++) {
        if (modes[i].equalsIgnoreCase(next)) {
            //  dir = i > prevIndex? 1 : -1;
            dir = i > prevIndex ? -1 : 1;
            rotate(abs(i - prevIndex) * 45, dir);
            break;
        }
    }
}
int StepperMotor :: setDirAndRotate(String prev, String next) {

    if (prev.equalsIgnoreCase("OFF")) {
        compareAndRotate(0, next);
    } else if (prev.equalsIgnoreCase("ON-OFF")) {
        compareAndRotate(1, next);
    } else if (prev.equalsIgnoreCase("ON")) {
        compareAndRotate(2, next);
    } else if (prev.equalsIgnoreCase("ON-SIM")) {
        compareAndRotate(3, next);
    } else if (prev.equalsIgnoreCase("SIM")) {
        compareAndRotate(4, next);
    }
}

void StepperMotor :: rotate(int deg, int dir) {

    int pulses = (5 * deg) / 9;
    if (pulses == 0) {
        return;
    }
    
    if (dir == 1) {
        digitalWrite(dirPin, HIGH);
    } else if (dir == -1) {
        digitalWrite(dirPin, LOW);
    }
    
    for (int x = 0; x < pulses; x++) {
    
        digitalWrite(stepPin, HIGH);
        delay(10);
        digitalWrite(stepPin, LOW);
        delay(10);
    }

}
//boolean cooking = false;

class Potentiometer{
    
    int potPin;
    
    public:

        int prevDeg = 0;
        
        void initPin(int potPin);
        int getDegree();
};

void Potentiometer :: initPin(int potPin){
    this -> potPin = potPin;
}

int Potentiometer :: getDegree() {
    
    int val = analogRead(potPin);    // read the value from the sensor

    // deg = map(val, 150, 820, 180, 0);
    int deg = map(val, 136, 840, 180, 0);
    
    if (deg < 0) {
        deg = 0;
    } else if (deg > 180) {
        deg = 180;
    }
    
    return deg;
}



class Burner{
    
    int burnerNumber;
    
    FirebaseObject modesIntervalsNode = NULL;
    String modesOrder[MAX_MODES];
    float modesFinishTimes[MAX_MODES];
    float modesIntervalsArr[MAX_MODES];

    bool modesFinished[MAX_MODES] = {false};

    int num_modes;

    String knob_status = "OFF";

    boolean flame_detect = false;

    // knob mode
    // prev mode and the mode to switch to next
    String prev, next;

    public:

        // the path to the the burner object in firebase
        String firebase_path;

        // records the time when burner is ignited        
        int startMillis;

        // motor control
        StepperMotor motor;

        // monitor potentiometer
        Potentiometer pot;
        
        boolean ignited = false;
    
        void setModesIntervalsNode(FirebaseObject modesIntervalsNode){
            this -> modesIntervalsNode = modesIntervalsNode;
        }
        FirebaseObject getModesIntervalsNode(){
            return modesIntervalsNode;
        }
        void setIgnitionStatus(boolean state){
            this -> ignited = state;
        }
        void initCooking();
        void autoKnobControl();

        void monitorManualKnob();

        boolean turnONandWaitForIgnition(String frstMode);

        void setKnobStatus(String pos){
            knob_status = pos;
            Serial.print("knob status path is");
            Serial.println(firebase_path + String("knob_status"));
            Serial.print("knob status is: ");
            Serial.println(pos);
            
            Firebase.setString(firebase_path + String("knob_status"), pos);
            // sometimes fails, so try againa after 1 second
            if(Firebase.failed()){
                delay(1000);
                Firebase.setString(firebase_path + String("knob_status"), pos);
            }
        }

        void setFlameStatus(boolean state){
            flame_detect = state;
            Serial.print("Flame status is ");
            Serial.println(firebase_path + String("flame_detect"));
            Firebase.setBool(firebase_path + String("flame_detect"), state);
        }

        void updateFirebaseModesNode(){
            Serial.println("In update firebase modes");
            // create JSON object and push to firebase
            StaticJsonBuffer<200> jsonBuffer;

            // create an object
            Serial.println("Updating..");
            JsonObject& newModesNode = jsonBuffer.createObject();
            for(int i = 0; i < num_modes; i++){
                newModesNode[modesOrder[i]] = modesIntervalsArr[i];
                Serial.print(modesOrder[i]);
                Serial.println(modesIntervalsArr[i]);
            }

            Serial.println("Updated modes node: ");
            for(JsonPair& kvp : newModesNode) {
                // But the key is actually stored in RAM (in the JsonBuffer)
                String key = kvp.key;
                float val = kvp.value;
                Serial.println(key);
                Serial.println(val);
            }
        }
};

void Burner :: initCooking(){

    Serial.println("INit cooking");

    // time when cooking has started    
    startMillis = millis();

    // reflect the same in the database
    Serial.print("flame detect path is");
    Serial.println(firebase_path + String("flame_detect"));
    Firebase.setBool(firebase_path + String("flame_detect"), true);
    ignited = true;
    
    // initialise all modes
    JsonObject& modesIntervals = modesIntervalsNode.getJsonVariant();
    num_modes = modesIntervals.size();
    Serial.print("Number of modes: ");
    Serial.println(num_modes);
    
    int i = 0;
    float sum = 0;
    float val;
    for(JsonPair& kvp : modesIntervals) {
        // But the key is actually stored in RAM (in the JsonBuffer)
        String key = kvp.key;
        modesOrder[i] = key;
        val = kvp.value;
        sum += val;
        modesIntervalsArr[i] = val;
        modesFinishTimes[i++] = sum;
//        Serial.println(key);
//        Serial.println(val);
//        Serial.println(sum);
        Serial.println(modesOrder[i-1]);
        Serial.println(modesFinishTimes[i-1]);
        Serial.println(modesIntervalsArr[i-1]);
    }
    
    
}
void Burner :: autoKnobControl(){

//    Serial.println("autoKnobControl called!");
    unsigned long diff = millis() - startMillis;
    for(int i = num_modes - 1; i >= 0; i--){
//        Serial.println(modesOrder[i]);
//        Serial.println(modesFinishTimes[i]);
//        Serial.print(modesFinished[i]);
//        Serial.println(i);
        if((diff >= modesFinishTimes[i]*60000) && (modesFinished[i] == false)){
            Serial.print(prev);
            Serial.println("is over");
            Serial.println(modesFinishTimes[i]);
            next = (i == num_modes - 1 ? "OFF": modesOrder[i+1]);
            Serial.print("Next mode is: ");
            Serial.println(next);
            motor.setDirAndRotate(prev, next);
            setKnobStatus(next);
            if(next.equalsIgnoreCase("OFF")){
                ignited = false;
                setFlameStatus(false);
                Firebase.setInt(firebase_path + String("req_to_start_stop"), 0);
            }
            prev = next;
            modesFinished[i] = true;
            Serial.print("Mode");
            Serial.print(i);
            Serial.println(" finished");
            Serial.println(modesFinished[i]);
            break;
        }
    }
}

void Burner :: monitorManualKnob(){

    // monitor manual knob(potentiometer) control
    int deg = pot.getDegree();
    
    if (abs(pot.prevDeg - deg) > 40) {
        // wait for 2 seconds and read again for a stable value
        delay(2000);
    
        deg = pot.getDegree();
        pot.prevDeg = deg;
        
        int minDiff = 180;
        int diff;
        int index = 4;
        for (int i = 0; i < 5; i++) {
            diff = abs(deg - positions[i]);
            if (diff < minDiff) {
                minDiff = diff;
                pos = positions[i];
                index = i;
            }
        }
    
    
        Serial.print("At degree: ");
        Serial.println(deg);
        
        next = modes[index];
//        burners[burnerNumber].motor.setDirAndRotate(prev, next);
        motor.setDirAndRotate(prev, next);
        
//        unsigned long diff1 = millis() - burners[burnerNumber].startMillis;
        unsigned long diff1 = millis() - startMillis;

        for(int i = 0; i < num_modes; i++){
            if((diff1 <= modesFinishTimes[i]*60000)){
                float prevEndTime = modesFinishTimes[i];

                // this mode has stopped early and the time is diff1
                modesFinishTimes[i] = (float)diff1/60000;

                Serial.print("Updated finish time for ");
                Serial.println(modesOrder[i]);
                Serial.println(modesFinishTimes[i]);

                float durReduced = prevEndTime - modesFinishTimes[i];

                Serial.print("Duration reduced: ");
                Serial.println(durReduced);

                modesIntervalsArr[i] -= durReduced;

                // if the manual knob position is the same as the next upcoming step in the automatic system mode order
                // then reduce all subsequent mode finish times by 'durReduced'
                if(i < num_modes - 1 && next.equalsIgnoreCase(modesOrder[i+1])){
                    for(int j = i+1; j < num_modes; j++){
                        modesFinishTimes[j] -= durReduced;
                    }
                    updateFirebaseModesNode();
                }
                // if the system is in its last step, and the user abrupts this last step and goes on to a new step
                else if(i == num_modes -1 && !next.equalsIgnoreCase("OFF")){
                    float k;
                    num_modes++;
                    modesOrder[i+1] = next;
                    modesFinishTimes[i+1] = modesFinishTimes[i] + durReduced * getFlameFactor(prev, next);
                    modesIntervalsArr[i+1] = durReduced * getFlameFactor(prev, next);
                    updateFirebaseModesNode();
                }
                // if the system is not in the last step and the user abrupts the step and manually goes to a new step
                // then the new step and its finish time should be added to the arrays
                else if(i < num_modes - 1 && !next.equalsIgnoreCase(modesOrder[i+1])){
                    float k;
                    // shift the arrays to insert the new mode and finish time in between
                    for(int j = num_modes; j > i+1; j--){
                        modesOrder[j] = modesOrder[j-1];
                        modesFinishTimes[j] = modesFinishTimes[j-1];
                        modesIntervalsArr[j] = modesIntervalsArr[j-1];
                    }
                    num_modes++;
                    modesOrder[i+1] = next;
                    modesFinishTimes[i+1] = modesFinishTimes[i] + durReduced * getFlameFactor(prev, next);
                    modesIntervalsArr[i+1] = durReduced * getFlameFactor(prev, next);
                    updateFirebaseModesNode();
                    
                }
                break;
            }
        }
        
        prev = next;
    }
}

boolean Burner :: turnONandWaitForIgnition(String frstMode){

    Serial.println("turn on and wait");
    
    prev = "OFF";
    next = frstMode;
    
    motor.setDirAndRotate(prev, next);
    setKnobStatus(next);
    prev = next;

    // TODO: detect flame
    boolean flame_detect = true;
    setFlameStatus(flame_detect);
    // check for flame 5 times within 10 seconds
    for(int i = 2000; i <= 10000; i+=2000){
        delay(i);
        if(flame_detect){
            break;
        }
    }

    if(!flame_detect){
        return false;
    } else {
        return true;
    }
}

Burner burners[MAX_BURNERS];


void setup() {
    
    Serial.begin(9600);

    // stepper motor pins
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    
    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    if(Firebase.success()){
        Serial.println("connection made"); 
    }
    
    String check;
    
    // wait until the app has initialised the user's information
    do {
        check = Firebase.getString("/aaa/model");
    } while(Firebase.failed() || check.equalsIgnoreCase(""));

    
    num_burners = Firebase.getInt("/aaa/config/num_burners");
    for(int i = 0; i < num_burners; i++){
//        burners[i].ignited = false;
//        burners[i].setModesIntervalsNode(NULL);
        burners[i].motor.initPins(D8, D7);
        burners[i].pot.initPin(A0);
    }

    
    Firebase.stream("/aaa");
    if(Firebase.success()){
        Serial.println("streaming success"); 
    } else {
        Serial.println(Firebase.error());
    }

}

void loop() {

    // TODO: check for mass of vessel

    // monitor manual knob and auto knob control for each burner
    for(int i = 0; i < num_burners; i++){
        // reference is very very important!!
        Burner &curr_burner = burners[i];
        if(curr_burner.ignited){
            curr_burner.monitorManualKnob();
//            Serial.print("calling auto knob control on burner");
//            Serial.println(i);
            curr_burner.autoKnobControl();
        }
    }


    if (Firebase.available()) {
        FirebaseObject event = Firebase.readEvent();
        String eventType = event.getString("type");
        eventType.toLowerCase();
        
        Serial.print("event: ");
        Serial.println(eventType);
        
        if (eventType == "put") {
            
            String path = event.getString("path");
            Serial.println(path);
            String data = event.getString("data");
            
            if(path.substring(8).equalsIgnoreCase("/req_to_start_stop")){
                
                String burner = path.substring(1,8);
                String path = "/aaa/" + burner + "/";
                int data = event.getInt("data");
//                String knob_status = Firebase.getString("/aaa/" + burner + "knob_status");
                Serial.println(path);
                String knob_status = Firebase.getString(path + "/knob_status");
                Serial.println(data);
                Serial.println(knob_status);
                if(knob_status.equalsIgnoreCase("OFF") && data == 1){
                    // the request is to switch on the burner
                    Serial.println("Need to start!");

                    String food_name = Firebase.getString(path + "food_name");
                    Serial.println(food_name);
//                    String stepPath = String("/aaa/") + String("Food_profile/") + food_name + String("/steps");
//                    Serial.println(stepPath);

                    Serial.println(burner.substring(6));
                    int burnerNumber = (burner.substring(6).toInt()) - 1;
                    burners[burnerNumber].firebase_path = path;
                    Serial.print("Burner number is: ");
                    Serial.println(burnerNumber);
                    FirebaseObject modesIntervalsNode = Firebase.get(String("/aaa/") + String("Food_profile/") + food_name + String("/steps"));
                    burners[burnerNumber].setModesIntervalsNode(modesIntervalsNode);
                    JsonObject& modesIntervals = (burners[burnerNumber].getModesIntervalsNode()).getJsonVariant();
                    
                    String frstMode;
                    for(JsonPair& kvp : modesIntervals) {
                        // But the key is actually stored in RAM (in the JsonBuffer)
                        frstMode = kvp.key;
                        Serial.println(frstMode);
                        break;
                        
//                        int val = kvp.value;
//                        Serial.println(key);
//                        Serial.println(val);
                    }
                    
                    boolean ignited = burners[burnerNumber].turnONandWaitForIgnition(frstMode);
                    if(ignited){
                        burners[burnerNumber].initCooking();
//                        burners[burnerNumber].startMillis[burnerNumber] = millis();
//                        Firebase.setBool(path + String("flame_detect"), true);
//                        ignitionStatus[burnerNumber] = true;
                        
                    }
                    
                    
                } else if(!knob_status.equalsIgnoreCase("OFF") && data == 0){
                    // the request is to switch off the burner
                    Serial.println("Need to stop!");
                }
                Serial.print("data: ");
            }
        
        }
    }
    delay(1000);
}

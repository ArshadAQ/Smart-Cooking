#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

// Set these to run example.
//#define FIREBASE_HOST "nodemcutest-641e9.firebaseio.com"
//#define FIREBASE_AUTH "981Hw0F0SbDQnf3nr4AZnyzKPgYUeEjEiRTQk6cC"


#define FIREBASE_HOST "stoveapp-8cd69.firebaseio.com"
#define FIREBASE_AUTH "1i6onmEYLJeKvwPeSsRUwU6cwm9QCb4fqunRpNE2"

//#define WIFI_SSID "TP-Link_D322"
//#define WIFI_PASSWORD "aqaiyoom123"
#define WIFI_SSID "Moto G (5S) Plus 6296"
#define WIFI_PASSWORD "ars123had"

#define MAX_MODES 6
#define MAX_BURNERS 6

//StaticJsonBuffer<1000> jsonBuffer;
//StaticJsonBuffer<400> jsonBuffer_arr;
//char JSONmessageBuffer[1024];
//const uint8_t fingerprint[20] = {0xE4, 0xE8, 0x1D, 0xD2, 0x66, 0x6D, 0x90, 0x71, 0x25, 0xC0, 0xDA, 0x53, 0x0B, 0x6F, 0x5B, 0xDF, 0x8C, 0xEC, 0x8B, 0x9E};


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
    Serial.println("getting factor..");
    if(prev.substring(0,2).equalsIgnoreCase("ON") && next.substring(0,6).equalsIgnoreCase("ON-OFF")){
        return 1.5;
    } else if(prev.substring(0,2).equalsIgnoreCase("ON") && next.substring(0,6).equalsIgnoreCase("ON-SIM")){
        return 1.5;
    } else if(prev.substring(0,6).equalsIgnoreCase("ON-SIM") && next.substring(0,2).equalsIgnoreCase("ON")){
        return 2.0/3;
    } else if(prev.substring(0,3).equalsIgnoreCase("SIM") && next.substring(0,6).equalsIgnoreCase("ON-SIM")){
        return 2.0/3;
    } else if(prev.substring(0,3).equalsIgnoreCase("SIM") && next.substring(0,2).equalsIgnoreCase("ON")){
        return 0.5;
    } else if(prev.substring(0,3).equalsIgnoreCase("SIM") && next.substring(0,6).equalsIgnoreCase("ON-OFF")){
        return 2.0/3;
    }
    return 0;
//    return factor;
}


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

    if (prev.substring(0,3).equalsIgnoreCase("OFF")) {
        compareAndRotate(0, next);
    } else if (prev.substring(0,6).equalsIgnoreCase("ON-OFF")) {
        compareAndRotate(1, next);
    } else if (prev.substring(0,2).equalsIgnoreCase("ON")) {
        compareAndRotate(2, next);
    } else if (prev.substring(0,6).equalsIgnoreCase("ON-SIM")) {
        compareAndRotate(3, next);
    } else if (prev.substring(0,3).equalsIgnoreCase("SIM")) {
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

class Loadcell{
      int dt;
      int sck;
      public:

        float mass = 0;
        void initPins(int dt, int sck);
        float getMass();
        
};

void Loadcell :: initPins(int dt, int sck){
    this -> dt = dt;
    this -> sck = sck;
}

float Loadcell :: getMass(){
    return 0.0;
}

class Burner{
    
    
    FirebaseObject modesIntervalsNode = NULL;
    String modesOrder[MAX_MODES];
    float modesFinishTimes[MAX_MODES];
    float modesIntervalsArr[MAX_MODES];

    bool modesFinished[MAX_MODES] = {false};

    int num_modes;

    int count = 0;

    String knob_status = "OFF";

    boolean flame_detect = false;

    // knob mode
    // prev mode and the mode to switch to next
//    String prev, next;

    public:

        String prev, next;

        int burnerNumber;

        String food_name;

        float mass;

        // the path to the the burner object in firebase
        String firebase_path;

        // records the time when burner is ignited 
        // this value is also changed when the user wants to continue cooking from the last mode. It is moved forward by an amount equal to the gap between stopping and restarting cooking
        int startMillis;

        // records the time when system came to a halt(aka, knob position reached "OFF")
        int endMillis;

        // motor control
        StepperMotor motor;

        // monitor potentiometer
        Potentiometer pot;

        Loadcell weight;
        
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
        // initialises the modes order required to be followed
        void initCooking();

        // when the user opts to restart cooking - this func does the necessary req to bring the system back to its last state
        void continueCooking();
        
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

        // function that updates knob modes duration in the database as and when the user overrides the duration
        void updateFirebaseModesNode(){
            Serial.println("In update firebase modes");
            // create JSON object and push to firebase
            StaticJsonBuffer<400> jsonBuffer;

            // create an object
            Serial.println("Updating..");
            JsonObject& newModesNodes = jsonBuffer.createObject();
            for(int i = 0; i < num_modes; i++){
                // Remove modes with mode duration < threshold
                if(modesIntervalsArr[i] < 0.2){
                    Serial.println("below threshold..");
                    for(int j = i+1; j < num_modes; j++){
                        modesFinishTimes[j-1] = modesFinishTimes[j];
                        modesOrder[j-1] = modesOrder[j];
                        modesIntervalsArr[j-1] = modesIntervalsArr[j];
                    }
                    num_modes--;
                }
                JsonObject& newModesNode = newModesNodes.createNestedObject(String("mode") + String(i+1));

                newModesNode[modesOrder[i]] = modesIntervalsArr[i];
                Serial.print(modesOrder[i]);
                Serial.println(modesIntervalsArr[i]);
                float inter = newModesNode[modesOrder[i]];
                Serial.println(inter);
                Serial.println(modesFinishTimes[i]);
            }

            // Remove modes with mode duration < threshold
            Serial.println("Updated modes node: ");
            for(JsonPair& nobj : newModesNodes) {
                JsonObject& obj = nobj.value;
                Serial.println(nobj.key);
                for(JsonPair& kvp: obj){
                    // But the key is actually stored in RAM (in the JsonBuffer)
                    String key = kvp.key;
                    float val = kvp.value;
                    Serial.println(key);
                    Serial.println(val);
                }
            }
            Serial.println(String("/aaa/Food_profile/") + food_name + String("/steps"));
            Firebase.set(String("/aaa/Food_profile/") + food_name + String("/steps"), newModesNodes);
            if(Firebase.failed()){
                Serial.println("Failed to update modes");
                Serial.println(Firebase.error());
                delay(1000);
                Firebase.set(String("/aaa/Food_profile/") + food_name + String("/steps"), newModesNodes);
            }
        }
};

boolean Burner :: turnONandWaitForIgnition(String frstMode){

    Serial.println("turn on and wait");

    // make sure manual knob is in OFF position
    int degree = pot.getDegree();
    if(degree > 20){
        Firebase.setBool(firebase_path + String("req_to_off_knob"), true);
        for(int i = 2000; i <= 10000; i+=2000){
            delay(i);
            if(pot.getDegree() < 20){
                Firebase.setBool(firebase_path + String("req_to_off_knob"), false);
                break;
            }
        }
    }
    
    prev = "OFF";
    next = frstMode;

    Serial.println("Next step is :" + prev + next);
    
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
    for (JsonPair& modes : modesIntervals){
        JsonObject& interval = modes.value;
        for(JsonPair& kvp : interval){
            String key = kvp.key;
            modesOrder[i] = key;
            val = kvp.value;
            sum += val;
            modesIntervalsArr[i] = val;
            modesFinishTimes[i++] = sum;
            Serial.println(modesOrder[i-1]);
            Serial.println(modesFinishTimes[i-1]);
            Serial.println(modesIntervalsArr[i-1]);
        }
    }

    // if cooking starts again, then none of the modes is finished
    for(int i = 0; i < num_modes; i++){
        modesFinished[i] = false;
    }

}
    
void Burner :: continueCooking(){

    Serial.println("continue cooking");

    String lstMode = modesOrder[num_modes - 1];
    boolean ignited_test = turnONandWaitForIgnition(lstMode);
    
    if(ignited_test){
        
        // after how long the system has restarted
        unsigned long delayed = millis() - endMillis;
        // move forward the start time by the amount delayed
        startMillis += delayed;

        // get the time the user wants the food to be cooked more for
//        float timeToExtend = Firebase.getFloat(firebase_path + String("extra_time"));

        float timeToExtend = 0;

//        do{
            timeToExtend = Firebase.getFloat(firebase_path + String("extra_time"));
            if(timeToExtend == 0){
                delay(2000);
                Serial.println("Failed to retrieve extra_time");
                Serial.println("mm1");
                Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
                if(Firebase.success()){
                    Serial.println("connection made"); 
                }
            }
//        } while(Firebase.failed() || timeToExtend == 0);

//        if(Firebase.failed()){
//            Serial.println("Failed to retrieve extra_time");
//            delay(1000);
//            timeToExtend = Firebase.getFloat(firebase_path + String("extra_time"));
//        }

        Serial.print("Time to extend is: ");
        Serial.println(timeToExtend);

        // add this duration to the last mode in the modes order
        modesIntervalsArr[num_modes - 1] += timeToExtend;
        modesFinishTimes[num_modes - 1] += timeToExtend;

        updateFirebaseModesNode();
        
        // reflect the ignition status in the database
        Serial.print("flame detect path is");
        Serial.println(firebase_path + String("flame_detect"));
        Firebase.setBool(firebase_path + String("flame_detect"), true);
        ignited = true; 

        // the last mode was marked as seen, so unmark it as the user is continuing cooking from this step.
        modesFinished[num_modes - 1] = false;
    }
    
    
    
}
void Burner :: autoKnobControl(){

    Serial.println("autoKnobControl called!");
    unsigned long diff = millis() - startMillis;
    Serial.print("diff is: ");
    Serial.println(diff);
    Serial.println(num_modes);
    String trimNext;
    for(int i = num_modes - 1; i >= 0; i--){

        if((diff >= modesFinishTimes[i]*60000) && (modesFinished[i] == false)){
            Serial.print(prev);
            Serial.println("is over");
            Serial.println(modesFinishTimes[i]);
            next = (i == num_modes - 1 ? "OFF": modesOrder[i+1]);

            // to remove any trailing digits
            trimNext = next.substring(0, next.length());
            Serial.print("Next mode is: ");
            Serial.println(trimNext);
            motor.setDirAndRotate(prev, trimNext);
            setKnobStatus(trimNext);
            if(next.equalsIgnoreCase("OFF")){
                endMillis = millis();
                ignited = false;
                setFlameStatus(false);
                Firebase.setInt(firebase_path + String("req_to_start_stop"), 0);
                Firebase.setInt(firebase_path + String("continue"), 0);
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
        if(prev.equalsIgnoreCase(next)){
            return;
        }
        motor.setDirAndRotate(prev, next);
        
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
                // if OFF is chosen as the next mode by the user
                // then delete all other subsequent steps from the modes list
                else if(next.equalsIgnoreCase("OFF")){
                    int gap = num_modes - (i+1);
                    Serial.print("No. of deleted modes: ");
                    Serial.println(gap);
                    num_modes -= gap;

                    // this is called so that the dur reduced in the current step is reflected in the db
                    updateFirebaseModesNode();
                }
                // if the the user abrupts a step in between(either in between or in the last) and manually goes to a new step(which is not OFF)
                // then the new step and its finish time should be added to the arrays
                else if( (i < num_modes - 1 && !next.equalsIgnoreCase(modesOrder[i+1])) || (i == num_modes -1 && !next.equalsIgnoreCase("OFF")) ){
                    Serial.println("NEW step in between..");
                    
                    // check if the next mode is already present in subsequent modes
                    bool presentInSubsequent = false;
                    int index = 0;
                    for(int j = i+2; j < num_modes; j++){
                        if(next.equalsIgnoreCase(modesOrder[j])){
                            presentInSubsequent = true;
                            index = j;
                            break;
                        }
                    }
                    // if it is already present in subsequent modes, then delete all modes in between
                    if(presentInSubsequent){
                        Serial.println("Present in subsequent..at index");
                        Serial.println(index);
                        int gap = index-(i+1);
                        for(int j = index; j < num_modes; j++){
                            modesOrder[j-gap] = modesOrder[j];
                            modesFinishTimes[j-gap] = modesFinishTimes[j];
                            modesIntervalsArr[j-gap] = modesIntervalsArr[j];
                        }

                        // update finish times
                        for(int j = i+1; j <= index; j++){
                            modesFinishTimes[j] = modesFinishTimes[j-1] + modesIntervalsArr[j];
                        }
                        num_modes -= gap;
                        Serial.println("number of modes");
                        Serial.println(num_modes);
                        updateFirebaseModesNode();
                    } else {

                        // shift the arrays to insert the new mode and finish time in between
                        for(int j = num_modes; j > i+1; j--){
                            modesOrder[j] = modesOrder[j-1];
                            modesFinishTimes[j] = modesFinishTimes[j-1];
                            modesIntervalsArr[j] = modesIntervalsArr[j-1];
                        }
                        num_modes++;

                        // check if the next mode was already encountered in previous modes
                        bool presentInPrev = false;
                        int index = 0;
                        for(int j = 0; j < i; j++){
                            if(next.equalsIgnoreCase(modesOrder[j])){
                                presentInPrev = true;
                                Serial.println("present in previous..");
                                index = j;
                                break;
                            }
                        }

                        // if present in previous modes, then add a counter to the mode name
                        if(presentInPrev){
                            modesOrder[i+1] = next + String(count++);
                        } else{
                            modesOrder[i+1] = next;
                        }
                        modesFinishTimes[i+1] = modesFinishTimes[i] + (durReduced * getFlameFactor(prev, next));
                        modesIntervalsArr[i+1] = durReduced * getFlameFactor(prev, next);

                        // change finish times after inserting mode in between
                        for(int j = i+2; j < num_modes; j++){
                            modesFinishTimes[j] = modesFinishTimes[j-1] + modesIntervalsArr[j];
                        }
                        updateFirebaseModesNode();
                    }
                    
                }
                break;
            }
        }
        
        prev = next;
    }
}


Burner burners[MAX_BURNERS];

class ML_model{

    public:


    static String instance_id;
    static String base_url;
    static String access_token;

    static bool onoffFound;
    static float onoffTime;
    static int orders[4];   
    
    String published_model_id;
    String deployment_id;

    ML_model(String published_model_id, String deployment_id){
        this -> published_model_id = published_model_id;
        this -> deployment_id = deployment_id;
    }


    void init(String published_model_id, String deployment_id){
        this -> published_model_id = published_model_id;
        this -> deployment_id = deployment_id;
    }

    static void getFeedbackData(Burner b){
        Serial.println("getting feedback data..");
        FirebaseObject modesIntervalsNode = Firebase.get(String("/aaa/") + String("Food_profile/") + b.food_name + String("/steps"));
        JsonObject& obj = modesIntervalsNode.getJsonVariant();

        bool onoffFound = false;



        // fill the order and also on-off value if present

        for (JsonPair& modes1 : obj){
            JsonObject& interval = modes1.value;
            for(JsonPair& kvp : interval){
                String key = kvp.key;
                if(key.equalsIgnoreCase("ON-OFF")){
                    float val = kvp.value;
                    onoffTime = val;
                    onoffFound = true;

                    orders[0] = (String(modes1.key).substring(4)).toInt();
                    Serial.println(orders[0]);
                } else if(key.equalsIgnoreCase("ON")){
                    orders[1] = (String(modes1.key).substring(4)).toInt();
                    Serial.println(orders[1]);
                } else if(key.equalsIgnoreCase("ON-SIM")){
                    orders[2] = (String(modes1.key).substring(4)).toInt();
                    Serial.println(orders[2]);
                } else if(key.equalsIgnoreCase("SIM")){
                    orders[3] = (String(modes1.key).substring(4)).toInt();
                    Serial.println(orders[3]);
                }
//                if(found) break;
            }
        }


    }

    // post data for on-offTime, order0,1,2,3
    void postFeedbackData(Burner b, String option){

        Serial.println(String("In post feedback data ") + option);

        StaticJsonBuffer<256> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        
        JsonArray& fields = root.createNestedArray("fields");
        fields.add("Food_item");
        fields.add("Mass");
        fields.add("Burner_size");

        JsonArray& values = root.createNestedArray("values");

        StaticJsonBuffer<200> jsonBuffer_arr;

        // create an empty array
        JsonArray& value = jsonBuffer_arr.createArray();

        value.add(b.food_name);
        String burner_size;
        burner_size = Firebase.getString(String("/aaa/") + String("config/") + String("size/") + String("Burner") + b.burnerNumber);
        Serial.println(String("burner size is: ") + burner_size);
        value.add(b.mass);
        value.add(burner_size);

        
        if(option.equalsIgnoreCase("on-offTime") && onoffFound){
            fields.add("on_offTime");
            value.add(onoffTime);
        }
        else if(option.equalsIgnoreCase("order0")) {
            fields.add("order0");
            value.add(orders[0]);
        }
        else if(option.equalsIgnoreCase("order1")) {
            fields.add("order1");
            value.add(orders[1]);
        }
        else if(option.equalsIgnoreCase("order2")) {
            fields.add("order2");
            value.add(orders[2]);
        }
        else if(option.equalsIgnoreCase("order3")) {
            fields.add("order3");
            value.add(orders[3]);
        }

        values.add(value);

        char JSONmessageBuffer[1000];
        root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println(JSONmessageBuffer);

        String url = base_url + instance_id + String("/published_models/") + published_model_id + String("/feedback");

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

//        client->setFingerprint(fingerprint);

        HTTPClient http;

        Serial.println(url);
        
        http.begin(*client, url);

        
        http.addHeader("Content-Type", "application/json");            
        http.addHeader("Authorization", String("Bearer ") + access_token);

//        jsonBuffer.clear();
//        jsonBuffer_arr.clear();

        DynamicJsonBuffer jsonBuffer1;

        String payload = http.getString();  //Get the response payload
 
        Serial.println(payload);    //Print request response payload

        JsonObject& root1 = jsonBuffer1.parseObject(payload);
        
        
        int httpResponseCode = http.POST(JSONmessageBuffer);
        Serial.println(httpResponseCode);
        if(httpResponseCode != 200){
            Serial.println("error sending http request..");
            if(httpResponseCode == 401){
                Serial.println("Token expired..");
                root1["errors"][0].printTo(Serial);
                String err_msg = root1["errors"][0]["code"];
                if(err_msg.equalsIgnoreCase("expired_authorization_token")){
                    refreshToken();
                    postFeedbackData(b, option);
                }
            }
            delay(10000);
        }

        http.end();  //Close connections

    }

    // possiblity to refactor here. The jsonobject gets created and destroyed for every call of this function. This could instead be kept in memory.
    void predict(String food_name, float mass, String burner_size, String option){

        Serial.println("In predict..");
        

        String url = base_url + instance_id + String("/published_models/") + published_model_id + String("/deployments/") + deployment_id + String("/online");

        // creating object to send a request
        StaticJsonBuffer<256> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        
        JsonArray& fields = root.createNestedArray("fields");
        fields.add("Food_item");
        fields.add("Mass");
        fields.add("Burner_size");

        JsonArray& values = root.createNestedArray("values");

        StaticJsonBuffer<200> jsonBuffer_arr;

        // create an empty array
        JsonArray& value = jsonBuffer_arr.createArray();

        value.add(food_name);
        value.add(mass);
        value.add(burner_size);

        values.add(value);

        char JSONmessageBuffer[1000];
        root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println(JSONmessageBuffer);


        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

//        client->setFingerprint(fingerprint);

        HTTPClient http;


        Serial.println(url);
        
        http.begin(*client, url);
        

        
        http.addHeader("Content-Type", "application/json");            
        http.addHeader("Authorization", "Bearer " + access_token);
        
        int httpResponseCode = http.POST(JSONmessageBuffer);
        Serial.println(httpResponseCode);
        

        DynamicJsonBuffer jsonBuffer1;

        String payload = http.getString();  //Get the response payload
 
        Serial.println(payload);    //Print request response payload

        JsonObject& root1 = jsonBuffer1.parseObject(payload);
        
        if(httpResponseCode != 200){
            Serial.println("error sending http request..");
            root1.printTo(Serial);
            if(httpResponseCode == 401){
                Serial.println("Token expired..");
                root1["errors"][0].printTo(Serial);
                String err_msg = root1["errors"][0]["code"];
                if(err_msg.equalsIgnoreCase("expired_authorization_token")){
                    refreshToken();
                    predict(food_name, mass, burner_size, option);
                }
            }
            delay(10000);
        }

        

        if(option.equalsIgnoreCase("onoffTime")){

            onoffTime = root1["values"][0][0][4];
            Serial.println(onoffTime);
            
        } else if(option.equalsIgnoreCase("order0")){

            orders[0]= root1["values"][0][0][7];
            Serial.println(orders[0]);
            
        } else if(option.equalsIgnoreCase("order1")){
            orders[1]= root1["values"][0][0][7];
            Serial.println(orders[1]);
            
        } else if(option.equalsIgnoreCase("order2")){
            orders[2]= root1["values"][0][0][7];
            Serial.println(orders[2]);
            
        } else if(option.equalsIgnoreCase("order3")){
            orders[3]= root1["values"][0][0][7];
            Serial.println(orders[3]);
            
        }

//        jsonBuffer1.clear();

        // save onofftime/order0/1/2/3
 
        
        http.end();  //Close connections

        
        
    }

    static void pushPredictedToFirebase(String food_name){
        StaticJsonBuffer<400> jsonBuffer;

        // create an object
        Serial.println("pushing predicted to firebase..");
        JsonObject& newModesNodes = jsonBuffer.createObject();
        for(int i = 0; i < 4; i++){
            if(orders[i] == -1){
                continue;
            }
            Serial.println(modes[i]);
            Serial.println(orders[i]);
            JsonObject& newModesNode = newModesNodes.createNestedObject(String("mode") + String(orders[i]+1));
            if(modes[i].equalsIgnoreCase("ON-OFF")){
                newModesNode[modes[i]] = onoffTime;   
            } else if(modes[i].equalsIgnoreCase("ON")){
                newModesNode[modes[i]] = onoffTime/2;   
            } else if(modes[i].equalsIgnoreCase("ON-SIM")){
                newModesNode[modes[i]] = onoffTime;   
            } else if(modes[i].equalsIgnoreCase("SIM")){
                newModesNode[modes[i]] = onoffTime*2;
            }
            
        }

        Firebase.set(String("/aaa/Food_profile/") + food_name + String("/steps"), newModesNodes);
        if(Firebase.failed()){
            Serial.println("Failed to push predicted to modes");
            Serial.println(Firebase.error());
            delay(1000);
            Firebase.set(String("/aaa/Food_profile/") + food_name + String("/steps"), newModesNodes);
        }

    }

    static void refreshToken(){
        Serial.println("Refreshing Token..");
        String url = "https://ibm-watson-ml.mybluemix.net/v3/identity/token";
        // old token in request body as json
        // new token in response body

        StaticJsonBuffer<256> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        root["token"] = access_token;

        char JSONmessageBuffer[1000];
        root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println(JSONmessageBuffer);


        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

//        client->setFingerprint(fingerprint);

        HTTPClient http;


        Serial.println(url);
        
        http.begin(*client, url);
        
        
        http.addHeader("Content-Type", "application/json");            
        http.addHeader("Authorization", "Bearer " + access_token);

        
        int httpResponseCode = http.PUT(JSONmessageBuffer);
        Serial.println(httpResponseCode);
        if(httpResponseCode != 200){
            Serial.println("error..");
            delay(10000);
            refreshToken();
        }
        

//        jsonBuffer.clear();
//        jsonBuffer_arr.clear();




        DynamicJsonBuffer jsonBuffer1;

        String payload = http.getString();  //Get the response payload
 
        Serial.println(payload);    //Print request response payload

        JsonObject& root1 = jsonBuffer1.parseObject(payload);

        access_token = (const char*)root1["token"];

        Serial.println("The new token is.." + access_token);
        
    }
    
    
};


String ML_model::instance_id = "c5962ef1-01be-4bb5-a395-9916e3ee2b68";
String ML_model::base_url = "https://ibm-watson-ml.mybluemix.net/v3/wml_instances/";
//String ML_model::access_token = "eyJhbGciOiJSUzUxMiIsInR5cCI6IkpXVCJ9.eyJ0ZW5hbnRJZCI6ImM1OTYyZWYxLTAxYmUtNGJiNS1hMzk1LTk5MTZlM2VlMmI2OCIsImluc3RhbmNlSWQiOiJjNTk2MmVmMS0wMWJlLTRiYjUtYTM5NS05OTE2ZTNlZTJiNjgiLCJwbGFuSWQiOiIzZjZhY2Y0My1lZGU4LTQxM2EtYWM2OS1mOGFmM2JiMGNiZmUiLCJyZWdpb24iOiJ1cy1zb3V0aCIsInVzZXJJZCI6ImVmYWNkNTM3LTY4ODUtNGE1MC04MTA5LWUxYTUyNzYzM2Q2ZiIsImlzcyI6Imh0dHBzOi8vdXMtc291dGgubWwuY2xvdWQuaWJtLmNvbS92My9pZGVudGl0eSIsImlhdCI6MTU1NDA0MDExNiwiZXhwIjoxNTU0MDY4OTE2LCJjcmVhdGVkVGltZSI6MTU1Mzg1Mjk4OX0.U2fU4L5x7_loCSRLh0lh8b6ufMLPLbj-1CN5s5cTRtNnhX8H_tn85oj-OTYbjDTX_4YckrU15kaAVWcY0U5GP0Z5s6VVBiLUOnjpfp23ofC8UJlouQvj7bBGHTzKVWNnF0WaX8ZvQb6OBwX0maOMJPGpzldpmM54dYjHlhn4S1lPjIhOVd7NAjHW-LR3u0x8XuxAbXdrCYk-bCwnXnQ4_lqgyb4qVfqcRunZk5KWyE9OEr5D-0yBBjjyhd3WA0td5z18yMiBJNkGjjzXKPLBASWXCnsKNswn_Uuk-tOxI90FDFW2WRbVtvu5dNM8FL2psU003zCf8fz-645Vkmpyxw";
String ML_model :: access_token = "eyJhbGciOiJSUzUxMiIsInR5cCI6IkpXVCJ9.eyJ0ZW5hbnRJZCI6ImM1OTYyZWYxLTAxYmUtNGJiNS1hMzk1LTk5MTZlM2VlMmI2OCIsImluc3RhbmNlSWQiOiJjNTk2MmVmMS0wMWJlLTRiYjUtYTM5NS05OTE2ZTNlZTJiNjgiLCJwbGFuSWQiOiIzZjZhY2Y0My1lZGU4LTQxM2EtYWM2OS1mOGFmM2JiMGNiZmUiLCJyZWdpb24iOiJ1cy1zb3V0aCIsInVzZXJJZCI6ImVmYWNkNTM3LTY4ODUtNGE1MC04MTA5LWUxYTUyNzYzM2Q2ZiIsImlzcyI6Imh0dHBzOi8vdXMtc291dGgubWwuY2xvdWQuaWJtLmNvbS92My9pZGVudGl0eSIsImlhdCI6MTU1NDE0ODM2NCwiZXhwIjoxNTU0MTc3MTY0LCJjcmVhdGVkVGltZSI6MTU1Mzg1Mjk4OX0.AIbBPqQ8RLXMCLAHDxzFCfuQXesdKlaadOCUmj8rcOKPDEAREHSM3hZ-0nc6d-uAmrra7YTaX4EcxUIwJdKZzotDibe1nBB9QFBM7lm9-DD4I7U4elwJhBKpCPh2zRooKjGdyO1plK6CHFJ6huoNP8vF6Dr__u7YFcswa9hRyY-upsR5BVXNzn44SDHYLoqjH3fLD5aJNoRt8yVjLuSGaSEx8JspuXt0Sxrxk-43Z-1Z2FTEiYIj-MhRYS_SobrAIFF2-vlaoiBKKq8jXmh8UDw_IIr1rShOQPXEjK6ge-T3BECieP4OjDYYwvJO_BgvEByeRt-Su6N5bJg3ODrKYg";

int ML_model :: orders[4];
bool ML_model::onoffFound = false;

float ML_model :: onoffTime = 0;

// model ids and deployment ids of 5 models
ML_model ml[5] = { {"e9aeec1b-9fff-4338-a7f1-a9ad2493a915", "e39b9323-d068-478f-a447-3aca2b282d8a"},
                   {"6469af67-2a25-48c8-8172-c936707996d7", "4f209f47-ce78-4939-9820-6d2c381cebe3"},
                   {"a38da041-b45e-455d-b3dc-1e422e2faf37", "f9c27b60-1a5f-45c3-8225-4ecd8d17c392"},
                   {"c3f560b5-2112-4455-a440-65307ac93def", "59b70da1-5f57-428b-96eb-e0ef0b1b3232"},
                   {"3c96a57b-71d8-4231-8140-1bbff9bd8b1b", "199e91d2-8cf4-464d-a132-f9b8d2fbdac9"}
                };

FirebaseArduino f;
void setup() {

    f.begin(FIREBASE_HOST, FIREBASE_AUTH);
    
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
        Serial.println("Hi");
        Serial.println(check);
    } while(Firebase.failed() || check.equalsIgnoreCase(""));

    
    num_burners = Firebase.getInt("/aaa/config/num_burners");
    Serial.println(num_burners);
    
    for(int i = 0; i < num_burners; i++){
        burners[i].motor.initPins(D8, D7);
        burners[i].pot.initPin(A0);
        burners[i].weight.initPins(D2, D3);
        burners[i].burnerNumber = i+1;
    }


    
//    Firebase.stream("/aaa");
//    if(Firebase.success()){
//        Serial.println("streaming success"); 
//    } else {
//        Serial.println("asasfassfaf");
//        Serial.println(Firebase.error());
//    }

     f.stream("/aaa");
    if(f.success()){
        Serial.println("streaming success"); 
    } else {
        Serial.println("asasfassfaf");
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
            curr_burner.autoKnobControl();
        }
    }


    if (f.available()) {
        FirebaseObject event = f.readEvent();
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

                Serial.println(path);
                String knob_status = Firebase.getString(path + "/knob_status");
                if(Firebase.failed()){
                    Serial.println("get knob status failed.. trying again");
                    delay(1000);
                        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
                            if(Firebase.success()){
                                Serial.println("connection made"); 
                            }
                    knob_status = Firebase.getString(path + "/knob_status");
                }
                Serial.println(data);
                Serial.println(knob_status);
                if(knob_status.equalsIgnoreCase("OFF") && data == 1){
                    // the request is to switch on the burner
                    Serial.println("Need to start!");

                    // Get the selected food name
                    String food_name = Firebase.getString(path + "Food_name");
                    Serial.println(food_name);
                    
                    // Get the burner number
                    Serial.println(burner.substring(6));
                    int burnerNumber = (burner.substring(6).toInt()) - 1;
                    burners[burnerNumber].firebase_path = path;
                    burners[burnerNumber].food_name = food_name;
                    Serial.print("Burner number is: ");
                    Serial.println(burnerNumber);

                    // Get the burner size
                    Serial.println(String("/aaa/config/size/") + burner);
                    String burner_size = Firebase.getString(String("/aaa/config/size/") + burner);
                    Serial.println(burner_size);

                    // Vessel is already kept as only then the app sends a signal to start cooking. So there is no necessity to detect vessel again

                    // Now, check if the mass of the food item is already within 50g variation in the database
                    // if yes, then there is no necessity to predict modes from the cloud.
//                    float mass = Firebase.getFloat(path + String("mass"));
                    float mass = 50;
                    
//                    ml[0].predict(food_name, mass, burner_size, "onoffTime");
//                    ml[1].predict(food_name, mass, burner_size, "order0");
//                    ml[2].predict(food_name, mass, burner_size, "order1");
//                    ml[3].predict(food_name, mass, burner_size, "order2");
//                    ml[4].predict(food_name, mass, burner_size, "order3");

                    // after gathering the predictions, push them to firebase
//                    ML_model :: pushPredictedToFirebase(food_name);

//                    Serial.println("path is.." + String("/aaa/") + String("Food_profile/") + food_name + String("/steps"));
                    FirebaseObject modesIntervalsNode = Firebase.get(String("/aaa/") + String("Food_profile/") + food_name + String("/steps"));
                    burners[burnerNumber].setModesIntervalsNode(modesIntervalsNode);
                    JsonObject& modesIntervals = (burners[burnerNumber].getModesIntervalsNode()).getJsonVariant();
                    
                    String frstMode;
                    for(JsonPair& nobj : modesIntervals) {
                        Serial.println("inside");
                        // But the key is actually stored in RAM (in the JsonBuffer)
                        Serial.println((String)nobj.key);
                        JsonObject& obj = nobj.value;
                        bool found = false;
                        for(JsonPair& kvp : obj){
                            frstMode = kvp.key;
                            Serial.println("First mode mm is:");
                            Serial.println(frstMode);
                            found = true;
                            break;
                             
                        }
                        if(found) break;
                    }
                    Serial.println("First mode is:" + frstMode);
                    boolean ignited = burners[burnerNumber].turnONandWaitForIgnition(frstMode);
                    if(ignited){
                        burners[burnerNumber].initCooking();
                        
                    }
                    
                    
                } else if(!knob_status.equalsIgnoreCase("OFF") && data == 0){
                    // the request is to switch off the burner
                    Serial.println("Need to stop!");

                    int burnerNumber = (burner.substring(6).toInt()) - 1;
                    burners[burnerNumber].next  = "OFF";
                    
                } 
                Serial.print("data: ");
            }
            // if the user wants to continue cooking
            else if(path.substring(8).equalsIgnoreCase("/continue")){
                int data = event.getInt("data");
                if(data == 1){
                    String burner = path.substring(1,8);
                    int burnerNumber = (burner.substring(6).toInt()) - 1;
                    burners[burnerNumber].continueCooking();
                }
            }
            // if the user was satisfied with the cooking and whether the data can be used for retraining the model.
            else if(path.substring(8).equalsIgnoreCase("/retrain")){
                int data = event.getInt("data");
                if(data == 1){
                    Serial.println("Initiating to retrain..");
                    String burner = path.substring(1,8);
                    int burnerNumber = (burner.substring(6).toInt()) - 1;

//                    ML_model :: getFeedbackData(burners[burnerNumber]);
//                    ml[0].postFeedbackData(burners[burnerNumber], "onoffTime");
//                    ml[1].postFeedbackData(burners[burnerNumber], "order1");
//                    ml[2].postFeedbackData(burners[burnerNumber], "order2");
//                    ml[3].postFeedbackData(burners[burnerNumber], "order3");
//                    ml[4].postFeedbackData(burners[burnerNumber], "order4");

                    // after retraining set retrain = false;
                    Firebase.setBool(path, false);
                    
                }
            }
        }
    }
    delay(1000);
}

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcutest-641e9.firebaseio.com"
#define FIREBASE_AUTH "981Hw0F0SbDQnf3nr4AZnyzKPgYUeEjEiRTQk6cC"
#define WIFI_SSID "TP-Link_D322"
#define WIFI_PASSWORD "aqaiyoom123"

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

long startMillis = 0;
boolean start = true;

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

    
    // initialise firebase connection
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

int getDegree() {
    
    val = analogRead(potPin);    // read the value from the sensor

    // deg = map(val, 150, 820, 180, 0);
    deg = map(val, 136, 840, 180, 0);
    
    if (deg < 0) {
        deg = 0;
    } else if (deg > 180) {
        deg = 180;
    }
    
    return deg;
}

void compareAndRotate(int prevIndex, String next) {
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
int setDirAndRotate(String prev, String next) {

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

void rotate(int deg, int dir) {

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

int onEndTime = 15000;
int simEndTime = 17000;

void loop() {

    unsigned long diff = millis() - startMillis;
    
    
    
    if (start == true) {
    
        Serial.println(3);
        prev = "OFF";
        next = "ON";
        setDirAndRotate(prev, next);
        
        startMillis = millis();
        
        start = false;
        prev = "ON";
    
    } else if (diff >= simEndTime) {
    
        next = "OFF";
        setDirAndRotate(prev, next);
        prev = "OFF";
        
    
    } else if (diff >= onEndTime) {
    
        next = "SIM";
        setDirAndRotate(prev, next);
        prev = "SIM";
        
    }
    
    deg = getDegree();
    
    if (abs(prevDeg - deg) > 40) {
        // wait for 2 seconds and read again for a stable value
        delay(2000);
    
        deg = getDegree();
        prevDeg = deg;
        
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
        setDirAndRotate(prev, next);
        
        unsigned long diff1 = millis() - startMillis;
        if (diff1 <= onEndTime) {
        
            int prevOnEndTime = onEndTime;
            onEndTime = diff1;
            
            int redByAmt = prevOnEndTime - onEndTime;
            
            if (next == "SIM") {
            
                // reduce finish time of all subsequent steps, as the previous step has stopped early
                simEndTime -= redByAmt;
                Serial.println(redByAmt);
            }
            // else if it is a new step
            else {
                int factor = 2;
                // change finish time of all subsequent steps, as the previous step has stopped early
                Serial.println(redByAmt/1000);
                simEndTime = simEndTime - redByAmt + (redByAmt * factor);
                Serial.println(simEndTime/1000);
                prev = "ON-OFF";
            }

        
        } else if (diff1 <= simEndTime) {
            int prevSimEndTime = simEndTime;
            simEndTime = diff1;
            
            int redByAmt = prevSimEndTime - simEndTime;
            
            if (next == "OFF") {
                //                simEndTime -= redByAmt
                ;
            } else {
                int factor = 3;
                // change finish time of all subsequent steps, as the previous step has stopped early
                //                simEndTime = simEndTime - redByAmt + redByAmt*factor;
            }
        
        
        }
        
        prev = next;
    }

  //    delay(2000);
}

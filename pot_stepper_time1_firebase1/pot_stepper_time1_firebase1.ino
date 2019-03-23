#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcutest-641e9.firebaseio.com"
#define FIREBASE_AUTH "981Hw0F0SbDQnf3nr4AZnyzKPgYUeEjEiRTQk6cC"
#define WIFI_SSID "TP-Link_D322"
#define WIFI_PASSWORD "aqaiyoom123"

void setup() {
  Serial.begin(9600);

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
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if(Firebase.success()){
    Serial.println("connection made"); 
  }

  String check;

  // wait until the app has initialised the user's information
  do{
    check = Firebase.getString("/aaa/model");
  }while(Firebase.failed() || check.equalsIgnoreCase(""));
  
  
  Firebase.stream("/aaa");
  if(Firebase.success()){
    Serial.println("streaming success"); 
  } else {
    Serial.println(Firebase.error());
  }
}

int n = 0;

void loop() {

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
            int data = event.getInt("data");
            String knob_status = Firebase.getString("/aaa/" + burner + "/knob_status");
            if(knob_status.equalsIgnoreCase("OFF") && data == 1){
                // the request is to switch on the burner
                Serial.println("Need to start!");
                
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

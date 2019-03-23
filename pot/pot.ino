int potPin = A0;    // select the input pin for the potentiometer
//int ledPin = 13;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor
int deg = 0;       // variable to store the value coming from the sensor
int prevDeg = 0;
int pos = 0;


int positions[] = {0, 45, 90, 135, 180};
String modes[] = {"OFF", "ON-OFF", "ON", "ON-SIM", "SIM"};
    
void setup() {
    Serial.begin(9600);
//    pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
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
void loop() {
    
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
    }
    
    delay(2000);
}

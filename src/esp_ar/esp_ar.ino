#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

//#define FIREBASE_HOST "vann-53570.firebaseio.com"                // ten host cua firebase
//#define FIREBASE_AUTH "LC6fDYVqNO7VVBfoymADdWtLZVl6jr6WVJxhONrF" // ma dôi cái này nữa

#define FIREBASE_HOST "smarthome-9998-default-rtdb.firebaseio.com" // ten host cua firebase
#define FIREBASE_AUTH "igiAWMhz9g1vKbBoEqJN3Vqymk80G1CZ8YOCS7X5" // ma dôi cái này nữa

#define WIFI_SSID "BLVCK"          //cái này nữa
#define WIFI_PASSWORD "88888888" // đây nữa
FirebaseData firebaseData;       // firebase datb
FirebaseJson json;
FirebaseJson json2;
// khai baos cac chan cam bien
#define BUZZER_PIN 25
#define BUZZER_CHANNEL 0
// khai bao chan cam bien
int Gas_analog = 15;
int Gas_digital = 2;
int Fire_analog = 4;
int Fire_digital = 23;

#define DHTPIN 5
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//Khai bao ngoai vi
int ledred = 33;
int ledgr = 32;

long last = 0;

String mac;

// dia chi id cua tb 24:62:AB:F9:22:0C
void setup(void)
{

  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  dht.begin();
  last = millis();
  mac = WiFi.macAddress();
  Serial.println(mac);
}
void loop()
{
  // if (millis() - last >= 1000)
  // {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int fire = digitalRead(Fire_digital);
    int gas = digitalRead(Gas_digital);
    Serial.println(gas);
  
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light", 0);
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Fan", 0);
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light2", 0);
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light3", 0);
    Firebase.setFloat(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/temperature", t);
     Firebase.setFloat(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/humidity", h);
      Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/gas", gas);
        Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/fire", fire);
   // Firebase.pushJSON(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor", json);
    //Firebase.intJSON(firebaseData, "/Home/24:62:AB:F9:22:0C/Device", json2);
  //   last = millis();
  // }
}


#define btn 36
bool isP = false;
void IRAM_ATTR isr(){
  digitalWrite(tb1,HIGH);
  isP =true;
}
void setup(){
  Serial.begin(9600);
  pinMode(btn,INPUT_PULLUP);
  attachInterrupt(btn,isr,FALLING);

}
void loop(){
  is(isP){
    digitalWrite(tb1,LOW);
    isP = false;
  }
}

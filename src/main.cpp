#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#define FIREBASE_HOST "vann-53570.firebaseio.com"                // ten host cua firebase
//#define FIREBASE_AUTH "LC6fDYVqNO7VVBfoymADdWtLZVl6jr6WVJxhONrF" // ma dôi cái này nữa
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define FIREBASE_HOST "smarthome-9998-default-rtdb.firebaseio.com" // ten host cua firebase
#define FIREBASE_AUTH "igiAWMhz9g1vKbBoEqJN3Vqymk80G1CZ8YOCS7X5"   // ma dôi cái này nữa

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

FirebaseData firebaseData; // firebase datb
FirebaseJson json;
FirebaseJson json2;
// khai baos cac chan cam bien

// khai bao chan cam bien
int Gas_digital = 2;
int Fire_digital = 0;

#define DHTPIN 4
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//Khai bao ngoai vi

long last = 0;

String mac;

void dhtprint();
void hello();
void time_date();
void btn_tb1();
void btn_tb2();
void btn_tb3();
void btn_tb4();
void get_data();
void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

float t, h;
int a1, b1, c1, d1 = 0;
// thiet bi
int tb1 = 32;
int tb2 = 33;
int tb3 = 25;
int tb4 = 26;
int tb5 = 27;
int tb6 = 14;
int tb7 = 12;
int tb8 = 13;
int tb9 = 23;
int tb10 = 19;
int tb11 = 18;
int tb12 = 5;
int tb13 = 17;
// btn
#define btn1 36
#define btn2 39
#define btn3 34
#define btn4 35

int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

// dia chi id cua tb 24:62:AB:F9:22:0C
void setup(void)
{

  pinMode(tb1, OUTPUT);
  pinMode(tb2, OUTPUT);
  pinMode(tb3, OUTPUT);
  pinMode(tb4, OUTPUT);
  pinMode(tb5, OUTPUT);
  pinMode(tb6, OUTPUT);
  pinMode(tb7, OUTPUT);
  pinMode(tb8, OUTPUT);
  pinMode(tb9, OUTPUT);
  pinMode(tb10, OUTPUT);
  pinMode(tb11, OUTPUT);
  pinMode(tb12, OUTPUT);
  pinMode(tb12, OUTPUT);

  // btn
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  attachInterrupt(btn1, btn_tb1, RISING);
  // attachInterrupt(digitalPinToInterrupt(btn1), btn_tb1, CHANGE);
  attachInterrupt(btn2, btn_tb2, RISING);
  attachInterrupt(btn3, btn_tb3, RISING);
  attachInterrupt(btn4, btn_tb4, RISING);

  WiFi.mode(WIFI_STA);
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  hello();
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("SmartHome"); // đặt tên wifi // nếu đặt có mật khẩu sẽ có dạng sau  // res = wm.autoConnect("Smarthome","password"); // password protected ap
  hello();
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    //if you get here you have connected to the WiFi
    Serial.println("Connected...yeey :)");
  }

  timeClient.begin();
  timeClient.setTimeOffset(+7 * 60 * 60);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();
  last = millis();
}

void loop()
{

  get_data();
  h = dht.readHumidity();
  t = dht.readTemperature();
  int gass = digitalRead(Gas_digital);

  dhtprint();

  // Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light", 0);
  // //Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Fan", 0);
  // Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light2", 0);
  // Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light3", 0);
  Firebase.setFloat(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/temperature", t);
  Firebase.setFloat(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/humidity", h);
  Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Sensor/gas", gass);

  //Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light2", state3);
  //  Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/Light3", state4);
  // đồng bộ nút bấm
  if (a1 == 1)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/fan", 1);
  }
  if (a1 == 2)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/fan", 0);
    a1 = 0;
  }
  // tb2
  if (b1 == 1)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/light", 1);
  }
  if (b1 == 2)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/light", 0);
    b1 = 0;
  }
  // tb3
  if (c1 == 1)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/light_bulb", 1);
  }
  if (c1 == 2)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/light_bulb", 0);
    c1 = 0;
  }
  // tb4
  if (d1 == 1)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/outlet", 1);
  }
  if (d1 == 2)
  {
    Firebase.setInt(firebaseData, "/Home/24:62:AB:F9:22:0C/Device/outlet", 0);
    d1 = 0;
  }

  if (millis() - last >= 100)
  {

    last = millis();
  }
}

void dhtprint()
{
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %");

  display.display();
}

void hello()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.print("DATN DHK");
  display.setTextSize(2);
  display.setCursor(35, 30);
  display.print("IT58");

  display.setTextSize(0.5);
  display.setCursor(35, 50);
  display.print("Disconnect");
  display.display();
}
void time_date()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.print(timeStamp);
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print(dayStamp);
  // connected
  display.setTextSize(0.5);
  display.setCursor(35, 50);
  display.print("Connected");
  display.display();
}
void btn_tb1()
{
  digitalWrite(tb1, !digitalRead(tb1));
  if (digitalRead(tb1) == 0)
  {

    a1 = 2;
  }
  if (digitalRead(tb1) == 1)
  {

    a1 = 1;
  }
}
void btn_tb2()
{
  digitalWrite(tb2, !digitalRead(tb2));
  if (digitalRead(tb2) == 0)
  {

    b1 = 2;
  }
  if (digitalRead(tb2) == 1)
  {

    b1 = 1;
  }
}

void btn_tb3()
{
  digitalWrite(tb3, !digitalRead(tb3));
  if (digitalRead(tb3) == 0)
  {

    c1 = 2;
  }
  if (digitalRead(tb3) == 1)
  {

    c1 = 1;
  }
}

void btn_tb4()
{
  digitalWrite(tb4, !digitalRead(tb4));
  if (digitalRead(tb4) == 0)
  {

    d1 = 2;
  }
  if (digitalRead(tb4) == 1)
  {

    d1 = 1;
  }
}

void get_data()
{

  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/fan"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("L1: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb1, LOW);
        a1 = 0;
      }
      if (g1 == 1)
      {
        digitalWrite(tb1, HIGH);
        a1 = 0;
      }
      Serial.print("st");
      Serial.println(state1);
    }
  }

  // light
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/light"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb2: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb2, LOW);
        b1=0;
      }
      if (g1 == 1)
      {
        digitalWrite(tb2, HIGH);
           b1=0;
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/light_bulb"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb3: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb3, LOW);
        c1=0;
      }
      if (g1 == 1)
      {
        digitalWrite(tb3, HIGH);
        c1=0;
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/outlet"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb4: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb4, LOW);
        d1=0;
      }
      if (g1 == 1)
      {
        digitalWrite(tb4, HIGH);
        d1=0;
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/air_conditioner"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb5: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb5, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb5, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_1"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb6: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb6, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb6, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_2"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb7: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb7, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb7, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_3"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb8: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb8, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb8, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_4"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb9: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb9, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb9, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_5"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb10: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb10, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb10, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_6"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb11: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb11, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb11, HIGH);
      }
    }
  }
  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_7"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("tb12: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb12, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb12, HIGH);
      }
    }
  }

  //
  if (Firebase.getInt(firebaseData, "Home/24:62:AB:F9:22:0C/Device/device_8"))
  {
    if (firebaseData.dataType() == "int")
    {
      int g1 = firebaseData.intData();
      Serial.print("t13: ");
      Serial.println(g1);
      if (g1 == 0)
      {
        digitalWrite(tb13, LOW);
      }
      if (g1 == 1)
      {
        digitalWrite(tb13, HIGH);
      }
    }
  }
}
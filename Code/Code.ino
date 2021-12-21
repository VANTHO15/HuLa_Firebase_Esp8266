#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define FIREBASE_HOST "https://phaohoa-6ef19-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "926krZsrFgqWbS6tAA8S6scRHHdbA6bLgiSlnYOp"
#define WIFI_SSID "Van Tho 15"
#define WIFI_PASSWORD "vannhucu15"

FirebaseData firebaseData;
String path = "/";
long long last = 0;
String Led = "OFF";
int NhietDo = 28;

void ReadDataToFirebase();
void WriteDataToFirebase();
void HandleLed();

void setup()
{
  Serial.begin(9600);
  pinMode(16, OUTPUT);
  digitalWrite(16,1);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData, path))
  {
    Serial.println("REASON:+ " + firebaseData.errorReason());
    Serial.println();
  }
  last = millis();
}

void loop()
{
  WriteDataToFirebase();
  ReadDataToFirebase();
}
void ReadDataToFirebase()
{
  if (Firebase.getString(firebaseData, path + "/Led"))
  {
    Led = firebaseData.stringData();
    HandleLed();
  }
}
void WriteDataToFirebase()
{
  if (millis() - last > 500)
  {
    NhietDo = random(30, 80);
    Firebase.setString(firebaseData, path + "/NhietDo", String(NhietDo));
    last = millis();
  }
}
void HandleLed()
{
  if(Led =="ON")
  {
    Serial.println("LED ON");
    digitalWrite(16,0);
  }
  else
  if(Led =="OFF")
  {
    Serial.println("LED OFF");
    digitalWrite(16,1);
  }
}

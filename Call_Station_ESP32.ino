#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#define FIREBASE_HOST "agv-jinpao.firebaseio.com"
#define FIREBASE_AUTH "BIRqS7JqvfderbOUVpBETvbYgYn3GEZLW74Q0s2m"
#define WIFI_SSID "NoThing"
#define WIFI_PASSWORD "0000000000"

int STATION= 3;
int now_station,next_station,count=0;
unsigned long previousMillis = 0;
const long interval = 500;
int button = 23,led=22;

void setup() {
  Serial.begin(9600);
  pinMode(button,INPUT_PULLUP);
  pinMode(led,OUTPUT);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    count++;
    if(count>=40){
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  now_station = Firebase.getInt("/position/now_position");
  next_station = Firebase.getInt("/position/next_position");

  Firebase.stream("/position", [](FirebaseStream stream) {
   if(stream.getPath() == "/now_position") {
    now_station = stream.getDataInt();
    Serial.print("now_station: ");
    Serial.println(now_station);
  }
  if(stream.getPath() == "/next_position") {
    next_station = stream.getDataInt();
    Serial.print("next_station: ");
    Serial.println(next_station);
  }
  });
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(now_station==STATION){
      digitalWrite(led,HIGH);
    }
    if((next_station==STATION)&&(now_station!=STATION)){
      digitalWrite(led,!digitalRead(led));
    }
    if(next_station!=STATION){
      digitalWrite(led,LOW);
    }
  }
  if(digitalRead(button)==LOW){
    while(digitalRead(button)==LOW){};
    Serial.println("call now");
    Firebase.setInt("/position/next_position", STATION);
  }
  
}

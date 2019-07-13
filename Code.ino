#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
 
#define sensor1 D2
#define sensor2 D1
 
#define FIREBASE_HOST "link to firebase real-time database"
#define FIREBASE_AUTH "your secret key"
#define WIFI_SSID "your wifi ssid"
#define WIFI_PASSWORD "your password"
 
int count, avail, cap;
void availUpdate()
{
  avail = cap - count;
  Firebase.setInt("Availability",avail);  		//SET THE INTEGER VALUE LOCATED AT THE PATH
  Serial.println(avail);               
}
 
void setup() {
  Serial.begin(9600);                   		 //9600 BITS PER SECOND
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 	 //INITIALISES THE WIFI LIBRARY SETTINGS                            
  while (WiFi.status() != WL_CONNECTED) 
{	
    delay(500);
  }
  ESP.wdtDisable();                   		 //DISABLES THE SOFTWARE WATCHDOG
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  Serial.println("Start");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop()
{
  ESP.wdtFeed();                         		 //RESTART WATCHDOG
  cap = Firebase.getInt("Capacity");     		//GET THE INTEGER VALUE
  if (digitalRead(sensor1) == HIGH) {
    while (digitalRead(sensor2) == LOW) {
      ESP.wdtFeed();
    }
    Serial.println("in");
    if (count < cap)
    {
      count++;
    }
    availUpdate();
  } else if (digitalRead(sensor2) == HIGH) {
    while (digitalRead(sensor1) == LOW) {
      ESP.wdtFeed();
    }
    Serial.println("out");
    if (count >0)
    {
      count--;
    }
    availUpdate();
  }
}

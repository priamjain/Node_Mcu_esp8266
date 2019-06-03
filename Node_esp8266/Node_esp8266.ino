#include <FirebaseError.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <Firebase.h>

#include <ESP8266WiFi.h>;
#include <ESP8266WebServer.h>
#define LED_BUILTIN 2
String fireStatus = ""; 
 #define FIREBASE_HOST "led-blink-0909.firebaseio.com"             
#define FIREBASE_AUTH "OvI15RqPtxQIG3XzxFyyXK5XLH12QFf4WUWbpCF7"
int flag=0;
String m_no;
const char *ssid = "MyESP8266AP";
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
 
<h2>Enter Username & Password of wifi<h2>
<h3> HTML Form ESP8266</h3>
 
<form action="/action_page" method="get">
  ID:<br>
  <input type="text" name="ID" >
  <br>
  Password:<br>
  <input type="password" name="Password" >
  <br>
  Model No:<br>
  <input type="text" name="m_no" >
  <br><br>
  <input type="submit" value="Submit">
</form> 
 
</body>
</html>
)=====";

 
ESP8266WebServer server(80);
void handleRoot() {
 String s = MAIN_page; 
 server.send(200, "text/html", s);
}
void handleForm() {
 String ID = server.arg("ID"); 
 String Password = server.arg("Password"); 
 m_no= server.arg("m_no");
 Serial.print("ID:");
 Serial.println(ID);
 
 Serial.print("Password:");
 Serial.println(Password);
  Serial.print("Model Number:");
 Serial.println(m_no);
 
 String s = "<a href=/> Go Back </a>";
 server.send(200, "text/html", s); 
  WiFi.begin(ID, Password); 
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN,HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN,LOW);
    delay(200);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                         
  Firebase.setString(m_no, "OFF");   
  flag=1;
  
}

 
void setup() {
 pinMode(LED_BUILTIN, OUTPUT);     
  Serial.begin(115200);
  WiFi.softAP(ssid);
 
  Serial.println();
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
 
  server.on("/", handleRoot);
  server.on("/action_page", handleForm);
  server.begin();
  Serial.println("Server listening");

}
 
void loop() {
  server.handleClient();
  if(flag==1)
  {
  Serial.println(m_no);
  fireStatus = Firebase.getString(m_no); // get ld status input from firebase
  Serial.println(fireStatus);
  if (fireStatus == "ON") {                                                          
    Serial.println("Led Turned ON");                         
    digitalWrite(LED_BUILTIN, LOW);                                                  
    } 
  else if (fireStatus == "OFF") {                                
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);                                               
  }
  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
  }     
}

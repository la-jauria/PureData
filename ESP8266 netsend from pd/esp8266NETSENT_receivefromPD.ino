/* 
 *  Receive UDP Packets from Pure Data with [netsend]
 *  
 *  Connect the ESP8266 to the "linksys" Wi-Fi router
 * 
 */
 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
const boolean DEBUG = false;
 
// WIFI

char ssid[] = "EspacioTiempo";          // your network SSID (name)
char pass[] = "244466666";           // your network password

IPAddress ip(192, 168, 1, 200);  // Local IP   
IPAddress gateway(192, 168, 1, 1); // Router IP
IPAddress subnet(255, 255, 255, 0);
//...
 

char packetBuffer[255]; // incoming
String data[3]; // data
int status = WL_IDLE_STATUS;
WiFiUDP udp;


const unsigned int localPort = 8266;  // port
 
void setup() {
 
  // Output

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);    // turn *on* led


  // WiFi Connection

  WiFi.config(ip, gateway, subnet); // Choose the static IP Address
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
  }
 
  // UDP Connection
  udp.begin(localPort);
 
  if (DEBUG) 
  {
    Serial.begin(115200);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
}
 
void loop() {
 
  // Read data if available
  int packetSize = udp.parsePacket();
  if (packetSize) 
  {
    // Read the packet into packetBuffer
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
 
    // Parse with space delimiter ' ' and fill data[]
    String strData(packetBuffer);
    splitString(strData, ' ');
 
    analogWrite(data[0].toInt(), data[1].toInt() * 4);
 
    if (DEBUG) 
    {
      Serial.println(data[0]);
      Serial.println(data[1]);
     // Serial.println(data[2]);
    }
  }
}
 
 
// Méthode pour découper le message avec un séparateur (ou "parser")
void splitString(String message, char separator) {
  int index = 0;
  int cnt = 0;
    do {
      index = message.indexOf(separator); 
      // s'il y a bien un caractère séparateur
      if(index != -1) { 
          // on découpe la chaine et on stocke le bout dans le tableau
          data[cnt] = message.substring(0,index); 
          cnt++;
          // on enlève du message le bout stocké
          message = message.substring(index+1, message.length());
      } else {
         // après le dernier espace   
         // on s'assure que la chaine n'est pas vide
         if(message.length() > 0) { 
           data[cnt] = message.substring(0,index); // dernier bout
           cnt++;
         }
      }
   } while(index >=0); // tant qu'il y a bien un séparateur dans la chaine
}

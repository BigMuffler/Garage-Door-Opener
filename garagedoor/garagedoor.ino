#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "";      // your network SSID (name) 
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

String readString;
String header;
int garageDoor = 5; 
String outputState = "Closed";
WiFiServer server(80);

void setup() {
   pinMode(garageDoor, OUTPUT);
   digitalWrite(garageDoor, LOW);
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        readString += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
             // send a standard http response header
             client.println("HTTP/1.1 200 OK");
             client.println("Content-Type: text/html");
             client.println();
  
             if (readString.indexOf("GET /5/on") >= 0)
             {
                 Serial.println("Pin 5 is open");
                 outputState = "Open";
                 digitalWrite(garageDoor,LOW);
                 delay(1000);
                 digitalWrite(garageDoor,HIGH);
             }else if (readString.indexOf("GET /5/off") >= 0) {
                 Serial.println("Pin 5 is off");
                 outputState = "Open";
                 digitalWrite(garageDoor, LOW);
                 delay(1000);
                 digitalWrite(garageDoor,HIGH);
             } 
  
             client.println("<!DOCTYPE html><html>");
             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
             client.println("<link rel=\"icon\" href=\"data:,\">");
        
             client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
             client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
             client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
             client.println(".button2 {background-color: #77878A;}</style></head>");
             client.println("<h1>Garage Door Opener</h1>");
  
              if (outputState=="Closed") {
                client.println("<p><a href=\"/5/on\"><button class=\"button\">OPEN</button></a></p>");
              } 
              else {
                client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OPEN</button></a></p>");
              } 
             client.println("</BODY>");
             client.println("</html>");
             break;
          }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    readString = "";
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

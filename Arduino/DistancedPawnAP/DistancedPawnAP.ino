/**
  \file DistancedPawn.ino
  \brief Main application for "The Distanced Pawn" project, AP module

  This software is developed to run on the Arduino MKR1010 connected to a second similad micro controller.
  The AP implement a web server to whch the second chess player (the remote opponent) connects 
  implementing a web client.

  \author Enrico Miglino <balearidcynamics@gmail.com>
  \version 1.0 build 5
 */

#include <SPI.h>
#include <WiFiNINA.h>
#include <Streaming.h>

#include "server_params.h"

//! #undef below to stop serial debugging info (speedup the system and reduces the memory)
#define _DEBUG

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

//! AP server status
int status = WL_IDLE_STATUS;
//! Create the http server on custom port
WiFiServer server(SERVER_PORT);

/** 
 *  Initialization function.
 *  
 *  In the setup funciton it is created the AP and assigned the default IP
 *  address, as well as the server creation. Only in debug mode (serial active)
 *  the setting operations are logged to the terminal.
 *  
 *  \note To manage the status when the AP can't be initializaed or there is a connection
 *  issue, the builting LED goes not to On
*/
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

#ifdef _DEBUG
  Serial.begin(9600);
  Serial.println("Access Point Web Server");

  // Check the firmware version and notify if it should be updated
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
#endif

   WiFi.config(IPAddress(IP(0), IP(1), IP(2), IP(3)) );

#ifdef _DEBUG
  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
#endif

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING) {
#ifdef _DEBUG
    Serial.println("Creating access point failed");
#endif
    // don't continue
    while (true);
  }

  // wait for connection
  delay(AP_DELAY);

  // start the web server on the assigned port
  server.begin();

  //! System is ready
  digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on

  // you're connected now, so print out the status
  printWiFiStatus();
}

//! Main appplication function. Focused on the server activity
void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

#ifdef _DEBUG
    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
#endif
  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
//        if (currentLine.endsWith("GET /H")) {
//          digitalWrite(led, HIGH);               // GET /H turns the LED on
//        }
//        if (currentLine.endsWith("GET /L")) {
//          digitalWrite(led, LOW);                // GET /L turns the LED off
//        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

#ifdef _DEBUG
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
#endif

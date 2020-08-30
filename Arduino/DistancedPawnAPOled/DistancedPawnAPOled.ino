/**
  \file DistancedPawn.ino
  \brief Main application for "The Distanced Pawn" project, AP module

  This software is developed to run on the Arduino MKR1010 connected to 
  a second similad micro controller.
  The AP implement a web server to whch the second chess player 
  (the remote opponent) connects implementing a web client.
  Starting from build 8 I have integrated the OLED I2C display

  \author Enrico Miglino <balearidcynamics@gmail.com>
  \version 1.0 build 8
 */

#include <SPI.h>
#include <WiFiNINA.h>
#include <Streaming.h>

#include "oledsettings.h"
#include "server_params.h"
#include "chess_moves.h"

#define PIN_R 3
#define PIN_G 4
#define PIN_B 5

//! #undef below to stop serial debugging info (speedup the system and reduces the memory)
#define _DEBUG

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

//! AP server status
int status = WL_IDLE_STATUS;
//! Create the http server on custom port
WiFiServer server(SERVER_PORT);

//! Create the board object
Board chessBoard;

//! Dispaly instance
//! Display size is not parametrized as it is specifically related
//! to the used hardware.
Adafruit_SSD1306 oled = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);

//! Test counter to change the displayed text
float testCount = 0;

/**
 * Debug function. The function works only if _DEBUG is defined, else do
 * nothing.
 * The message is shown on the Serial1 device with line-terminated character.
 * 
 * \param The debug string
 */
void sDebug(String msg) {
#ifdef _DEBUG
  Serial1.print("debug: ");
  Serial1.println(msg);
#endif
}

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

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  digitalWrite(PIN_R,HIGH);
  digitalWrite(PIN_G,LOW);
  digitalWrite(PIN_B,LOW);
  delay(2000);
  digitalWrite(PIN_R,LOW);
  digitalWrite(PIN_G,HIGH);
  digitalWrite(PIN_B,LOW);
  delay(2000);
  digitalWrite(PIN_R,LOW);
  digitalWrite(PIN_G,LOW);
  digitalWrite(PIN_B,HIGH);
  delay(2000);
  digitalWrite(PIN_R,LOW);
  digitalWrite(PIN_G,LOW);
  digitalWrite(PIN_B,LOW);

  pinMode(LED_BUILTIN, OUTPUT);

#ifdef _DEBUG
  Serial1.begin(115200);
#endif
  sDebug("Access Point Web Server");

  // Check the firmware version and notify if it should be updated
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    sDebug("Please upgrade the firmware");
  }

   WiFi.config(IPAddress(IP(0), IP(1), IP(2), IP(3)) );

  // print the network name (SSID);
  sDebug("Creating access point named: ");
  sDebug(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING) {
    sDebug("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait for connection
  delay(AP_DELAY);

  // start the web server on the assigned port
  server.begin();

  //! System is ready
  digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on

  printWiFiStatus();

  chessBoard.setBoard();
  chessBoard.drawBoard(BOARD_SERIAL);

  // Initialize the display
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C);
  sDebug("OLED initialized");

  // Clear the buffer.
  oled.clearDisplay();
  oled.display();
  sDebug("Buffer cleared. Starting Fonts test");

  // Show title scrolling on the display
  initDisplay(&oled); 
  textFont(SANS_BOLD, 9, &oled);
  showText("The", 45, 15, COL_WHITE, &oled); 
  showText("Distanced", 20, 35, COL_WHITE, &oled); 
  showText("Pawn", 35, 55, COL_WHITE, &oled); 
}

//! Main appplication function. Focused on the server activity
void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      sDebug("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      sDebug("Device disconnected from AP");
    }
  }
  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial1.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial1.write(c);                    // print it out the serial monitor
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
    sDebug("client disconnected");
  }
}

//! Debug onlly
void printWiFiStatus() {
#ifdef _DEBUG
  // print the SSID of the network you're attached to:
  Serial1 << "SSID: " << WiFi.SSID() << endl;
  IPAddress ip = WiFi.localIP();
  Serial1 << "IP Address: " << ip << endl;
  Serial1 <<"To see this page in action, open a browser to http://" <<
          ip << endl;
#endif
}

// =========================================================
//                      Oled functions
// =========================================================

/**
 * Show the text string at the desired coordinates.
 * 
 * The text is shown according to the current settings (color, font, etc.)
 * 
 * \param text The string of text to display
 * \param x The x cursor coordinates
 * \param y The y cursor coordinates
 * \param color The color of the text
 * \param disp Pointer to the Oled display class
 */
void showText(char* text, int x, int y, int color, Adafruit_SSD1306* disp) {
  // Set the desired color
  switch(color) {
    case COL_WHITE:
    oled.setTextColor(SSD1306_WHITE);
    break;
  }
  disp->setCursor(x, y);
  disp->print(text);
  disp->display();
}

/**
 * Initialize the display before showing a new screen.
 * 
 * \note This method should be called for first before setting a new
 * string or when the screen setting changes.
 * 
 * \param disp Pointer to the Oled display class
 */
void initDisplay(Adafruit_SSD1306* disp) {
  disp->clearDisplay();
  disp->display();
}

/**
 * Start the text scrolling in the desired direction.
 * The scrolled text is what has already been composed
 * and shown on the screen.
 * 
 * Start this method after the text screen has been set
 * 
 * \param Dir the scrolling direction
 * \param disp Pointer to the Oled display class
 */
void textScroll(int dir, Adafruit_SSD1306* disp) {
  switch(dir) {
    case OLED_SCROLL_LEFT_RIGHT:
      disp->startscrollright(0x00, 0x0F);
    break;
    case OLED_SCROLL_RIGHT_LEFT:
      disp->startscrollleft(0x00, 0x0F);
    break;
    case OLED_SCROLL_DIAG_RIGHT:
      disp->startscrolldiagright(0x00, 0x0F);
    break;
    case OLED_SCROLL_DIAG_LEFT:
      disp->startscrolldiagleft(0x00, 0x0F);
    break;
    case OLED_SCROLL_STOP:
      disp->stopscroll();
    break;
  }
}

/**
 * Set the desired font and size to the text.
 * 
 * The fonts have four predefined sizes: 9, 12, 18, 24 points
 * Any different value is ignored and the function do nothing
 * 
 * The selected font should be enabled in the fonts definition 
 * else the method set the default font and returns -1
 * 
 * \param fontName One of the following font families:\n
 * <ul>
 * <li>MONO
 * <li>MONO_BOLD
 * <li>MONO_OBLIQUE
 * <li>MONO_BOLD_OBLIQUE
 * <li>SERIF
 * <li>SERIF_BOLD
 * <li>SERIF_ITALIC
 * <li>SERIF_BOLD_ITALIC
 * <li>SANS
 * <li>SANS_OBLIQUE
 * <li>SANS_BOLD
 * <li>SANS_BOLD_OBLIQUE
 * </ul>
 * \param fontSize The size in point of the font chosen between one of the
 * following values: 9, 12, 18, 24
 * \param disp Pointer to the Oled display class
 * 
 * \return 0 if the font is set else return -1
 */
int textFont(int fontName, int fontSize, Adafruit_SSD1306* disp) {
  int j;

  // Check for the font size
  if( (fontSize != 9) && (fontSize != 12) &&
      (fontSize != 18) &&(fontSize != 24) ) {
        return -1;
      }

  // Switch the font family
  switch(fontName) {
    case MONO:
    #ifndef FREE_MONO
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeMono9pt7b);
      break;
      case 12:
        disp->setFont(&FreeMono12pt7b);
      break;
      case 18:
        disp->setFont(&FreeMono18pt7b);
      break;
      case 24:
        disp->setFont(&FreeMono24pt7b);
      break;
    }
    #endif
    break;

    case MONO_BOLD:
    #ifndef FREE_MONO_BOLD
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeMonoBold9pt7b);
      break;
      case 12:
        disp->setFont(&FreeMonoBold12pt7b);
      break;
      case 18:
        disp->setFont(&FreeMonoBold18pt7b);
      break;
      case 24:
        disp->setFont(&FreeMonoBold24pt7b);
      break;
    }
    #endif
    break;

    case MONO_OBLIQUE:
    #ifndef FREE_MONO_OBLIQUE
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeMonoOblique9pt7b);
      break;
      case 12:
        disp->setFont(&FreeMonoOblique12pt7b);
      break;
      case 18:
        disp->setFont(&FreeMonoOblique18pt7b);
      break;
      case 24:
        disp->setFont(&FreeMonoOblique24pt7b);
      break;
    }
    #endif
    break;

    case MONO_BOLD_OBLIQUE:
    #ifndef FREE_MONO_BOLD_OBLIQUE
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeMonoBoldOblique9pt7b);
      break;
      case 12:
        disp->setFont(&FreeMonoBoldOblique12pt7b);
      break;
      case 18:
        disp->setFont(&FreeMonoBoldOblique18pt7b);
      break;
      case 24:
        disp->setFont(&FreeMonoBoldOblique24pt7b);
      break;
    }
    #endif
    break;

    case SERIF:
    #ifndef FREE_SERIF
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSerif9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSerif12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSerif18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSerif24pt7b);
      break;
    }
    #endif
    break;

    case SERIF_BOLD:
    #ifndef FREE_SERIF_BOLD
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSerifBold9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSerifBold12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSerifBold18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSerifBold24pt7b);
      break;
    }
    #endif
    break;

    case SERIF_ITALIC:
    #ifndef FREE_SERIF_ITALIC
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSerifItalic9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSerifItalic12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSerifItalic18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSerifItalic24pt7b);
      break;
    }
    #endif
    break;

    case SERIF_BOLD_ITALIC:
    #ifndef FREE_SERIF_BOLD_ITALIC
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSerifBoldItalic9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSerifBoldItalic12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSerifBoldItalic18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSerifBoldItalic24pt7b);
      break;
    }
    #endif
    break;

    case SANS:
    #ifndef FREE_SANS
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSans9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSans12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSans18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSans24pt7b);
      break;
    }
    #endif
    break;

    case SANS_OBLIQUE:
    #ifndef FREE_SANS_OBLIQUE
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSansOblique9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSansOblique12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSansOblique18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSansOblique24pt7b);
      break;
    }
    #endif
    break;

    case SANS_BOLD:
    #ifndef FREE_SANS_BOLD
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSansBold9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSansBold12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSansBold18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSansBold24pt7b);
      break;
    }
    #endif
    break;

    case SANS_BOLD_OBLIQUE:
    #ifndef FREE_SANS_BOLD_OBLIQUE
    disp->setFont();
    return -1;
    #else
    switch(fontSize) {
      case 9:
        disp->setFont(&FreeSansBoldOblique9pt7b);
      break;
      case 12:
        disp->setFont(&FreeSansBoldOblique12pt7b);
      break;
      case 18:
        disp->setFont(&FreeSansBoldOblique18pt7b);
      break;
      case 24:
        disp->setFont(&FreeSansBoldOblique24pt7b);
      break;
    }
    #endif
    break;
  } 
}

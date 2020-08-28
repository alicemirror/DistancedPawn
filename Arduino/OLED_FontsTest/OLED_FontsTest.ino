/**
  \file OLED_FontsTestedPawn.ino
  \brief A full test of all the fonts available on the OLED Display
  
  This software is developed to run on the Arduino MKR1010, so some parameters
  may refer exclusively to this board.
  The OLED display used is a 128x64 pixels model connected to I2C

  \note The OLED display includes some scrolling text methods:

  startscrollright(0x00, 0x0F): scroll text from left to right\n
  startscrollleft(0x00, 0x0F): scroll text from right to left\n
  startscrolldiagright(0x00, 0x07): scroll text from left bottom\n
                                    corner to right upper corner\n
  startscrolldiagleft(0x00, 0x07): scroll text from right bottom corner\n
                                   to left upper corner

  \author Enrico Miglino <balearidcynamics@gmail.com>
  \version 1.0 build 1
 */

#include <Streaming.h>
#include "oledsettings.h"

// Undef below to remove the debug serial notifications
#undef _DEBUG

//! Dispaly instance
//! Display size is not parametrized as it is specifically related
//! to the used hardware.
Adafruit_SSD1306 oled = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);

//! Test counter to change the displayed text
float testCount = 0;

/**
 * Debug function. The function works only if _DEBUG is defined, else do
 * nothing.
 * The message is shown on the serial device with line-terminated character.
 * 
 * \param The debug string
 */
void sDebug(String msg) {
#ifdef _DEBUG
//  Serial << "debug: " << msg << endl;
  Serial.print("debug: ");
  Serial.println(msg);
#endif
}

/**
 * Initialization function
 */
void setup() {
#ifdef _DEBUG
  Serial.begin(9600);
  sDebug("Started");
#endif
  sDebug("128x64 OLED fonts test");
  // Initialize the display
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C);
  sDebug("OLED initialized");

  // Clear the buffer.
  oled.clearDisplay();
  oled.display();
  sDebug("Buffer cleared. Starting Fonts test");

}

// --- Defines to enable test blocks
#undef _SCROLL
#define _FONT

//! Main application loop
void loop() {
  char cnt[20];
  
  testCount += PI;

#ifdef _SCROLL
  sprintf(cnt, "Var %3.5f", testCount);
  // Scroll sequence
  initDisplay(&oled); 
  showText(cnt, 0, 0, COL_WHITE, &oled); 
  textScroll(OLED_SCROLL_LEFT_RIGHT, &oled); delay(2000);
  textScroll(OLED_SCROLL_RIGHT_LEFT, &oled); delay(2000);
  textScroll(OLED_SCROLL_DIAG_RIGHT, &oled); delay(2000);
  textScroll(OLED_SCROLL_DIAG_LEFT, &oled); delay(2000);
  textScroll(OLED_SCROLL_STOP, &oled); delay(2000);
#endif

#ifdef _FONT
  sprintf(cnt, ">%3.0f", testCount);
  initDisplay(&oled);
  textFont(SERIF, 9, &oled);
  showText(cnt, 20, 15, COL_WHITE, &oled); 
  textFont(SERIF_BOLD, 12, &oled);
  showText(cnt, 20, 40, COL_WHITE, &oled); 
  textFont(SERIF_ITALIC, 9, &oled);
  showText(cnt, 20, 62, COL_WHITE, &oled); 
  delay(3000);
  initDisplay(&oled);
  textFont(SERIF_BOLD, 18, &oled);
  showText(cnt, 20, 40, COL_WHITE, &oled); 
  delay(3000);
  initDisplay(&oled);
  textFont(SERIF_BOLD, 24, &oled);
  showText(cnt, 20, 40, COL_WHITE, &oled); 
  textScroll(OLED_SCROLL_RIGHT_LEFT, &oled);
  delay(3000);
  textScroll(OLED_SCROLL_STOP, &oled); delay(2000);
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

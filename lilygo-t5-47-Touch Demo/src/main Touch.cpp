#include <Arduino.h>                  // Arduino for PlatformIO

#include "Wire.h"                     // I2C Bus
#include "touch.h"                    // Touch Pad

// epd
#include "epd_driver.h"               // Display Driver (modi)
#include "epd_highlevel.h"            // ?



// Display ausrichtung
EpdRotation orientation =
EPD_ROT_INVERTED_PORTRAIT;            // Handgerät ausrichtung oben
//EPD_ROT_LANDSCAPE;                  // Handgerät ausrichtung rechts
//EPD_ROT_INVERTED_LANDSCAPE;         // Handgerät ausrichtung links
//EPD_ROT_PORTRAIT;                   // Handgerät ausrichtung unten

// font
#include "Firasans.h"                 // FiraSans_12
#include "opensans8b.h"               // OpenSans8B
#include "opensans9b.h"               // OpenSans9B
#include "opensans10b.h"              // OpenSans10B
#include "opensans14b.h"              // OpenSans14B
#include "opensans24b.h"              // OpenSans24B

// Farben
#define White                         0xFF
#define LightGrey                     0xBB
#define Grey                          0x88
#define DarkGrey                      0x44
#define Black                         0x00

// unsortiert
#define WAVEFORM EPD_BUILTIN_WAVEFORM // ?
EpdiyHighlevelState hl;               // ?
int temperature = 30;                 // Umgebungstemp Geräteeinstellung
EpdFont  currentFont;                 // ?
enum alignment {LEFT, RIGHT, CENTER}; // Textausrichtung
uint8_t *fb;                          // ?
uint16_t x, y ;                       // Touch Pad
TouchClass touch;                     // ?



// Button
#define BUTTON_0                       0
#define BUTTON_1                      34
#define BUTTON_2                      35
#define BUTTON_3                      39 

// Pinbelegung
#define BATT_PIN                      36
#define TOUCH_INT                     13           



///////////////////////////////////////// Deklaration //////////////////////////////////////////


void setFont(EpdFont const &font) {
  currentFont = font;
}

void drawString(int x, int y, String text, alignment align) {   
  char * data  = const_cast<char*>(text.c_str());
  EpdFontProperties font_props = epd_font_properties_default();
  if (align == CENTER) font_props.flags = EPD_DRAW_ALIGN_CENTER;
  if (align == LEFT) font_props.flags = EPD_DRAW_ALIGN_LEFT;
  if (align == RIGHT) font_props.flags = EPD_DRAW_ALIGN_RIGHT;
  epd_write_string(&currentFont, data, &x, &y, fb, &font_props);
}

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) {
  epd_draw_hline(x0, y0, length, color, fb);
}

void drawPixel(int x, int y, uint8_t color) {
  epd_draw_pixel(x, y, color, fb);
}

void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color) {
  epd_draw_vline(x0, y0, length, color, fb);
}

void drawCircle(int x0, int y0, int r, uint8_t color) {
  epd_draw_circle(x0, y0, r, color, fb);
}

void fillCircle(int x, int y, int r, uint8_t color) {
  epd_fill_circle(x, y, r, color, fb);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color) {
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, fb);
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  EpdRect area = {
        .x = x,
        .y = y,
        .width = w,
        .height = h,
    };
  epd_draw_rect(area, color, fb);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  EpdRect area = {
        .x = x,
        .y = y,
        .width = w,
        .height = h,
    };
  epd_fill_rect(area, color, fb);
}

void epd_update() {
     epd_poweron();
     epd_hl_update_screen(&hl, MODE_GC16, temperature);
     //delay(100);
     //epd_poweroff();                // Probleme Accu mit Touch!!!
}




/////////////////////////////////////////// Programm ///////////////////////////////////////////


void HauptScreen(){
  drawRect(0, 0, 576, 961, Black);

  setFont(OpenSans24B);  //Schrift wahl
   drawString(10, 40, "Test Seite !",  LEFT);   //String (Pos,Text, ausrichtung)
 
  setFont(OpenSans9B);
   drawString(10, 65, "Greyscal", LEFT); 

  setFont(FiraSans_12);
   drawString(10, 120, "ESP MIRKO",  LEFT); 

                  drawRect(94, 49, 32, 22, Black);
                   fillRect(95, 50, 30, 20, White);
                  drawRect(139, 49, 32, 22, Black);
                   fillRect(140, 50, 30, 20, LightGrey);
                  drawRect(184, 49, 32, 22, Black); 
                   fillRect(185, 50, 30, 20, Grey); 
                  drawRect(229, 49, 32, 22, Black);
                   fillRect(230, 50, 30, 20,DarkGrey);
                  drawRect(274, 49, 32, 22, Black);
                   fillRect(275, 50, 30, 20, Black);

                  drawRect(20, 850, 120, 60, Black); 
                   setFont(OpenSans14B);
                    drawString(80, 890, "touch 1", CENTER);

                  drawRect(150, 850, 120, 60, Black); 
                   setFont(OpenSans14B);
                    drawString(210, 890, "touch 2", CENTER);
}

void MenuCase1(){
                  epd_fullclear(&hl, temperature);
                  epd_hl_set_all_white(&hl);
                  drawRect(0, 0, 576, 961, Black);
                   setFont(OpenSans24B);
                    drawString(250, 800, ("Touch 1 \n hat \n funktioniert"), CENTER);
                  epd_update();
                   delay(5000);
                  epd_fullclear(&hl, temperature);
}

void MenuCase2(){
                  epd_fullclear(&hl, temperature);
                  epd_hl_set_all_white(&hl);
                  drawRect(0, 0, 576, 961, Black);
                   setFont(OpenSans24B);
                    drawString(250, 800, ("Touch 2 \n hat \n funktioniert"), CENTER);
                  epd_update();
                   delay(5000);
                  epd_fullclear(&hl, temperature);
}



void setup()
{  
epd_init(EPD_OPTIONS_DEFAULT);        //
hl = epd_hl_init(WAVEFORM) ;          //
epd_set_rotation(orientation);        // 
fb = epd_hl_get_framebuffer(&hl);     //

pinMode( TOUCH_INT, INPUT_PULLUP) ;   // Touch IRQ Pin
Wire.begin(15, 14);                   // Touch I2C Pin
touch.begin();                        // Touch initial

epd_fullclear(&hl, temperature);      //
epd_hl_set_all_white(&hl);            //
drawRect(0, 0, 576, 961, Black);      // draw Hilfsrahmen
}



void loop()
{
//Main Screen
  HauptScreen(); 

//Touch Menu
  {if (touch.scanPoint())                   
      if (digitalRead ( TOUCH_INT ) )
          {touch.getPoint(x, y, 0);
           if ((x > 800 && x < 900) && (y >  20 && y < 120)) {MenuCase1();} 
           if ((x > 800 && x < 900) && (y > 150 && y < 250)) {MenuCase2();}
          }     
  }   

  epd_update();
}

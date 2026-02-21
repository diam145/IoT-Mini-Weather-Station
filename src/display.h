#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h> // ...
#include <GxEPD2_7C.h> // ...
#include <Fonts/FreeMonoBold9pt7b.h> // ...
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/TomThumb.h>
#include "displayIcons.h"



// Define pins for the LCD
constexpr int Pin_LCD_CS = 5; //27;
constexpr int Pin_LCD_DC = 2; // A0 23;
constexpr int Pin_LCD_MOSI = 23; // 14;
constexpr int Pin_LCD_SCLK = 18; // 14;
constexpr int Pin_LCD_RST = 4; // 22;

// constexpr int Pin_LCD_MISO = 12;
// constexpr int Pin_LCD_MOSI = 13;

Adafruit_ST7735 lcd(Pin_LCD_CS, Pin_LCD_DC, Pin_LCD_MOSI, Pin_LCD_SCLK, Pin_LCD_RST);


// HelloWorld
const char HelloWorld[] = "Hello Mountus!";

void initDisplay()
{
  // initialize the ST7735; choose the initR/tab variant for your module if needed
  lcd.initR(INITR_BLACKTAB); // or INITR_144GREENTAB / other tab depending on module
  // lcd.setRotation(1);
  // lcd.fillScreen(ST7735_WHITE);
  // lcd.setFont(); // use default font; setFont(&FreeMonoBold9pt7b) if using GFX font
}

void helloWorld()
{
  lcd.setRotation(1);
  lcd.fillScreen(ST7735_WHITE);
  lcd.setFont(); // use default font; setFont(&FreeMonoBold9pt7b) if using GFX font
  lcd.setFont(&FreeMonoBold9pt7b);
  lcd.setTextColor(ST7735_BLACK);
  
  int16_t tbx, tby; uint16_t tbw, tbh;
  lcd.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((lcd.width() - tbw) / 2) - tbx;
  uint16_t y = ((lcd.height() - tbh) / 2) - tby;
  lcd.fillScreen(ST7735_WHITE);
  // Serial.println();
  // Serial.println("x: " + String(x) + " y: " + String(y));
  lcd.setCursor(x, y);
  lcd.print(HelloWorld);
}

void showDisplayIcons ()
{
  // Placeholder for displaying icons on the LCD
  lcd.setRotation(1);
  // lcd.fillScreen(ST7735_WHITE);
  //
  // lcd.setTextColor(ST7735_BLACK);
  //
  // lcd.drawBitmap(0, 0, (const uint8_t*)displayIcons, 160, 128, ST7735_BLACK);
  // bitmap uses 1-bits for background — draw inverted:
  lcd.fillScreen(ST7735_BLACK);
  lcd.drawBitmap(0, 0, (const uint8_t*)displayIcons, 160, 128, ST7735_WHITE);

  // Add display text next to icons
  lcd.setFont(&FreeSans9pt7b);
  lcd.setTextColor(GxEPD_BLACK);
  lcd.setCursor(40, 40);
  lcd.print("25");

  lcd.setCursor(115, 35);
  lcd.print("Sun");

  lcd.setCursor(50, 105);
  lcd.print("Cloudy");

  lcd.setFont(&TomThumb);
  lcd.setCursor(115, 45);
  lcd.print("09/11/25");


   delay(10000);
 
}



void displayOpenWeathermap(const einkDataTemplate &einkData)
{

  // Placeholder for displaying data from OpenWeathermap
  lcd.fillScreen(ST7735_BLACK);
  lcd.drawBitmap(0, 0, (const uint8_t*)displayIcons, 160, 128, ST7735_WHITE);

  // Add display text next to icons
  lcd.setFont(&FreeSans9pt7b);
  lcd.setTextColor(GxEPD_BLACK);
  lcd.setCursor(40, 40);
  lcd.print(einkData.temperature);

  lcd.setCursor(115, 35);
  lcd.print(einkData.day);

  lcd.setCursor(50, 105);
  lcd.print(einkData.weatherDescription);

  lcd.setFont(&TomThumb);
  lcd.setCursor(115, 45);
  lcd.print(einkData.date);

  lcd.setFont(&TomThumb);
  lcd.setCursor(5, 125);
  String lastRow = "Sun: " + einkData.sunrise + " / " + einkData.sunset + "  Upd: " + einkData.update;
  lcd.print(lastRow);


   delay(5000);
}


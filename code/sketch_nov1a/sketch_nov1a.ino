#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4

char oldtime[20];
char old_date[25];

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Booting");

  WiFi.begin("here wifi name", "here wifi pass");
  while(WiFi.status() != WL_CONNECTED) delay(100);

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");

  tft.fillScreen(ILI9341_BLACK);

  showWeather();
  showPcStats();
}

void showWeather(){

  HTTPClient http;
  http.begin("http://wttr.in/41.89,12.48?m&format=%t+%C");
  int code = http.GET();

  if(code == 200){
    String w = http.getString();
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 230);
    tft.print(w);
  }
  http.end();
}

void showPcStats(){

//its empty for now cause ill make it when ill get the parts.. ill use a placeholder for now
tft.setTextSize(1);
tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
tft.setCursor(5, 80);
tft.print("Pc Stats");

}


void loop() {

  struct tm t;
  if(getLocalTime(&t)){
    char timeStr[20];
    char dateStr[25];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &t);
    strftime(dateStr, sizeof(dateStr), "%a %d %b %Y", &t);


    if(strcmp(timeStr, oldtime) != 0){

        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setCursor(60, 20);
        tft.print(timeStr);
        strcpy(oldtime, timeStr);

    }


    if(strcmp(dateStr, old_date) != 0){

      tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
      tft.setTextSize(2);
      tft.setCursor(60, 50);
      tft.print(dateStr);
      strcpy(old_date, dateStr);

    }
  }

  delay(200);
}
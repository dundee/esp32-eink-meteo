#include "AppConfig.h"

Config config;

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude;

// Waveshare e-Paper Driver HAT
// navody.dratek.cz
// Program vychází z příkladu
//  GxEPD2_HelloWorld.ino dodaného s knihovnou

#include <GxEPD2_BW.h>

#define GxEPD2_DISPLAY_CLASS GxEPD2_BW
#define GxEPD2_DRIVER_CLASS GxEPD2_290_T94_V2

// připojení fontu pro výpis textu
#include <Fonts/FreeMonoBold9pt7b.h>
//#include <Fonts/FreeSans9pt7b.h>

// stav WiFi - je pripojena?
bool wifiOK = false;
// duvod posledniho startu, naplni se automaticky
int wakeupReason;
// je aktualni start probuzenim z deep sleep?
bool deepSleepStart;

#include "forecast.h"

#include "raLogger.h"
raLogger* logger;

// nastavení propojovacích pinů
#define BUSY 4
#define RST  16
#define DC   17
#define CS   5
// CLK = IO18, DIN = IO23

// vytvoření objektu displeje z knihovny,
// všechny podporované displeje najdete v původním příkladu z knihovny
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(CS, DC, RST, BUSY));

void vypisDataEpaper() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C Pressure: "));
  Serial.print(pressure);
  Serial.print(F(" Altitude: "));
  Serial.println(altitude);


  // možné nastavení otočení displeje,
  // zadává se jako 1, 2, 3 (90, 180, 270 stupňů)
  display.setRotation(1);
  // nastavení použitého fontu
  display.setFont(&FreeMonoBold9pt7b);
  //display.setFont(&FreeSans9pt7b);
  // nastavení barvy textu
  display.setTextColor(GxEPD_BLACK);
  // inicializace přepisu displeje
  display.setFullWindow();
  // následující do-while cyklus obstará výpis vybraného textu
  display.firstPage();
  do {
    // vyplnění displeje bílou barvou
    display.fillScreen(GxEPD_WHITE);
    // nastavení kurzoru (x, y), při výpisu textu
    // označuje kurzor pozici řádku, levý-dolní pixel písmen
    display.setCursor(50, 50);
    // výpis textu
    display.print("Weather 0.1");
    // nastavení kurzoru
    display.setCursor(20, 100);
    // výpis textu
    char buffer[40];
    sprintf(buffer, "T:%.2f H:%.2f P:%.2f", temperature, humidity, pressure);
    display.print(buffer);
  }
  while (display.nextPage());
}

void setup() {
  Serial.begin(115200);

  logger = new raLogger( 1 );
  loadConfig( &config );

  bme.begin(0x76);

  networkConfig( "aaaaaaaa", true );

  // inicializace displeje, číslo v závorce udává rychlost,
  // jakou se budou vypisovat ladící údaje na sériovou linku
  // display.init(115200);
  display.init(115200, true, 2, false);

  parse();

  // funkce pro vypsání dat na displej
  vypisDataEpaper();
  // přepnutí displeje do stavu hibernace
  display.hibernate();
}

void loop() {
  // zde se už nic neděje :)
};
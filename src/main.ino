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

  long DEEP_SLEEP_TIME_SEC = 3600;
  esp_sleep_enable_timer_wakeup(1000000ULL * DEEP_SLEEP_TIME_SEC);
  Serial.println("Starting deep-sleep period...");
  esp_deep_sleep_start();
}

void loop() {
  // zde se už nic neděje :)
};
// připojení fontu pro výpis textu
#include <Fonts/FreeMonoBold9pt7b.h>
//#include <Fonts/FreeMono9pt7b.h>
//#include <Fonts/FreeSans9pt7b.h>
#include <string>
#include <map>

void vypisDataEpaper() {
  std::map<std::string, std::string> periods;
  periods["dnes_noc"] = "22-06";
  periods["dnes_dopoledne"] = "06-12";
  periods["dnes_odpoledne"] = "12-18";
  periods["dnes_vecer"] = "18-22";
  periods["zitra_noc"] = "22-06";
  periods["zitra_den"] = "06-21";
  periods["zitra_dopoledne"] = "06-12";
  periods["zitra_odpoledne"] = "12-18";
  periods["zitra_vecer"] = "18-22";

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
    display.setCursor(0, 10);
    // výpis textu

    for (int i = 0; i < sizeof(sections); i++) {
      display.setCursor(i * 70 + 10, 13);
      display.print(periods[sections[i].name].c_str());

      display.setCursor(i * 70 + 10, 30);
      display.print(String(sections[i].temp_min, 1).c_str());
      display.print("C");

      display.setCursor(i * 70 + 10, 45);
      display.print(String(sections[i].temp_max, 1).c_str());
      display.print("C");

      display.setCursor(i * 70 + 10, 60);
      display.print(String(sections[i].rain, 1).c_str());
      display.print("mm");
    }



    // // nastavení kurzoru
    // display.setCursor(20, 100);
    // // výpis textu
    // char buffer[40];
    // sprintf(buffer, "T:%.2f H:%.2f P:%.2f", temperature, humidity, pressure);
    // display.print(buffer);
  }
  while (display.nextPage());
}
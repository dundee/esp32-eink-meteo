#include "forecast.h"
#include "ArduinoJson.h"
#include <HTTPClient.h>

char json[] = "{                     "
    "\"sections\":[                    "
    "      {                              "
    "         \"nazev\":\"dnes_vecer\",   "
    "         \"temp_min\":17.4,          "
    "         \"temp_max\":17.4,          "
    "         \"rain_sum\":0.0,           "
    "         \"rain_max\":0,             "
    "         \"clouds_min\":0.8,         "
    "         \"clouds_max\":0.8,         "
    "         \"fog\":\"-\",              "
    "         \"icon\":\"clearsky\"       "
    "      {                              "
    "      },                             "
    "         \"nazev\":\"dnes_noc\",     "
    "         \"temp_min\":14.7,          "
    "         \"temp_max\":17.9,          "
    "         \"rain_sum\":0.0,           "
    "         \"rain_max\":0,             "
    "         \"clouds_min\":0.8,         "
    "         \"clouds_max\":78.1,        "
    "         \"fog\":\"-\",              "
    "         \"icon\":\"partlycloudy\"   "
    "      },                             "
    "      {                              "
    "         \"nazev\":\"zitra_dopoledne\","
    "         \"temp_min\":14.5,          "
    "         \"temp_max\":21.5,          "
    "         \"rain_sum\":0.0,           "
    "         \"rain_max\":0,             "
    "         \"clouds_min\":24.2,        "
    "         \"clouds_max\":100.0,       "
    "         \"fog\":\"-\",              "
    "         \"icon\":\"partlycloudy\"   "
    "      },                             "
    "      {                              "
    "         \"nazev\":\"zitra_odpoledne\","
    "         \"temp_min\":22.4,          "
    "         \"temp_max\":23.4,          "
    "         \"rain_sum\":0.0,           "
    "         \"rain_max\":0,             "
    "         \"clouds_min\":3.1,         "
    "         \"clouds_max\":67.2,        "
    "         \"fog\":\"-\",              "
    "         \"icon\":\"partlycloudy\"   "
    "      },                             "
    "      {                              "
    "         \"nazev\":\"zitra_vecer\",  "
    "         \"temp_min\":16.4,          "
    "         \"temp_max\":21.9,          "
    "         \"rain_sum\":0.0,           "
    "         \"rain_max\":0,             "
    "         \"clouds_min\":0.0,         "
    "         \"clouds_max\":51.6,        "
    "         \"fog\":\"-\",              "
    "         \"icon\":\"partlycloudy\"   "
    "      }                              "
    "   ]                                 "
    "}\";                                 ";


    void parse() {
        startWifi();

        delay(2000);

        const char* url = config.getString("yrno_url", "none");
        String payload;

        logger->log("Nacitam data z URL: %s", url);

        HTTPClient http;
        bool ok = http.begin(url);
        if (!ok) {
            logger->log("dotaz selhal");
        }

        int httpResponseCode = http.GET();

      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        ESP.restart();
      }

      DynamicJsonDocument doc(1024);
      DeserializationError err = deserializeJson(doc, payload.c_str());

      // Free resources
      http.end();
      stopWifi();

      if (err != DeserializationError::Ok) {
        logger->log("Parsovani selhalo: %s", err.c_str());
        return;
      }

      sections = new Section[doc["sections"].size()];

      for (int i = 0; i < doc["sections"].size(); i++) {
        sections[i].name = doc["sections"][i]["nazev"];
        sections[i].temp_min = doc["sections"][i]["temp_min"];
        sections[i].temp_max = doc["sections"][i]["temp_max"];
        sections[i].rain = doc["sections"][i]["rain_sum"];
        sections[i].icon = doc["sections"][i]["icon"];
      }

      const char* nazev = doc["sections"][0]["nazev"];
      const float tempMin = doc["sections"][0]["temp_min"];
      const float tempMax = doc["sections"][0]["temp_max"];

      Serial.print("JSON: ");
      Serial.println(nazev);
      Serial.println(tempMin);
      Serial.println(tempMax);

      Serial.println();
}
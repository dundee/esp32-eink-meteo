#ifndef FORECAST__H_
#define FORECAST__H_

/*
    {
       "nazev":"dnes_vecer",
       "temp_min":17.4,
       "temp_max":17.4,
       "rain_sum":0.0,
       "rain_max":0,
       "clouds_min":0.8,
       "clouds_max":0.8,
       "fog":"-",
       "icon":"clearsky"
    {
*/
struct Section {
    const char* name;
    float temp_min;
    float temp_max;
    float rain;
    const char* icon;
};

Section *sections;

void parse();

#endif
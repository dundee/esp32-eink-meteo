#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#ifndef CONFIGPROVIDER_H
#define CONFIGPROVIDER_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

// jmeno konfiguracniho souboru ve SPIFS
#define CONFIG_FILE_NAME (char*)"/config.meteo"

bool isConfigValid( Config * config );
void loadConfig( Config * cfg );
void saveConfig( Config * cfg );

#endif

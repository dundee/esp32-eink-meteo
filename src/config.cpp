#include "config.h"

#define CBC 1
#define ECB 0
#define CTR 0


Config::Config()
{
    this->fields = (char**) malloc( this->size * sizeof(char *) );
    this->values = (char**) malloc( this->size * sizeof(char *) );
    this->length = 0;
    this->dirty = false;
}

Config::~Config()
{
    for( int i = 0; i<this->length; i++ ) {
        free( this->fields[i] );
        free( this->values[i] );
    }
    free(this->fields);
    free(this->values);
}


/**
 * Pokud je jiz pole zaplneno, prodlouzi ho.
 */
void Config::reserveSize()
{
    if( this->length >= this->size ) {
        this->size = this->size * 2;
        this->fields = (char**)realloc( this->fields, this->size * sizeof(char *) );
        this->values = (char**)realloc( this->values, this->size * sizeof(char *) );
    }
}

void trimRight( char * text )
{
    int n = strlen(text) - 1;
    while( n>=0 ) {
        if( text[n]==' ' ) {
            text[n] = 0;
        } else {
            break;
        }
        n--;
    }
}

#include "raLogger.h"

void Config::zapisVystup( char * name, int namePos, char * value, int valuePos )
{
    this->reserveSize();

    name[namePos] = 0;
    trimRight( name );
    value[valuePos] = 0;
    trimRight( value );

    this->values[this->length] = (char*)malloc( strlen(value)+1 );
    strcpy( this->values[this->length], value );

    this->fields[this->length] = (char*)malloc( strlen(name)+1 );
    strcpy( this->fields[this->length], name );

    this->length++;
}

#define KEY_NAME_MAX 25
#define VALUE_MAX 255

void Config::parseFromString(  char * input )
{
    this->dirty = false;

    char * p = input;
    char name[KEY_NAME_MAX+1];
    int namePos = 0;
    char value[VALUE_MAX+1];
    int valuePos = 0;

    /**
     * 0 = zaciname / pred jmenem
     * 1 = ve jmenu
     * 2 = pred hodnotou
     * 3 = v hodnote
     */
    int state = 0;

    while( true )
    {
        if( *p==0 ) {
            // konec vstupniho textu
            if( state==3 ) {
                this->zapisVystup( name, namePos, value, valuePos );
            }
            break;
        }
        if( state==0 ) {
            if( *p==' ' || *p==13 || *p==10 || *p==8 ) {
                // nic
            } else {
                state = 1;
                continue; // protoze jinak by se na konci posunulo na dalsi pozici
            }
        } else if( state==1 ) {
            if( *p==13 || *p==10 ) {
                state = 0;
                namePos = 0;
            } else if( *p=='=' )  {
                state = 2;
            } else {
                if( namePos<KEY_NAME_MAX ) {
                    name[namePos++] = *p;
                }
            }
        } else if( state==2 ) {
            if( *p == ' ' ) {
                // nedelame nic - preskakujeme mezery na zacatku
            } else {
                state = 3;
                continue; // protoze jinak by se na konci posunulo na dalsi pozici
            }
        } else if( state==3 ) {
            if( *p==13 || *p==10 ) {
                this->zapisVystup( name, namePos, value, valuePos );
                namePos = 0;
                valuePos = 0;
                state = 0;
            } else {
                if( valuePos < VALUE_MAX ) {
                    value[valuePos++] = *p;
                }
            }
        }
        p++;
    } // while( true )
}

const char * Config::getString( const char * fieldName, const char * defaultValue )
{
    int i = this->findKey( fieldName );
    if( i!=-1 ) {
        return this->values[i];
    } else {
        return defaultValue;
    }
}

long Config::getLong( const char * fieldName, long defaultValue )
{
    int i = this->findKey( fieldName );
    if( i!=-1 ) {
        return atol(this->values[i]);
    } else {
        return defaultValue;
    }
}

/**
 * Vraci klic v poli nebo -1
 */
int Config::findKey( const char * fieldName )
{
    for(int i = 0; i < this->length; i++ ) {
        if( strcmp( this->fields[i], fieldName )== 0 ) {
            return i;
        }
    }
    return -1;
}


/**
 * Umi zmenit hodnotu existujiciho klice ci pridat novy
 */
void Config::setValue( const char * fieldName, const char * value )
{
    int i = this->findKey( fieldName );
    if( i!=-1 ) {
        // nalezeno
        free( this->values[i] );
        this->values[i] = (char*)malloc( strlen(value)+1 );
        strcpy( this->values[i], value );
    } else {
        // nenalezena polozka
        this->reserveSize();
        this->fields[this->length] = (char*)malloc( strlen(fieldName)+1 );
        strcpy( this->fields[this->length], fieldName );
        this->values[this->length] = (char*)malloc( strlen(value)+1 );
        strcpy( this->values[this->length], value );
        this->length++;
    }
    this->dirty = true;
}


void Config::setInfo( char * deviceId, char * appPassPhrase )
{
    strncpy( this->deviceId, deviceId, 32 );
    this->deviceId[32] = 0;
    strncpy( this->appPassPhrase, appPassPhrase, 32 );
    this->appPassPhrase[32] = 0;
}


void Config::printTo( Print& p )
{
    char buffer[256];


    for(int i = 0; i < this->length; i++ ) {
        p.print( this->fields[i] );
        p.print( '=' );
        p.print( this->values[i] );
        p.print( '\n' );
    }
    this->dirty = false;
}

bool Config::isDirty()
{
    return this->dirty;
}


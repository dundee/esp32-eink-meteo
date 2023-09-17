#include <Arduino.h>

#ifndef CONFIG_H
#define CONFIG_H

class Config
{
  public:
    Config();
    ~Config();
    void setInfo( char * deviceId, char * appPassPhrase );

    void parseFromString( char * input );
    void printTo( Print& p );
    const char * getString( const char * fieldName, const char * defaultValue );
    long getLong( const char * fieldName, long defaultValue );

    /**
     * Umi zmenit hodnotu existujiciho klice ci pridat novy
     */
    void setValue( const char * fieldName, const char * value );

    bool isDirty();

    char ** fields;
    char ** values;
    int length;

    /** je public jen kvuli testovani, nepouzivat */
    bool dectryptItem( char * aes_key, char * itemName, char * ciphertext, char * target, int targetSize );
    /** je public jen kvuli testovani, nepouzivat */
    void encryptItem( char * aes_key, char * itemName, char * value, char * target, int targetSize );
    /** je public jen kvuli testovani, nepouzivat */
    void createEncryptionKey( char * deviceId, char * appPassphrase, char * aes_key );

  private:
    void zapisVystup( char * name, int namePos, char * value, int valuePos );

    /**
     * Vraci klic v poli nebo -1
     */
    int findKey( const char * fieldName );

    /**
     * Pokud je jiz pole zaplneno, prodlouzi ho.
     */
    void reserveSize();

    int size = 10;
    bool dirty;

    char deviceId[33];
    char appPassPhrase[33];
};


#endif
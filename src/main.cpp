/*
 *  Project 22-ntp - main.cpp
 *
 *      Austral 2023 - Electronica Digital - EAM
 *
 *      Connects with time server and
 *      shows week day and complete time for
 *      the time zone defined
 * 
 *      Also, through Unix library <time.h> one can
 *      print date and time with desired format using
 *      call to function 'strftime' based in 'epoch time'
 *
 */

/*
 *  Definitions included  in platformio.ini
 * 
 *      TIME_ZONE   Time zone in hours
 *      NTP_SERVER  NTP server name
 *      SERIAL_BAUD Serial port Baud rate
 */

/*
 *  Includes
 */

//  General includes

#include <Arduino.h>

#include <WiFi.h>

#include <NTPClient.h>          // for access to time servers
#include <WiFiUdp.h>            // for using UDP protocol
#include <time.h>               // to access to Unix library calls

//  Project includes

#include "wifi_ruts.h"          // WiFi access

//  Macros and definitions

#define TZ(tz)      (tz*3600)               //  Macro to convert from time zone in hours to seconds

/*
 *  Objects creation
 */

WiFiUDP ntpUDP;                                                 //  Object for using UDP
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ(TIME_ZONE) );       //  Object NTPClient

/*
 *  Public functions
 */

void
setup(void)
{
    Serial.begin(SERIAL_BAUD);
    wifi_connect();
    timeClient.begin();
}

void
loop(void)
{
    time_t epoch_time;
    char buf[80];

    timeClient.update();                    //  Get time and date from server

    Serial.printf( "Epoch = %ld\n", epoch_time = timeClient.getEpochTime() );
    strftime(buf, sizeof(buf),"%A: %d %B %Y %H:%M:%S", localtime(&epoch_time));
    Serial.printf( "%s\n", buf); 

    delay(1000);
}

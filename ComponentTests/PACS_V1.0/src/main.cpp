#include <Arduino.h>

#include "sbus.h"
#include <Adafruit_GPS.h>

/* SBUS object, reading SBUS */
//Put sbus pin on rx which is 12
bfs::SbusRx Sbusrx(&Serial1, 12, 10, true);
#define GPSSerial Serial2

Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis();

/* SBUS object, writing SBUS */
//bfs::SbusTx sbus_tx(&Serial2);
/* SBUS data */
bfs::SbusData data;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Adafruit GPS library basic parsing test!");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  /* Begin the SBUS communication */
  Sbusrx.Begin();
  //SbusTx.Begin();
}

void loop () {
  if (millis() - timer > 200) 
  {
  if (Sbusrx.Read()) 
  {
    /* Grab the received data */
    data = Sbusrx.data();
    /* Display the received data */
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
  }
  }

    char c = GPS.read();
    if (GPSECHO)
    if (c) Serial.print(c);

      if (GPS.newNMEAreceived()) 
      {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  if (millis() - timer > 2000) 
  {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) 
    {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 15); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 15); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
    delay(1000);
  }
}

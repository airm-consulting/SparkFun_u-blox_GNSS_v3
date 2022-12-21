/*
  Reading Position, Velocity and Time (PVT) via UBX binary commands
  By: Paul Clark
  SparkFun Electronics
  Date: December 21st, 2022
  License: MIT. Please see LICENSE.md for more information.

  This example shows how to query a u-blox module for its position, velocity and time (PVT) data using SPI.
  We also turn off the NMEA output on the SPI port. This decreases the amount of traffic dramatically.

  Note: Lat/Lon are large numbers because they are * 10^7. To convert lat/lon
  to something google maps understands simply divide the numbers by 10,000,000.

  Feel like supporting open source hardware?
  Buy a board from SparkFun!
  SparkFun GPS-RTK2 - ZED-F9P (GPS-15136)    https://www.sparkfun.com/products/15136
  SparkFun GPS-RTK-SMA - ZED-F9P (GPS-16481) https://www.sparkfun.com/products/16481
  SparkFun MAX-M10S Breakout (GPS-18037)     https://www.sparkfun.com/products/18037
  SparkFun ZED-F9K Breakout (GPS-18719)      https://www.sparkfun.com/products/18719
  SparkFun ZED-F9R Breakout (GPS-16344)      https://www.sparkfun.com/products/16344

  Hardware Connections:
  Hook up the PICO, POCI, SCK, CS and GND pins, plus 3V3 or 5V depending on your needs
  Connect: GNSS PICO to microcontroller PICO; GNSS POCI to microcontroller POCI
  Open the serial monitor at 115200 baud to see the output
*/

#include <SPI.h> //Needed for SPI to GNSS

#include <SparkFun_u-blox_GNSS_v3.h> //http://librarymanager/All#SparkFun_u-blox_GNSS_v3

SFE_UBLOX_GNSS_SPI myGNSS; // SFE_UBLOX_GNSS_SPI uses SPI. For I2C or Serial, see Example1 and Example2

#define myCS 4 // Define the GPIO pin number for the SPI Chip Select. Change this if required

void setup()
{
  Serial.begin(115200);
  delay(1000); 
  Serial.println("SparkFun u-blox Example");

  SPI.begin(); // Begin SPI communication

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  while (myGNSS.begin(myCS) == false) //Connect to the u-blox module using SPI and myCS (defined above)
  {
    Serial.println(F("u-blox GNSS not detected. Retrying..."));
    delay (1000);
  }

  myGNSS.setSPIOutput(COM_TYPE_UBX); //Set the UART1 port to output UBX only (turn off NMEA noise)
  
  //myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Optional: save (only) the communications port settings to flash and BBR
}

void loop()
{
  // Request (poll) the position, velocity and time (PVT) information.
  // The module only responds when a new position is available. Default is once per second.
  // getPVT() returns true when new data is received.
  if (myGNSS.getPVT() == true)
  {
    int32_t latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    int32_t longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    int32_t altitude = myGNSS.getAltitudeMSL(); // Altitude above Mean Sea Level
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    Serial.println();
  }
}

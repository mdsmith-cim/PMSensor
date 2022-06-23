#include <Arduino.h>
#include <PMserial.h>
#include <U8g2lib.h>
#include <Wire.h>

SerialPM pms(PLANTOWER_AUTO, D7, D8);  // PMSx003, RX, TX

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


void setup() {
  Serial.begin(9600);
  pms.init();                   // config serial port

  u8g2.begin();
  u8g2.enableUTF8Print();	
}


void loop() {
  pms.read();                   // read the PM sensor
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                  pms.pm01, pms.pm25, pms.pm10);
    Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                  pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);

  // Serial.println(F("Status:")); 
  // Serial.print(F("Formaldehyde: ")); Serial.println(pms.has_formaldehyde());
  // Serial.print(F("Particulate Matter: ")); Serial.println(pms.has_particulate_matter());
  // Serial.print(F("Number concentration: ")); Serial.println(pms.has_number_concentration());
  // Serial.print(F("Temp/Humid: ")); Serial.println(pms.has_temperature_humidity());

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 11);
    u8g2.print(F("PM1.0 ")); u8g2.print(pms.pm01); u8g2.print(F(" [ug/m3]"));
    u8g2.setCursor(0, 22);
    u8g2.print(F("PM2.5 ")); u8g2.print(pms.pm25); u8g2.print(F(" [ug/m3]"));
    u8g2.setCursor(0, 33);
    u8g2.print(F("PM10 ")); u8g2.print(pms.pm10); u8g2.print(F(" [ug/m3]"));
  } while ( u8g2.nextPage() );
  delay(2000);                 // wait for 2 seconds
}


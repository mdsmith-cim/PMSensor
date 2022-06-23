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
}


void loop() {
  pms.read();                   // read the PM sensor
  if (pms) {
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                  pms.pm01, pms.pm25, pms.pm10);
    Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                  pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);

    u8g2.firstPage();
    do {
      // Display is 128x64
      // Font height 8, width 6, total 21 characters across
      u8g2.setFont(u8g2_font_t0_12_mr);
      u8g2.setCursor(0, 8);
      u8g2.print(F("PM 1.0|PM 2.5|PM 10  "));
      u8g2.setCursor(0, 18);
      u8g2.printf("%-6u|%-6u|%-7u", pms.pm01, pms.pm25, pms.pm10);
      u8g2.drawHLine(0, 20, 128);
      u8g2.setCursor(0, 30);
      u8g2.print(F("N0.3  |N0.5  |N1.0   "));
      u8g2.setCursor(0, 40);
      u8g2.printf("%-6u|%-6u|%-7u", pms.n0p3, pms.n0p5, pms.n1p0);
      u8g2.drawHLine(0, 42, 128);
      u8g2.setCursor(0, 52);
      u8g2.print(F("N2.5  |N5.0  |N10    "));
      u8g2.setCursor(0, 62);
      u8g2.printf("%-6u|%-6u|%-7u", pms.n2p5, pms.n5p0, pms.n10p0);
      u8g2.drawHLine(0, 64, 128);

    } while ( u8g2.nextPage() );
    delay(1000);                 // wait for 2 seconds
  }
  else {
    switch (pms.status)
    {
    case pms.OK: // should never come here
      break;     // included to compile without warnings
    case pms.ERROR_TIMEOUT:
      Serial.println(F(PMS_ERROR_TIMEOUT));
      break;
    case pms.ERROR_MSG_UNKNOWN:
      Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case pms.ERROR_MSG_HEADER:
      Serial.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case pms.ERROR_MSG_BODY:
      Serial.println(F(PMS_ERROR_MSG_BODY));
      break;
    case pms.ERROR_MSG_START:
      Serial.println(F(PMS_ERROR_MSG_START));
      break;
    case pms.ERROR_MSG_LENGTH:
      Serial.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case pms.ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms.ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_PMS_TYPE));
      break;
    }
  }
}


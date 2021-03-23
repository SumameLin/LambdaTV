#include "LambdaTV.h"
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <NTPClient.h>
#include <OneButton.h>
#include <SPI.h>
#include <Ticker.h>
#include <TimeLib.h>
#include <U8g2lib.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <qrcode.h>
#include <stdint.h>

const uint8_t write_num0[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
  0x00, 0x7E, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00,
  0x00, 0xF8, 0x03, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x1F, 0x07, 0x00,
  0x00, 0x0F, 0x07, 0x00, 0xC0, 0x03, 0x07, 0x00, 0xC0, 0x03, 0x0E, 0x00,
  0xC0, 0x01, 0x0E, 0x00, 0xE0, 0x01, 0x0E, 0x00, 0xE0, 0x00, 0x0E, 0x00,
  0xE0, 0x00, 0x0E, 0x00, 0xF0, 0x00, 0x0E, 0x00, 0xE0, 0x00, 0x0E, 0x00,
  0xE0, 0x00, 0x06, 0x00, 0xE0, 0x00, 0x07, 0x00, 0x60, 0x00, 0x07, 0x00,
  0xF0, 0x80, 0x07, 0x00, 0xE0, 0xC0, 0x03, 0x00, 0xE0, 0xE0, 0x03, 0x00,
  0xE0, 0xFB, 0x01, 0x00, 0xC0, 0x7F, 0x00, 0x00, 0x80, 0x7F, 0x00, 0x00,
  0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t write_num1[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
  0x00, 0x38, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x7C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x7E, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x38, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t write_num2[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00,
  0xE0, 0xFF, 0x01, 0x00, 0xFC, 0xEB, 0x01, 0x00, 0x7E, 0xC0, 0x01, 0x00,
  0x0F, 0xC0, 0x01, 0x00, 0x06, 0xC0, 0x01, 0x00, 0x00, 0xE0, 0x00, 0x00,
  0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,
  0x00, 0x78, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
  0x00, 0x1C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x00, 0x80, 0x07, 0xC0, 0x00, 0x80, 0x03, 0x70, 0x00,
  0xC0, 0x01, 0x38, 0x00, 0xE0, 0x01, 0x1E, 0x00, 0xE0, 0x00, 0x07, 0x00,
  0xF0, 0xE0, 0x03, 0x00, 0x70, 0xF0, 0x01, 0x00, 0x70, 0x7E, 0x00, 0x00,
  0xF8, 0x1F, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num3[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0x01, 0x00, 0x00, 0xFE, 0x03, 0x00, 0x00, 0xFF, 0x5F, 0x00, 0x00,
  0xFE, 0xFF, 0x01, 0x00, 0xFE, 0xFF, 0x03, 0x00, 0x82, 0xFF, 0x03, 0x00,
  0x00, 0xE0, 0x07, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00,
  0x00, 0xE0, 0x03, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0xF8, 0x00, 0x00,
  0x00, 0x7C, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00,
  0x00, 0xFF, 0x01, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xE0, 0x3F, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0xF0, 0x00,
  0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x70, 0x00,
  0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x3F, 0x00,
  0x00, 0xF8, 0x1F, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0xE0, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num4[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xE0, 0xC0, 0x03, 0x00,
  0xF0, 0xC1, 0x07, 0x00, 0xF0, 0xC3, 0x0F, 0x00, 0x70, 0x82, 0x0F, 0x00,
  0x38, 0xC0, 0x19, 0x00, 0x38, 0x80, 0x0B, 0x00, 0x38, 0x80, 0x13, 0x00,
  0x38, 0x80, 0x13, 0x00, 0x3C, 0x80, 0x23, 0x00, 0x3C, 0x80, 0x23, 0x00,
  0x1C, 0x80, 0x23, 0x00, 0x3C, 0x80, 0x67, 0x00, 0x1C, 0x00, 0xF7, 0x00,
  0x1C, 0xC0, 0x7F, 0x00, 0xFC, 0xFF, 0x7F, 0x00, 0xFC, 0xFF, 0x1F, 0x00,
  0xFC, 0xFF, 0x07, 0x00, 0xF8, 0xDF, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00,
  0x00, 0x80, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00,
  0x00, 0x80, 0x0F, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x80, 0x0F, 0x00,
  0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num5[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x03, 0x00,
  0xFC, 0xFF, 0x7F, 0x00, 0xFC, 0xFF, 0x7F, 0x00, 0xFC, 0xFF, 0x7F, 0x00,
  0x78, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00,
  0x78, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00,
  0x3C, 0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00,
  0xF8, 0x7F, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00, 0x80, 0xFF, 0x03, 0x00,
  0x00, 0xFC, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x80, 0x07, 0x00,
  0x00, 0x80, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x03, 0x00,
  0x00, 0xFE, 0x03, 0x00, 0x80, 0xFF, 0x01, 0x00, 0x80, 0xFF, 0x00, 0x00,
  0x80, 0x1F, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num6[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x70, 0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0xDF, 0x03, 0x00,
  0x80, 0x0F, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00,
  0xE0, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x70, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00,
  0x1C, 0x00, 0x00, 0x00, 0x1C, 0x70, 0x00, 0x00, 0x1C, 0xFE, 0x03, 0x00,
  0x1E, 0xFF, 0x0F, 0x00, 0xDC, 0xCF, 0x1F, 0x00, 0xDE, 0x01, 0x3E, 0x00,
  0xFE, 0x00, 0x78, 0x00, 0x7C, 0x00, 0xF0, 0x00, 0x7C, 0x00, 0xF0, 0x00,
  0xFC, 0x00, 0x7C, 0x00, 0xF8, 0x03, 0x7F, 0x00, 0xF8, 0xFF, 0x3F, 0x00,
  0xE0, 0xFF, 0x0F, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num7[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC6, 0x3F, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00,
  0xFE, 0xFF, 0x00, 0x00, 0xDE, 0xFF, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00,
  0x00, 0xF0, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00,
  0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
  0x00, 0x70, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
  0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x38, 0x40, 0x00,
  0x38, 0x38, 0x60, 0x00, 0x7C, 0x3C, 0x78, 0x00, 0xF8, 0xFF, 0x3F, 0x00,
  0xF8, 0xFF, 0x1F, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x00, 0x7F, 0x02, 0x00,
  0x00, 0x1F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00,
  0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
  0x80, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num8[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00,
  0xF8, 0x3F, 0x00, 0x00, 0xF8, 0xFF, 0x00, 0x00, 0xBC, 0xFF, 0x00, 0x00,
  0x7C, 0xFC, 0x01, 0x00, 0x5E, 0xC0, 0x01, 0x00, 0x5E, 0xC0, 0x00, 0x00,
  0x6E, 0xE0, 0x01, 0x00, 0x4E, 0xF0, 0x00, 0x00, 0x7C, 0xFC, 0x00, 0x00,
  0xFC, 0x7F, 0x00, 0x00, 0xF8, 0xFF, 0x00, 0x00, 0xF0, 0xFF, 0x07, 0x00,
  0xF0, 0xF7, 0x0F, 0x00, 0xF0, 0x81, 0x1F, 0x00, 0xF0, 0x00, 0x1C, 0x00,
  0xF0, 0x00, 0x38, 0x00, 0x70, 0x00, 0x30, 0x00, 0xF0, 0x00, 0x38, 0x00,
  0xF0, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x3C, 0x00, 0xE0, 0x00, 0x3C, 0x00,
  0xE0, 0x01, 0x1E, 0x00, 0xC0, 0x01, 0x1F, 0x00, 0xC0, 0xE7, 0x0F, 0x00,
  0x80, 0xFF, 0x07, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_num9[] U8X8_PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0x01, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00,
  0x78, 0x3E, 0x01, 0x00, 0x38, 0xBC, 0x03, 0x00, 0x38, 0xF8, 0x03, 0x00,
  0x38, 0xF0, 0x07, 0x00, 0x18, 0xE0, 0x07, 0x00, 0x18, 0xF0, 0x03, 0x00,
  0x1C, 0xF0, 0x07, 0x00, 0x1C, 0xF8, 0x07, 0x00, 0x1C, 0xFC, 0x07, 0x00,
  0xFC, 0x3F, 0x07, 0x00, 0xF8, 0x1F, 0x07, 0x00, 0xF8, 0x07, 0x06, 0x00,
  0xF0, 0x03, 0x07, 0x00, 0xC0, 0x01, 0x06, 0x00, 0x00, 0x00, 0x0E, 0x00,
  0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00,
  0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3E, 0x00,
  0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00,
  0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t write_maohao[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00,
  0x00, 0xFE, 0x01, 0x00, 0x00, 0xFF, 0x07, 0x00, 0x80, 0xEF, 0x07, 0x00,
  0xC0, 0xC7, 0x07, 0x00, 0xC0, 0x9F, 0x03, 0x00, 0xC0, 0xFF, 0x01, 0x00,
  0x80, 0xFF, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xB0, 0x01, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00,
  0x00, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x80, 0xFF, 0x02, 0x00,
  0x80, 0x3F, 0x03, 0x00, 0x80, 0xBF, 0x03, 0x00, 0x00, 0xFF, 0x00, 0x00,
  0x00, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/2, /* dc=*/4);
OneButton s_button(5, true);
OneButton c_button(0, true);
File dataFile;
Ticker key_tick;
Ticker led_tick;
WiFiManager wifiManager; // 建立WiFiManager对象
WiFiUDP ntpUDP;
ESP8266WebServer esp8266_server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
// NTPClient timeClient(ntpUDP, "time.nist.gov", 60*60*7, 60000);
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);
LambdaTV_INF lambdaTV;
EEPROM_INF eeprom;
RGB_INF rgb;
RGB_INF rgb_save;
uint8_t badapple_buf[1024] U8X8_PROGMEM = {};   //更新badapple的数组
uint8_t num_kongbai[168] U8X8_PROGMEM = {0x00}; //空白 闪烁效果
void time_update(void);
void bin_player(void);
void web_introduce(void);
void config(void);
void (*current_operation_index)(void);
menu_state home_state = {ICON_BGAP, ICON_BGAP, 0};
menu_state home_last_state = {ICON_BGAP, ICON_BGAP, 0};
// encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
menu_entry_type menu_entry_list[] =
{
    {u8g2_font_open_iconic_app_4x_t, 69, "Clock", (*time_update)},
    {u8g2_font_open_iconic_play_4x_t, 78, "Player", (*bin_player)},
    {u8g2_font_open_iconic_www_4x_t, 78, "Web", (*web_introduce)},
    {u8g2_font_open_iconic_embedded_4x_t, 72, "Config", (*config)},
    {NULL, 0, NULL, NULL}
};
uint32_t config_state, config_last_state;
void (*config_operation_index)(void);
void clock_mode(void);
void instrustions(void);
void clear_wifi(void);
void config_about(void);
void close_open_rgb(void);
void instrustions_enter(void);
void about_enter(void);
void clock_mode_enter(void);
void clear_wifi_enter(void);
void close_open_rgb_enter(void);
#define SETING_FIRST_MENU 5 //设置的一级菜单数量
config_table config_list[] =
{
    {0, 4, 1, 5, 0, (*clock_mode)},     //0
    {1, 0, 2, 6, 1, (*close_open_rgb)}, //1
    {2, 1, 3, 7, 2, (*clear_wifi)},     //2
    {3, 2, 4, 8, 3, (*instrustions)},   //3
    {4, 3, 0, 9, 4, (*config_about)},   //4
    {5, 5, 5, 5, 0, (*clock_mode_enter)},//5
    {6, 6, 6, 6, 1, (*close_open_rgb_enter)},    //6
    {7, 7, 7, 7, 2, (*clear_wifi_enter)},//7
    {8, 8, 8, 8, 3, (*instrustions_enter)},//8
    {9, 9, 9, 9, 4, (*about_enter)},//9
};
/*
函 数 名:void print_fs_info(void)
功能说明:打印fs信息
形    参:void
返 回 值:void
时    间：2020-12-20
RAiny
*/
void print_fs_info(void)
{
    FSInfo fs_info;
    // 闪存文件系统信息
    SPIFFS.info(fs_info);

    // 可用空间总和（单位：字节）
    Serial.print("totalBytes: ");
    Serial.print(fs_info.totalBytes);
    Serial.println(" Bytes");

    // 已用空间（单位：字节）
    Serial.print("usedBytes: ");
    Serial.print(fs_info.usedBytes);
    Serial.println(" Bytes");

    // 最大文件名字符限制（含路径和'\0'）
    Serial.print("maxPathLength: ");
    Serial.println(fs_info.maxPathLength);

    // 最多允许打开文件数量
    Serial.print("maxOpenFiles: ");
    Serial.println(fs_info.maxOpenFiles);
}
/*
函 数 名:void LambdaTV(void)
功能说明:开机LOGO
形    参:void
返 回 值:void
时    间：2020-12-20
RAiny
*/
void LambdaTV(void)
{
    uint8_t x = 20;
    do
    {
        u8g2.setFont(u8g2_font_ncenB14_tr);
        //u8g2.setFont(u8g2_font_courB24_tn);
        u8g2.clearBuffer();
        /* Solid font mode with XOR drawing: Does not make much sense */
        u8g2.setFontMode(0);
        u8g2.setDrawColor(2);
        u8g2.drawStr(x, 26, "LambdaTV");
        u8g2.drawHLine(0, 29, 128);

        /* Transparent font mode with XOR drawing: looks good */
        u8g2.setFontMode(1);
        u8g2.setDrawColor(2);
        u8g2.drawStr(x, 26 + 32, "LambdaTV");
        u8g2.drawHLine(0, 29 + 32, 128);
        u8g2.sendBuffer();
        delay(200);
        x--;
    } while (x != 0);
}
/*
函 数 名:void bin_player(void)
功能说明:bin 动画播放器
形    参:void
返 回 值:void
时    间：2020-12-21
RAiny
*/
void bin_player(void)
{
    char data_read;
    const char *apple_bin = "/apple.bin";
    const char *basket_bin = "/basket.bin";
    const char *elephant_bin = "/elephant.bin";
    const char *plant_bin = "/plant.bin";
    const char *space_bin = "/space.bin";
    const uint8_t hidden_mode_max = 4;
    static int data_len = 0;
    static uint8_t exit_flag = 0;
    uint8_t hidden_mode = 0;
    uint32_t delay_show_time = 165000;
    //建立File对象用于从SPIFFS中读取文件
    String file_name = apple_bin;
    set_rgb_val(69, 137, 148);
    while (1)
    {
        dataFile = SPIFFS.open(file_name, "r");
        //确认闪存中是否有file_name文件
        if (SPIFFS.exists(file_name))
        {
            #if USEING_UART
            Serial.println(file_name + " FOUND");
            Serial.println(dataFile.size());
            #endif
        }
        else
        {
            #if USEING_UART
            Serial.print(file_name + " NOT FOUND");
            #endif
        }
        // dataFile.readBytes
        for (uint64_t xbm_num = 0; xbm_num < dataFile.size(); xbm_num++)
        {
            data_read = (char)dataFile.read();
            badapple_buf[data_len++] = 0xff - data_read; //ffmpge 处理后的xbm文件是相反的，所以用0xff减去对应的数值
            if (data_len == 1024)                        //分辨率 128*64
            {
                //34500 延时是对于15FPS的bin文件
                delayMicroseconds(delay_show_time); //不延时3029张一共用时107S，差不多FPS=30.7
                u8g2.clearBuffer();
                u8g2.drawXBM(0, 0, OLED_WIDTH, OLED_HEIGHT, badapple_buf);
                u8g2.sendBuffer();
                data_len = 0;
            }
            if (get_keymenu_event() == KEY_HIDDEN)
            {
                clear_keymenu_event();
                data_len = 0;
                hidden_mode += 1;
                if (hidden_mode > hidden_mode_max)
                    hidden_mode = 0;
                if (hidden_mode == 0)
                {
                    file_name = apple_bin;
                    delay_show_time = 165000;
                }
                else if (hidden_mode == 1)
                {
                    file_name = basket_bin;
                    delay_show_time = 135000;
                }
                else if (hidden_mode == 2)
                {
                    file_name = elephant_bin;
                    delay_show_time = 100000;
                }
                else if (hidden_mode == 3)
                {
                    file_name = plant_bin;
                    delay_show_time = 120000;
                }
                else if (hidden_mode == 4)
                {
                    file_name = space_bin;
                    delay_show_time = 180000;
                }
                break;
            }
            else if (get_keymenu_event() == KEY_CANCEL)
            {
                clear_keymenu_event();
                data_len = 0;
                exit_flag = 1;
                break;
            }
        }
        #if USEING_UART
        Serial.print("Play finish\r\n");
        #endif
        //完成文件读取后关闭文件
        dataFile.close();
        if (exit_flag == 1)
        {
            exit_flag = 0;
            break; //跳出循环
        }
        delay(50); //可以延时一下
    }
}
/*
函 数 名:void time_show_2(tmElements_t time)
功能说明:没有动画，直接1变成2
形    参:void
返 回 值:void
时    间：2020-1-16
RAiny
*/
void time_show_2(tmElements_t time)
{
    uint8_t data_y = 0,anima_minu_low = 30;
    uint8_t hour_high = 0, hour_low = 0, minu_high = 0, minu_low = 0, seco_high = 0,seco_low = 0;
    static uint8_t maohao_flag = 0;
    static uint8_t last_hour_high = 0xff, last_hour_low = 0xff, last_minu_high = 0xff, last_minu_low = 0xff;
    static uint8_t last_seco_high = 0xff, last_seco_low = 0xff;
    const uint8_t seco_high_x = 4 + 20 * 5, seco_low_x = 6 + 20 * 5 + 10;
    const uint8_t minu_high_x = 2 + 20 * 3 - 2, minu_low_x = 2 + 20 * 4;
    const uint8_t hour_high_x = 2, hour_low_x = 2 + 20 + 2;
    const uint8_t maohao_x = 2 + 20 * 2 + 7;
    const uint8_t time_y = 40;
    char data_arry[15] = {0};
    hour_high = time.Hour / 10;
    hour_low = time.Hour % 10;
    minu_high = time.Minute / 10;
    minu_low = time.Minute % 10;
    seco_high = time.Second / 10;
    seco_low = time.Second % 10;
    sprintf(data_arry, "%d/%d/%d", time.Year, time.Month, time.Day);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_smg10);//pixel hight 25 u8g2_font_freedoomr25_tn
    data_y = u8g2.getUTF8Width(data_arry);
    u8g2.drawStr((OLED_WIDTH - data_y) / 2, 64, data_arry);
    u8g2.setFont(u8g2_font_smg20);//pixel hight 25 u8g2_font_freedoomr25_tn
    u8g2.setCursor(hour_high_x,time_y);
    u8g2.print(hour_high);
    u8g2.setCursor(hour_low_x, time_y);
    u8g2.print(hour_low);
    u8g2.setCursor(maohao_x, time_y);
    u8g2.print(":");
    u8g2.setCursor(minu_high_x, time_y);
    u8g2.print(minu_high);
    u8g2.setCursor(minu_low_x, time_y);
    u8g2.print(minu_low);
    u8g2.setFont(u8g2_font_smg10);
    u8g2.setCursor(seco_high_x, time_y);
    u8g2.print(seco_high);
    u8g2.setCursor(seco_low_x, time_y);
    u8g2.print(seco_low);
    u8g2.sendBuffer();
    last_hour_high = hour_high;
    last_hour_low = hour_low;
    last_minu_high = minu_high;
    last_minu_low = minu_low;
}
void time_up_anima(const uint8_t *fontData,uint8_t x,uint8_t y,uint8_t space_x,uint8_t space_y,uint8_t data,uint8_t last_data)
{
    /* assign a clip window and draw some text into it */
    u8g2.setClipWindow(x, 40-space_y, x+space_x, 40);
    u8g2.setFont(fontData);
    u8g2.setCursor(x, y);
    u8g2.print(last_data);
    u8g2.setCursor(x, 40 + y);
    u8g2.print(data);
    /* remove clip window, draw to complete screen */
    u8g2.setMaxClipWindow();
}
/*
函 数 名:void time_show_3(tmElements_t time)
功能说明:动画，上下翻页的效果
形    参:void
返 回 值:void
时    间：2020-1-16
RAiny
*/
void time_show_3(tmElements_t time)
{
    uint8_t data_y = 0, anima_minu_low = 40, anima_y = 0;
    uint8_t hour_high = 0, hour_low = 0, minu_high = 0, minu_low = 0, seco_high = 0,seco_low = 0;
    static uint8_t maohao_flag = 0;
    static uint8_t last_hour_high = 0xff, last_hour_low = 0xff, last_minu_high = 0xff, last_minu_low = 0xff;
    static uint8_t last_seco_high = 0xff, last_seco_low = 0xff;
    const uint8_t seco_high_x = 4 + 20 * 5, seco_low_x = 6 + 20 * 5 + 10;
    const uint8_t minu_high_x = 2 + 20 * 3 - 2, minu_low_x = 2 + 20 * 4;
    const uint8_t hour_high_x = 2, hour_low_x = 2 + 20 + 2;
    const uint8_t maohao_x = 2 + 20 * 2 + 7;
    const uint8_t time_y = 40;
    char data_arry[15] = {0};
    hour_high = time.Hour / 10;
    hour_low = time.Hour % 10;
    minu_high = time.Minute / 10;
    minu_low = time.Minute % 10;
    seco_high = time.Second / 10;
    seco_low = time.Second % 10;
    sprintf(data_arry, "%d/%d/%d", time.Year, time.Month, time.Day);
    while (anima_minu_low > 0)
    {
        anima_y = anima_minu_low--;
        Serial.print(anima_minu_low);
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_smg10);//pixel hight 25 u8g2_font_freedoomr25_tn
        data_y = u8g2.getUTF8Width(data_arry);
        u8g2.drawStr((OLED_WIDTH - data_y) / 2, 64, data_arry);
        u8g2.setFont(u8g2_font_smg20);//pixel hight 25 u8g2_font_freedoomr25_tn
        u8g2.setCursor(maohao_x, time_y);
        u8g2.print(":");
        if (last_seco_low == seco_low)
        {
            u8g2.setFont(u8g2_font_smg10);
            u8g2.setCursor(seco_low_x, 40);
            u8g2.print(seco_low);
        }
        else
        {
            time_up_anima(u8g2_font_smg10, seco_low_x, anima_y, 10, 20, seco_low, last_seco_low);
            /* assign a clip window and draw some text into it */
            // u8g2.setClipWindow(seco_low_x, 20, seco_low_x+10, time_y);
            // u8g2.setFont(u8g2_font_smg10);
            // u8g2.setCursor(seco_low_x, anima_y);
            // u8g2.print(last_seco_low);
            // u8g2.setCursor(seco_low_x, 40+anima_y);
            // u8g2.print(seco_low);
            // /* remove clip window, draw to complete screen */
            // u8g2.setMaxClipWindow();
        }
        if(last_seco_high == seco_high)
        {
            u8g2.setFont(u8g2_font_smg10);
            u8g2.setCursor(seco_high_x, 40);
            u8g2.print(seco_high);
        }
        else
        {
            time_up_anima(u8g2_font_smg10, seco_high_x, anima_y, 10, 20, seco_high, last_seco_high);
        }
        if (last_minu_low == minu_low)
        {
            u8g2.setFont(u8g2_font_smg20);
            u8g2.setCursor(minu_low_x, 40);
            u8g2.print(minu_low);
        }
        else
        {
            time_up_anima(u8g2_font_smg20, minu_low_x, anima_y, 20, 40, minu_low, last_minu_low);
        }
        if (last_minu_high == minu_high)
        {
            u8g2.setFont(u8g2_font_smg20);
            u8g2.setCursor(minu_high_x, 40);
            u8g2.print(minu_high);
        }
        else
        {
            time_up_anima(u8g2_font_smg20, minu_high_x, anima_y, 20, 40, minu_high, last_minu_high);
        }
        if (last_hour_low == hour_low)
        {
            u8g2.setFont(u8g2_font_smg20);
            u8g2.setCursor(hour_low_x, 40);
            u8g2.print(hour_low);
        }
        else
        {
            time_up_anima(u8g2_font_smg20, hour_low_x, anima_y, 20, 40, hour_low, last_hour_low);
        }
        if (last_hour_high == hour_high)
        {
            u8g2.setFont(u8g2_font_smg20);
            u8g2.setCursor(hour_high_x, 40);
            u8g2.print(hour_high);
        }
        else
        {
            time_up_anima(u8g2_font_smg20, hour_high_x, anima_y, 20, 40, hour_high, last_hour_high);
        }
        u8g2.sendBuffer();
        delay(20);//动作时间长了过1S，S的显示就不连续
    }
    last_hour_high = hour_high;
    last_hour_low = hour_low;
    last_minu_high = minu_high;
    last_minu_low = minu_low;
    last_seco_high = seco_high;
    last_seco_low = seco_low;
}
/*
函 数 名:void time_show_4(tmElements_t time)
功能说明:动画，ScrollingTime
形    参:void
返 回 值:void
时    间：2020-3-20
RAiny
*/
void time_show_4(tmElements_t time)
{
    char bless_arry[30] = {0};
    char time_arry[15] = {0};
    static int16_t offset=0;
    uint16_t bless_width;
    uint16_t time_width;
    static uint16_t bless_x;
    uint8_t hour_high = 0, hour_low = 0, minu_high = 0, minu_low = 0, seco_high = 0,seco_low = 0;
    hour_high = time.Hour / 10;
    hour_low = time.Hour % 10;
    minu_high = time.Minute / 10;
    minu_low = time.Minute % 10;
    seco_high = time.Second / 10;
    seco_low = time.Second % 10;
    sprintf(bless_arry, "%d Rich Happy Healthy ", time.Year);
    sprintf(time_arry, "%d%d:%d%d:%d%d", hour_high, hour_low, minu_high, minu_low, seco_high, seco_low);
    u8g2.clearBuffer();
    bless_x = offset;
    u8g2.setFont(u8g2_font_inb30_mr);
    bless_width = u8g2.getStrWidth(bless_arry);//打开#ifdef U8G2_16BIT 宏定义
    do {
        u8g2.drawStr(bless_x, 30, bless_arry);
        bless_x += bless_width;//接着后面
    } while( bless_x < OLED_WIDTH);
    u8g2.setFont(u8g2_font_inb16_mn);
    time_width = u8g2.getStrWidth(time_arry);
    u8g2.setCursor((OLED_WIDTH-time_width)/2, 58);
    u8g2.print(time_arry);
    u8g2.sendBuffer();
    offset-=1;
    if ( (uint16_t)offset < (uint16_t)-bless_width )
        offset = 0;
}
/*
函 数 名:void time_show_5(tmElements_t time)
功能说明:矩形抽象
形    参:void
返 回 值:void
时    间：2020-3-21
RAiny
*/
void time_show_5(tmElements_t time)
{
    u8g2.clearBuffer();
    u8g2.drawBox(0,2,time.Hour*5.3,20);
    u8g2.drawBox(0,22,time.Minute*2.1,20);
    u8g2.drawBox(0,42,time.Second*2.1,20);
    u8g2.sendBuffer();
}
/*
函 数 名:void time_show_5(tmElements_t time)
功能说明:模拟指针
        x1 = x0 + r * cos(angle * PI / 180)
        y1 = y0 + r * sin(angle * PI /180)
形    参:void
返 回 值:void
时    间：2020-3-21
RAiny
*/
void time_show_6(tmElements_t time)
{
    const float pi = 3.1415f;
    const uint8_t angle_diff = 20;//差值角度
    const uint16_t r = OLED_HEIGHT / 2;
    const uint16_t x0 = OLED_WIDTH / 2, y0 = OLED_HEIGHT / 2;
    uint16_t x_minu, y_minu, x1_hour, y1_hour, x2_hour, y2_hour, x_num, y_num;
    int16_t angle_minu = 0, angle_hour = 0;
    uint16_t time_width;
    uint8_t time_hour;
    if (time.Hour==12||time.Hour==0)
    {
        time_hour = 0;
        angle_hour = -90;
        x1_hour = x0 + r * cosf((angle_hour - angle_diff) * pi / 180);
        y1_hour = y0 + r * sinf((angle_hour - angle_diff) * pi / 180);
        x2_hour = x0 + r * cosf((angle_hour + angle_diff) * pi / 180);
        y2_hour = y0 + r * sinf((angle_hour + angle_diff) * pi / 180);
        x_num = x0 + (r / 2) * cosf(angle_hour * pi / 180);
        y_num = y0 + (r / 2) * sinf(angle_hour * pi / 180);
    }
    else
    {
        if(time.Hour>12)
            time_hour = time.Hour - 12;
        else
            time_hour = time.Hour;
        angle_hour = time_hour * 30 - 90;
        x1_hour = x0 + r * cosf((angle_hour - angle_diff) * pi / 180);
        y1_hour = y0 + r * sinf((angle_hour - angle_diff) * pi / 180);
        x2_hour = x0 + r * cosf((angle_hour + angle_diff) * pi / 180);
        y2_hour = y0 + r * sinf((angle_hour + angle_diff) * pi / 180);
        x_num = x0 + (r / 2) * cosf(angle_hour * pi / 180);
        y_num = y0 + (r / 2) * sinf(angle_hour * pi / 180);
    }
    angle_minu = time.Minute * 6 - 90;
    x_minu = x0 + r * cosf(angle_minu * pi / 180);
    y_minu = y0 + r * sinf(angle_minu * pi / 180);

    u8g2.clearBuffer();
    u8g2.drawLine(x0, y0, x_minu, y_minu);
    u8g2.drawTriangle(x0, y0, x1_hour, y1_hour, x2_hour, y2_hour);
    u8g2.setFont(u8g2_font_tenfatguys_tu);
    if(time_hour>=10)
        time_width = u8g2.getStrWidth("10");
    else
        time_width = u8g2.getStrWidth("1");
    switch (time_hour)
    {
        case 0:
            u8g2.setCursor(x_num - (time_width) / 2 - 3, y_num); //0
            break;
        case 1:
            u8g2.setCursor(x_num - (time_width) / 2 + 2, y_num); //1
            break;
        case 2:
            u8g2.setCursor(x_num - (time_width) / 2 + 2, y_num + 2);//2
            break;
        case 3:
            u8g2.setCursor(x_num - (time_width) / 2 + 2, y_num + 4);//3
            break;
        case 4:
            u8g2.setCursor(x_num - (time_width) / 2 + 2, y_num + 9);//4
            break;
        case 5:
            u8g2.setCursor(x_num - (time_width) / 2 + 1, y_num + 10);//5
            break;
        case 6:
            u8g2.setCursor(x_num - (time_width) / 2 - 2, y_num + 12);//6
            break;
        case 7:
            u8g2.setCursor(x_num - (time_width) / 2 - 6, y_num + 12);//7
            break;
        case 8:
            u8g2.setCursor(x_num - (time_width) / 2 - 8, y_num + 8);//8
            break;
        case 9:
            u8g2.setCursor(x_num - (time_width) / 2 - 8, y_num + 6);//9
            break;
        case 10:
            u8g2.setCursor(x_num - (time_width) / 2 - 4, y_num + 4);//10
            break;
        case 11:
            u8g2.setCursor(x_num - (time_width) / 2 + 3, y_num + 4);//11
            break;
        default:
            break;
    }
    u8g2.print(time_hour);
    u8g2.drawBox(0, 0, OLED_WIDTH, OLED_HEIGHT);//drawFrame
    u8g2.sendBuffer();

}
/*
函 数 名:void time_ipdate_anima(void)
功能说明:时间数字更新动画
形    参:void
返 回 值:void
时    间：2020-12-26
RAiny
*/
void time_ipdate_anima(uint8_t x, uint8_t y, uint8_t bin_num)
{
    char file_name_buff[10];
    char data_read;
    static int data_len = 0;
    //建立File对象用于从SPIFFS中读取文件
    sprintf(file_name_buff, "/%d.bin", bin_num);
    String file_name = file_name_buff;
    dataFile = SPIFFS.open(file_name, "r");
    //确认闪存中是否有file_name文件
    if (SPIFFS.exists(file_name))
    {
        Serial.println(file_name + " FOUND");
        Serial.println(dataFile.size());
    }
    else
    {
        Serial.print(file_name + " NOT FOUND");
    }
    // dataFile.readBytes
    for (uint64_t xbm_num = 0; xbm_num < dataFile.size(); xbm_num++)
    {
        data_read = (char)dataFile.read();
        badapple_buf[data_len++] = data_read;
        if (data_len == 168) //分辨率 25*42
        {
            delay(150);
            u8g2.clearBuffer();
            u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, badapple_buf);
            // u8g2.sendBuffer();
            //局部刷新有个问题就是不能全部覆盖xbm图片，并不是8的完整倍数
            //局部刷新要/8
            if (x == HOUR_HIGH_X)
            {
                u8g2.updateDisplayArea(0, y / 8, NUM_WIDTH / 8, NUM_HEIGHT / 8);
            }
            else if (x == (HOUR_LOW_X))
            {
                u8g2.updateDisplayArea(3, y / 8, NUM_WIDTH / 8 + 1, NUM_HEIGHT / 8);
            }
            else if (x == (MINU_HIGH_X))
            {
                u8g2.updateDisplayArea(10, y / 8, NUM_WIDTH / 8, NUM_HEIGHT / 8);
            }
            else if (x == (MINU_LOW_X))
            {
                u8g2.updateDisplayArea(13, y / 8, NUM_WIDTH / 8, NUM_HEIGHT / 8);
            }
            data_len = 0;
        }
    }
    //完成文件读取后关闭文件
    dataFile.close();
    delay(200);
}
/*
函 数 名:void time_select_draw(uint8_t x,uint8_t y,uint8_t num)
功能说明:将时间分为十位和个位显示手写数字xbm图片
形    参:uint8_t x：x坐标
        uint8_t y：y坐标
        uint8_t num：显示数字 大于等于10为空白显示
返 回 值:void
时    间：2020-12-26
RAiny
*/
void time_select_draw(uint8_t x, uint8_t y, uint8_t num)
{
    switch (num)
    {
    case 0:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num0);
        break;
    case 1:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num1);
        break;
    case 2:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num2);
        break;
    case 3:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num3);
        break;
    case 4:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num4);
        break;
    case 5:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num5);
        break;
    case 6:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num6);
        break;
    case 7:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num7);
        break;
    case 8:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num8);
        break;
    case 9:
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_num9);
        break;
    case 10: //显示冒号：
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, write_maohao);
        break;
    default: //其它均不显示
        u8g2.drawXBM(x, y, NUM_WIDTH, NUM_HEIGHT, num_kongbai);
        break;
    }
}
/*
函 数 名:void time_show_1(tmElements_t time)
功能说明:显示手写数字时间，将数字对应的xbm图片显示
形    参:tmElements_t time：tmElements_t结构体
返 回 值:void
时    间：2020-12-26
RAiny
*/
void time_show_1(tmElements_t time)
{
    uint8_t hour_high = 0, hour_low = 0, minu_high = 0, minu_low = 0;
    static uint8_t maohao_flag = 0;
    static uint8_t last_hour_high = 0xff, last_hour_low = 0xff, last_minu_high = 0xff, last_minu_low = 0xff;
    hour_high = time.Hour / 10;
    hour_low = time.Hour % 10;
    minu_high = time.Minute / 10;
    minu_low = time.Minute % 10;
    u8g2.clearBuffer();
    if (last_minu_low == minu_low) //不去更新
    {
        time_select_draw(MINU_LOW_X, TIME_XBM_HEIGHT, minu_low);
    }
    if (last_minu_high == minu_high)
    {
        time_select_draw(MINU_HIGH_X, TIME_XBM_HEIGHT, minu_high);
    }
    if (maohao_flag == 0)
    {
        time_select_draw(MAOHAO_X, TIME_XBM_HEIGHT, 10);
        maohao_flag = 1;
    }
    else if (maohao_flag == 1)
    {
        time_select_draw(MAOHAO_X, TIME_XBM_HEIGHT, 11);
        maohao_flag = 0;
    }
    if (last_hour_low == hour_low)
    {
        time_select_draw(HOUR_LOW_X, TIME_XBM_HEIGHT, hour_low);
    }
    if (last_hour_high == hour_high)
    {
        time_select_draw(HOUR_HIGH_X, TIME_XBM_HEIGHT, hour_high);
    }
    u8g2.sendBuffer();
    //在这里更新动画
    if (last_minu_low != minu_low)
    {
        time_ipdate_anima((MINU_LOW_X), TIME_XBM_HEIGHT, minu_low);
    }
    if (last_minu_high != minu_high)
    {
        time_ipdate_anima((MINU_HIGH_X), TIME_XBM_HEIGHT, minu_high);
    }
    if (last_hour_low != hour_low)
    {
        time_ipdate_anima((HOUR_LOW_X), TIME_XBM_HEIGHT, hour_low);
    }
    if (last_hour_high != hour_high)
    {
        time_ipdate_anima(HOUR_HIGH_X, TIME_XBM_HEIGHT, hour_high);
    }
    last_hour_high = hour_high;
    last_hour_low = hour_low;
    last_minu_high = minu_high;
    last_minu_low = minu_low;
}
/*
函 数 名:void time_update(void)
功能说明:NTP Time 更新 周期100ms
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
void time_update(void)
{
    tmElements_t time;
    uint32_t delay_time;
    set_rgb_val(254, 67, 101);
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            u8g2.setFont(u8g2_font_unifont_t_shopl16); //自己制作的字体
            u8g2.firstPage();
            do
            {
                u8g2.setCursor(10, 36);
                u8g2.print("No WiFi");
            } while (u8g2.nextPage());
        }
        else
        {
            timeClient.update();
            unsigned long unix_epoch = timeClient.getEpochTime();
            time.Second = second(unix_epoch);
            time.Minute = minute(unix_epoch);
            time.Hour = hour(unix_epoch);
            time.Wday = weekday(unix_epoch);
            time.Day = day(unix_epoch);
            time.Month = month(unix_epoch);
            time.Year = year(unix_epoch);
            // #if USEING_UART
            // Serial.print(" Minute is ");
            // Serial.print(time.Minute);
            // Serial.print(" Hour is ");
            // Serial.print(time.Hour);
            // Serial.print(" Wday is ");
            // Serial.print(time.Wday);
            // Serial.print(" Day is ");//Sunday is day 1
            // Serial.print(time.Day);
            // Serial.print(" Month is ");
            // Serial.print(time.Month);
            // Serial.print(" Year is ");
            // Serial.print(time.Year);
            // Serial.print("\r\n");
            // #endif
            if(eeprom.data.clock_mode == 0)
            {
                time_show_1(time);
                delay_time = 500;
            }
            else if(eeprom.data.clock_mode == 1)
            {
                time_show_2(time);
                delay_time = 500;
            }
            else if(eeprom.data.clock_mode == 2)
            {
                time_show_3(time);
                delay_time = 20;
            }
            else if(eeprom.data.clock_mode == 3)
            {
                time_show_4(time);
                delay_time = 30;
            }
            else if(eeprom.data.clock_mode == 4)
            {
                time_show_5(time);
                delay_time = 100;
            }
            else if(eeprom.data.clock_mode == 5)
            {
                time_show_6(time);
                delay_time = 1000;
            }
        }
        if (get_keymenu_event() == KEY_HIDDEN)
        {
            eeprom.data.clock_mode += 1;
            if(eeprom.data.clock_mode > CLOCK_MAX_MODE)
            {
                eeprom.data.clock_mode = 0;
            }
            eeprom_write();
            clear_keymenu_event();
        }
        if (get_keymenu_event() == KEY_CANCEL)
        {
            clear_keymenu_event();
            break;
        }
        delay(delay_time);
    }
}
/*
函 数 名:void web_introduce(void)
功能说明:Web访问
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void web_introduce(void)
{
    char qrcode_buff[25];
    QRCode qrcode; //gen the QR code
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    set_rgb_val(229, 131, 8);
    if (WiFi.status() != WL_CONNECTED)
    {
        u8g2.setFont(u8g2_font_unifont_t_shopl16); //自己制作的字体
        u8g2.firstPage();
        do
        {
            u8g2.setCursor(10, 36);
            u8g2.print("No WiFi");
        } while (u8g2.nextPage());
    }
    else
    {
        esp8266_server.onNotFound(handleUserRequet); // 告知系统如何处理用户请求
        esp8266_server.begin();                      // 启动网站服务
        String localIP = WiFi.localIP().toString();
        // Serial.print(localIP);
        sprintf(qrcode_buff, "http://%s/", localIP.c_str());
        Serial.print(qrcode_buff);
        qrcode_initText(&qrcode, qrcodeData, 3, ECC_QUARTILE, qrcode_buff);
        // start draw
        u8g2.clearBuffer();
        // get the draw starting point,128 and 64 is screen size
        uint8_t x0 = (OLED_WIDTH - qrcode.size * 2) / 2;
        uint8_t y0 = (OLED_HEIGHT - qrcode.size * 2) / 2;
        // get QR code pixels in a loop
        for (uint8_t y = 0; y < qrcode.size; y++)
        {
            for (uint8_t x = 0; x < qrcode.size; x++)
            {
                // Check this point is black or white
                if (qrcode_getModule(&qrcode, x, y))
                {
                    u8g2.setColorIndex(1); //1：表示显示，不透明
                }
                else
                {
                    u8g2.setColorIndex(0); //0：表示不显示，透明。
                }
                // Double the QR code pixels
                u8g2.drawPixel(x0 + x * 2, y0 + y * 2);
                u8g2.drawPixel(x0 + 1 + x * 2, y0 + y * 2);
                u8g2.drawPixel(x0 + x * 2, y0 + 1 + y * 2);
                u8g2.drawPixel(x0 + 1 + x * 2, y0 + 1 + y * 2);
            }
        }
        u8g2.sendBuffer();
    }
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
        }
        else
        {
            esp8266_server.handleClient(); // 处理用户请求
        }
        if (get_keymenu_event() == KEY_CANCEL)
        {
            esp8266_server.stop();
            clear_keymenu_event();
            u8g2.setColorIndex(1); //退出恢复 1实显
            break;
        }
        delay(10);
    }
}
/*
函 数 名:void config(void)
功能说明:设置
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
void config(void)
{
    static uint8_t func_index = 0;
    set_rgb_val(224, 160, 158);
    while (1)
    {
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            func_index = config_list[func_index].down;
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            func_index = config_list[func_index].up;
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            func_index = config_list[func_index].enter;
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            clear_keymenu_event();
            if (func_index < SETING_FIRST_MENU) //设置的一级菜单
            {
                func_index = 0;
                config_last_state = 0;
                config_state = 0;
                break;
            }
            else
            {
                func_index = config_list[func_index].exit;
            }
        }
        config_operation_index = config_list[func_index].current_operation;
        (*config_operation_index)();
        delay(10);
    }
}
/*
函 数 名:void key_check(void)
功能说明:按键检测函数 周期10ms
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void key_check(void)
{
    s_button.tick();
    c_button.tick();
}
/*
函 数 名:void rgb_led_run(void)
功能说明:RGB_LED 周期30ms
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
void rgb_led_run(void)
{
    rgb.r_val--;
    rgb.g_val--;
    rgb.b_val--;
    if (rgb.r_val == 0)
        rgb.r_val = rgb_save.r_val;
    if (rgb.g_val == 0)
        rgb.g_val = rgb_save.g_val;
    if (rgb.b_val == 0)
        rgb.b_val = rgb_save.b_val;
    rgb_led_set(rgb);
}
/*
函 数 名:void select_menu(void)
功能说明:选择菜单
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void select_menu(void)
{
    KEY_EVENT_INF menu_event = KEY_NOEVENT;
    static uint8_t init_menu = 0;
    do
    {
        menu_event = get_keymenu_event();
        if (init_menu == 0)
        {
            if (WiFi.status() != WL_CONNECTED) //如果没有WIFI
            {
                menu_event = KEY_NEXT; //进入BadApple
            }
            else
            {
                //进入时间
            }
            init_menu = 1;
        }
        else if (init_menu == 1)
        {
            menu_event = KEY_CONFIRM;
            init_menu = 2;
        }
        u8g2.clearBuffer();
        draw(&home_state);
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(menu_entry_list[home_last_state.position].name)) / 2, u8g2.getDisplayHeight() - 5);
        u8g2.print(menu_entry_list[home_last_state.position].name);
        u8g2.sendBuffer();
        if (menu_event == KEY_NEXT)
        {
            clear_keymenu_event();
            to_right(&home_last_state);
        }
        else if (menu_event == KEY_PRVE)
        {
            clear_keymenu_event();
            to_left(&home_last_state);
        }
        else if (menu_event == KEY_CONFIRM)
        {
            clear_keymenu_event();
            current_operation_index = menu_entry_list[home_last_state.position].current_operation;
            (*current_operation_index)(); //执行当前操作函数
        }
        delay(10);
    } while (towards(&home_state, &home_last_state));
}
/*
函 数 名:void eeprom_read(void)
功能说明:EEPROM 读函数，全部读出
形    参:void
返 回 值:void
时    间：2020-1-2
RAiny
*/
void eeprom_read(void)
{
    for (uint16_t i = 0; i < EEPROM_SIZE; i++)
        eeprom.arry[i] = EEPROM.read(i);
    // Serial.print("eeprom.data.clock_mode is ");
    // Serial.print(eeprom.data.clock_mode);
    // Serial.print("\r\n");
    //第一次烧写程序Flash里面的参数不对应
    if(eeprom.data.clock_mode>CLOCK_MAX_MODE)
    {
        eeprom.data.clock_mode = 0;
        eeprom.data.led_on = 0;
        eeprom_write();
    }
}
/*
函 数 名:void eeprom_write(void)
功能说明:EEPROM 写函数
形    参:void
返 回 值:void
时    间：2020-1-2
RAiny
*/
void eeprom_write(void)
{
    for (uint16_t i = 0; i < EEPROM_SIZE; i++)
        EEPROM.write(i, eeprom.arry[i]);
    if (EEPROM.commit())
    {
        Serial.println("EEPROM successfully committed");
    }
    else
    {
        Serial.println("ERROR! EEPROM commit failed");
    }
}
/*
函 数 名:void setup(void)
功能说明:ESP8266 初始化函数
形    参:void
返 回 值:void
时    间：2020-12-20
RAiny
*/
void setup(void)
{
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE); //开启EEPROM，开辟64个位空间
    eeprom_read();
    u8g2.begin();
    u8g2.enableUTF8Print();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    if (SPIFFS.begin()) // 启动闪存文件系统
    {
        Serial.println("SPIFFS Started.");
    }
    else
    {
        Serial.println("SPIFFS Failed to Start.");
    }
    //SPIFFS.format();    // 格式化SPIFFS
    rgb_led_init();
    led_tick.attach_ms(1000, rgb_led_run);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
    u8g2.firstPage();
    do
    {
        u8g2.setCursor(10, 14);
        u8g2.print("连接WiFi中...");
        u8g2.setCursor(10, 28);
        u8g2.print("如果长时间没有进入");
        u8g2.setCursor(10, 42);
        u8g2.print("1.连接LambdaTV");
        u8g2.setCursor(10, 56);
        u8g2.print("2.打开网页配置WiFi");
    } while (u8g2.nextPage());
    // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
    // wifiManager.resetSettings();
    wifiManager.setTimeout(60); //配置超时时间S
    // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
    if (!wifiManager.autoConnect("LambdaTV")) //xzh1978n1120abc
    {
        //没有WiFi
        Serial.println("No WiFi");
    }
    else
    {
        // 如果您希望该WiFi添加密码，可以使用以下语句：
        // wifiManager.autoConnect("AutoConnectAP", "12345678");
        // 以上语句中的12345678是连接AutoConnectAP的密码
        // WiFi连接成功后将通过串口监视器输出连接成功信息
        Serial.print("ESP8266 Connected to ");
        Serial.println(WiFi.SSID());
        Serial.print("IP address:\t");
        Serial.println(WiFi.localIP());
        timeClient.begin();
    }
    print_fs_info();
    key_init();
    LambdaTV();
    key_tick.attach_ms(10, key_check);
}
/*
函 数 名:void loop(void)
功能说明:ESP8266 循环函数
形    参:void
返 回 值:void
时    间：2020-12-20
RAiny
*/
void loop(void)
{
    select_menu();
}

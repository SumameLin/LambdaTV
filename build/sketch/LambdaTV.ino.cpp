#include <Arduino.h>
#line 1 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
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
        {NULL, 0, NULL, NULL}};
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
#line 271 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void print_fs_info(void);
#line 303 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void LambdaTV(void);
#line 413 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_show_2(tmElements_t time);
#line 459 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_up_anima(const uint8_t *fontData,uint8_t x,uint8_t y,uint8_t space_x,uint8_t space_y,uint8_t data,uint8_t last_data);
#line 479 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_show_3(tmElements_t time);
#line 597 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_ipdate_anima(uint8_t x, uint8_t y, uint8_t bin_num);
#line 662 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_select_draw(uint8_t x, uint8_t y, uint8_t num);
#line 712 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_show_1(tmElements_t time);
#line 991 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void key_check(void);
#line 1004 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void rgb_led_run(void);
#line 1025 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void select_menu(void);
#line 1082 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void eeprom_read(void);
#line 1095 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void eeprom_write(void);
#line 1116 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void setup(void);
#line 1182 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void loop(void);
#line 11 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void s_click(void);
#line 24 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void s_doubleclick(void);
#line 36 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void s_longclick(void);
#line 49 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_click(void);
#line 62 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_doubleclick(void);
#line 75 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_longclick(void);
#line 88 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
KEY_EVENT_INF get_keymenu_event(void);
#line 102 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void set_keymenu_event(KEY_EVENT_INF KEY);
#line 114 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void clear_keymenu_event(void);
#line 126 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void key_init(void);
#line 143 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void rgb_led_init(void);
#line 160 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void rgb_led_set(RGB_INF rgb_set);
#line 185 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void set_rgb_val(uint8_t r_val, uint8_t g_val, uint8_t b_val);
#line 15 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void draw(menu_state *state);
#line 51 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void to_right(menu_state *state);
#line 77 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void to_left(menu_state *state);
#line 102 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
uint8_t towards_int16(int16_t *current, int16_t dest);
#line 124 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
uint8_t towards(menu_state *current, menu_state *destination);
#line 141 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void config_fun(int8_t state);
#line 163 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void clock_mode();
#line 447 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void select_cancel_anima(const char *title, const char *content, uint8_t x1);
#line 13 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_server.ino"
void handleUserRequet();
#line 33 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_server.ino"
bool handleFileRead(String path);
#line 57 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_server.ino"
String getContentType(String filename);
#line 271 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
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
    static int data_len = 0;
    static uint8_t exit_flag = 0;
    uint8_t hidden_mode = 0;
    //建立File对象用于从SPIFFS中读取文件
    String file_name = apple_bin;
    set_rgb_val(69, 137, 148);
    while (1)
    {
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
            badapple_buf[data_len++] = 0xff - data_read; //ffmpge 处理后的xbm文件是相反的，所以用0xff减去对应的数值
            if (data_len == 1024)                        //分辨率 128*64
            {
                //34500 延时是对于15FPS的bin文件
                delayMicroseconds(165000); //不延时3029张一共用时107S，差不多FPS=30.7
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
                if (hidden_mode > 1)
                    hidden_mode = 0;
                if (hidden_mode == 0)
                    file_name = apple_bin;
                else if (hidden_mode == 1)
                    file_name = basket_bin;
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
        Serial.print("Play finish\r\n");
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
        delay(30);//动作时间长了过1S，S的显示就不连续
    }
    last_hour_high = hour_high;
    last_hour_low = hour_low;
    last_minu_high = minu_high;
    last_minu_low = minu_low;
    last_seco_high = seco_high;
    last_seco_low = seco_low;
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
            Serial.print(" Minute is ");
            Serial.print(time.Minute);
            Serial.print(" Hour is ");
            Serial.print(time.Hour);
            Serial.print(" Wday is ");
            Serial.print(time.Wday);
            Serial.print(" Day is ");//Sunday is day 1
            Serial.print(time.Day);
            Serial.print(" Month is ");
            Serial.print(time.Month);
            Serial.print(" Year is ");
            Serial.print(time.Year);
            Serial.print("\r\n");
            if(eeprom.data.clock_mode == 0)
            {
                time_show_1(time);
                delay_time = 1000;
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

#line 1 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
#include "Arduino.h"
#include "LambdaTV.h"
/*
函 数 名:void s_click(void)
功能说明:s键按一下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_click(void)
{
    lambdaTV.key_event = KEY_CONFIRM;
    Serial.println("s_click");
}
/*
函 数 名:void s_doubleclick(void)
功能说明:s键按两下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_doubleclick(void)
{
    Serial.println("s_doubleclick");
}
/*
函 数 名:void s_longclick(void)
功能说明:s键长按
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_longclick(void)
{
    lambdaTV.key_event = KEY_CANCEL;
    Serial.println("s_longclick");
}
/*
函 数 名:void c_click(void)
功能说明:c键按一下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_click(void)
{
    lambdaTV.key_event = KEY_NEXT;
    Serial.println("c_click");
}
/*
函 数 名:void c_doubleclick(void)
功能说明:c键按两下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_doubleclick(void)
{
    lambdaTV.key_event = KEY_PRVE;
    Serial.println("c_doubleclick");
}
/*
函 数 名:void c_longclick(void)
功能说明:c键长按
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_longclick(void)
{
    lambdaTV.key_event = KEY_HIDDEN;
    Serial.println("c_longclick");
}
/*
函 数 名:KEY_EVENT_INF get_keymenu_event(void)
功能说明:获取按键事件
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
KEY_EVENT_INF get_keymenu_event(void)
{
    KEY_EVENT_INF event;
    event = lambdaTV.key_event;
    return event;
}
/*
函 数 名:void set_keymenu_event(KEY_EVENT_INF KEY)
功能说明:设置按键事件
形    参:void
返 回 值:void
时    间：2020-1-5
RAiny
*/
void set_keymenu_event(KEY_EVENT_INF KEY)
{
    lambdaTV.key_event = KEY;
}
/*
函 数 名:void clear_keymenu_event(void)
功能说明:清除按键事件
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
void clear_keymenu_event(void)
{
    lambdaTV.key_event = KEY_NOEVENT;
}
/*
函 数 名:void key_init(void)
功能说明:按键初始化，用了OneButton库
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void key_init(void)
{
    s_button.attachClick(s_click);
    s_button.attachDoubleClick(s_doubleclick);
    s_button.attachLongPressStart(s_longclick);
    c_button.attachClick(c_click);
    c_button.attachDoubleClick(c_doubleclick);
    c_button.attachLongPressStart(c_longclick);
}
/*
函 数 名:void rgb_led_init(void)
功能说明:RGB LED初始化 IO占用了串口
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void rgb_led_init(void)
{
    #if USEING_UART
    pinMode(RGB_R_PIN, OUTPUT);
    #endif
    pinMode(RGB_G_PIN, OUTPUT);
    pinMode(RGB_B_PIN, OUTPUT);
    set_rgb_val(255, 255, 255);
}
/*
函 数 名:void rgb_led_set(RGB_INF rgb_set)
功能说明:直插三脚RGB共阳极
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
void rgb_led_set(RGB_INF rgb_set)
{
    if(eeprom.data.led_on)
    {
        #if USEING_UART
        analogWrite(RGB_R_PIN, 1024 - rgb_set.r_val * 4);
        #endif
        analogWrite(RGB_G_PIN, 1024 - rgb_set.g_val * 4);
        analogWrite(RGB_B_PIN, 1024 - rgb_set.b_val * 4);
    }
    else
    {
        analogWrite(RGB_R_PIN, 1024);
        analogWrite(RGB_G_PIN, 1024);
        analogWrite(RGB_B_PIN, 1024);
    }
}
/*
函 数 名:void set_rgb_val(uint8_t r_val,uint8_t g_val,uint8_t b_val)
功能说明:设置RGB的数值，呼吸效果
形    参:void
返 回 值:void
时    间：2020-12-31
RAiny
*/
void set_rgb_val(uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
    rgb_save.r_val = rgb.r_val = r_val;
    rgb_save.g_val = rgb.g_val = g_val;
    rgb_save.b_val = rgb.b_val = b_val;
}
#line 1 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
#include "Arduino.h"
#include "LambdaTV.h"
#include <U8g2lib.h>
/*
U8g2的IconMenu.ino example
*/
/*
函 数 名:void draw(menu_state *state)
功能说明:绘制字体字集的符号和选中框
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void draw(menu_state *state)
{
    int16_t x;
    uint8_t i;
    const uint8_t wid_max = 3, hv_len = 8;
    x = state->menu_start;
    i = 0;
    while (menu_entry_list[i].icon > 0)
    {
        if (x >= -ICON_WIDTH && x < u8g2.getDisplayWidth())
        {
            u8g2.setFont(menu_entry_list[i].font);
            u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon);
        }
        i++;
        x += ICON_WIDTH + ICON_GAP;
    }
    for (uint8_t wid = 0; wid < wid_max; wid++) //多条直线重叠，来实现线的宽度
    {
        u8g2.drawHVLine(state->frame_position - 1, ICON_Y - ICON_HEIGHT - 3 - wid, ICON_WIDTH + 2, 0);                             //往上增加
        u8g2.drawHVLine(state->frame_position - 2 - wid, ICON_Y - ICON_HEIGHT - 3 + hv_len - wid_max, hv_len, 3);                  //往左增加 模式3是从下往上画
        u8g2.drawHVLine(state->frame_position - 1 + ICON_WIDTH + 2 + wid, ICON_Y - ICON_HEIGHT - 3 + hv_len - wid_max, hv_len, 3); //往右增加

        u8g2.drawHVLine(state->frame_position - 1, ICON_Y + 3 + wid, ICON_WIDTH + 2, 0);                        //往下增加
        u8g2.drawHVLine(state->frame_position - 2 - wid, ICON_Y + 3 + wid_max - 1, hv_len, 3);                  //往左增加
        u8g2.drawHVLine(state->frame_position - 1 + ICON_WIDTH + 2 + wid, ICON_Y + 3 + wid_max - 1, hv_len, 3); //往右增加
    }
}
/*
函 数 名:void to_right(menu_state *state)
功能说明:向右移动
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void to_right(menu_state *state)
{
    if (menu_entry_list[state->position + 1].font != NULL)
    {
        //小于128
        if ((int16_t)state->frame_position + 2 * (int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth()) //获取显示器的宽度
        {
            state->position++;
            state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
        }
        else
        {
            state->position++;
            state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}
/*
函 数 名:void to_left(menu_state *state)
功能说明:向左移动
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void to_left(menu_state *state)
{
    if (state->position > 0)
    {
        if ((int16_t)state->frame_position >= (int16_t)ICON_BGAP + (int16_t)ICON_WIDTH + (int16_t)ICON_GAP)
        {
            state->position--;
            state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
        }
        else
        {
            state->position--;
            state->frame_position = ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}
/*
函 数 名:uint8_t towards_int16(int16_t *current, int16_t dest)
功能说明:
形    参:void
返 回 值:void
时    间：2020-12-30
RAiny
*/
uint8_t towards_int16(int16_t *current, int16_t dest)
{
    if (*current < dest)
    {
        (*current)++;
        return 1;
    }
    else if (*current > dest)
    {
        (*current)--;
        return 1;
    }
    return 0;
}
/*
函 数 名:uint8_t towards(menu_state *current,menu_state *destination)
功能说明:
形    参:void
返 回 值:void
时    间：2020-12-30
RAiny
*/
uint8_t towards(menu_state *current, menu_state *destination)
{
    uint8_t r = 0;
    r |= towards_int16(&(current->frame_position), destination->frame_position);
    r |= towards_int16(&(current->frame_position), destination->frame_position);
    r |= towards_int16(&(current->menu_start), destination->menu_start);
    r |= towards_int16(&(current->menu_start), destination->menu_start);
    return r;
}
/*
函 数 名:void config_fun(void)
功能说明:主显示菜单
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void config_fun(int8_t state)
{
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2.setCursor(5, 16 * state - 2);
    u8g2.print("时钟模式");
    u8g2.setCursor(5, 16 * (state + 1) - 2);
    u8g2.print("设置RGB_LED");
    u8g2.setCursor(5, 16 * (state + 2) - 2);
    u8g2.print("清除WiFi");
    u8g2.setCursor(5, 16 * (state + 3) - 2);
    u8g2.print("操作说明");
    u8g2.setCursor(5, 16 * (state + 4) - 2);
    u8g2.print("关于");
}
/*
函 数 名:void clock_mode(void)
功能说明:时钟模式
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clock_mode()
{
    config_state = 0;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1);
            u8g2.setDrawColor(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.setDrawColor(2);
            config_fun(1);
            u8g2.setCursor(110, 16 * 1 - 2);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE)
                u8g2.print("0");
            else
                u8g2.print(eeprom.data.clock_mode);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1);
            u8g2.setDrawColor(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.setDrawColor(2);
            config_fun(1);
            u8g2.setCursor(110, 16 * 1 - 2);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE)
                u8g2.print("0");
            else
                u8g2.print(eeprom.data.clock_mode);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void instrustions(void)
功能说明:LambdaTV介绍
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void instrustions(void)
{
    config_state = 16 * 3;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void clear_wifi(void)
功能说明:清除WIFI信息
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clear_wifi(void)
{
    config_state = 16 * 2;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void config_about(void)
功能说明:关于
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void config_about(void)
{
    config_state = 16 * 3;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(0);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(0);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void instrustions_enter(void)
功能说明:进入介绍页面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void instrustions_enter(void)
{
    const char *instru_title = "操作说明";
    const char *instru_1 = "单击C 下一步";
    const char *instru_2 = "双击C 上一步";
    const char *instru_3 = "单击S确认 长按S退出";
    uint8_t str_len = 0;
    str_len = u8g2.getUTF8Width(instru_title);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(instru_title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2-2),16,str_len+4);
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(instru_1);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2 - 2);
    u8g2.print(instru_1);
    str_len = u8g2.getUTF8Width(instru_2);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 3 - 2);
    u8g2.print(instru_2);
    str_len = u8g2.getUTF8Width(instru_3);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 4 - 2);
    u8g2.print(instru_3);
    u8g2.sendBuffer();
}
/*
函 数 名:void about_enter(void)
功能说明:进入关于界面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void about_enter(void)
{
    const char *about_title = "关于";
    const char *about_1 = "LambdaTV";
    uint8_t str_len = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(about_title);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(about_title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*2+4);
    u8g2.setFont(u8g2_font_ncenB14_tr);
    str_len = u8g2.getUTF8Width(about_1);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2);
    u8g2.print(about_1);
    u8g2.setFont(u8g2_font_unifont_t_shopl16);
    u8g2.setCursor(0, 16 * 3);
    u8g2.print("by RAiny");
    u8g2.sendBuffer();
}
/*
函 数 名:void clock_mode_enter(void)
功能说明:进入时钟模式界面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clock_mode_enter(void)
{
    uint8_t mode = 0;
    uint8_t box_y = 0;
    const char *mode_title = "时钟模式";
    uint8_t str_len = 0;
    mode = eeprom.data.clock_mode;
    while (1)
    {
        while (box_y < 27)
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1); /* activate transparent font mode */
            u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
            str_len = u8g2.getUTF8Width(mode_title);
            u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
            u8g2.setDrawColor(2);
            u8g2.print(mode_title);
            u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
            // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*4+4);
            u8g2.setFont(u8g2_font_ncenB24_tr);
            str_len = u8g2.getUTF8Width("1");
            u8g2.setDrawColor(1); //白 /* color 1 for the box */
            u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 4, 16 + 12 - 1, box_y++, box_y++);
            u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 + 12 + 24);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE) //一开始EPPROM不为零
            {
                mode = eeprom.data.clock_mode = 0;
            }
            u8g2.setDrawColor(0); //黑
            u8g2.print(mode);
            u8g2.sendBuffer();
            delay(10);
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            box_y = 0;
            mode += 1;
            if (mode > CLOCK_MAX_MODE)
                mode = 0;
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            box_y = 0;
            mode -= 1;
            if (mode <= 0)
                mode = CLOCK_MAX_MODE;
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            eeprom.data.clock_mode = mode;
            eeprom_write();
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}
/*
函 数 名:void select_cancel_anima(uint8_t x1)
功能说明:清除WiFi 选择框移动动画
形    参:void
返 回 值:void
时    间：2020-1-5
RAiny
*/
void select_cancel_anima(const char *title, const char *content, uint8_t x1)
{
    uint8_t str_len = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(title);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*4+4);
    str_len = u8g2.getUTF8Width(content);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2 - 2 + 5);
    u8g2.print(content);
    u8g2.setCursor(24, 16 * 4 - 7);
    u8g2.print("取消");
    u8g2.setCursor(24 + 28 + 28, 16 * 4 - 7);
    u8g2.print("确认");
    u8g2.drawFrame(0, 16 + 2, 128, 64 - 18);   //大框框
    u8g2.drawBox(x1, 16 * 4 - 7 - 16, 34, 20); //确认框
    // u8g2.drawFrame(x2,16*4-7-16,34,20);
    u8g2.sendBuffer();
}
/*
函 数 名:void clear_wifi_enter(void)
功能说明:清除WiFi
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clear_wifi_enter(void)
{
    int8_t select = 0; //0 是取消，1是确认
    uint8_t show_flag = 0, frame_x = 0;
    const uint8_t x1 = 24 - 4, x2 = 24 + 28 + 28 - 4;
    const char *wifi_title = "清除WiFi";
    const char *wifi_1 = "清除所有WiFi信息";
    while (1)
    {
        if (show_flag == 0) //初次进入
        {
            select_cancel_anima(wifi_title, wifi_1, x1);
        }
        else if (show_flag == 1)
        {
            if (select == 1)
            {
                while (frame_x < x2)
                {
                    select_cancel_anima(wifi_title, wifi_1, frame_x++);
                }
            }
            else if (select == 0)
            {
                while (frame_x > x1)
                {
                    select_cancel_anima(wifi_title, wifi_1, frame_x--);
                }
            }
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            select += 1;
            show_flag = 1; //按下按键
            if (select > 1)
            {
                select = 0;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            select -= 1;
            show_flag = 1;
            if (select < 0)
            {
                select = 1;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            if (select == 1)
            {
                //清除ESP8266所存储的WiFi连接信息
                wifiManager.resetSettings();
                ESP.restart();
            }
            else if (select == 0)
            {
                set_keymenu_event(KEY_CANCEL);
                break;
            }
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}
/*
函 数 名:void close_open_rgb(void)
功能说明:设置RGB_LED
形    参:void
返 回 值:void
时    间：2020-1-8
RAiny
*/
void close_open_rgb(void)
{
    config_state = 16;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void close_open_rgb_enter(void)
功能说明:进入打开关闭RGB设置页面
形    参:void
返 回 值:void
时    间：2020-1-8
RAiny
*/
void close_open_rgb_enter(void)
{
    int8_t select = 0; //0 是取消，1是确认
    uint8_t show_flag = 0, frame_x = 0;
    const uint8_t x1 = 24 - 4, x2 = 24 + 28 + 28 - 4;
    const char *rgb_title = "设置RGB_LED";
    const char *rgb_content = "开启RGB_LED";
    while (1)
    {
        if (show_flag == 0) //初次进入
        {
            select_cancel_anima(rgb_title, rgb_content, x1);
        }
        else if (show_flag == 1)
        {
            if (select == 1)
            {
                while (frame_x < x2)
                {
                    select_cancel_anima(rgb_title, rgb_content, frame_x++);
                }
            }
            else if (select == 0)
            {
                while (frame_x > x1)
                {
                    select_cancel_anima(rgb_title, rgb_content, frame_x--);
                }
            }
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            select += 1;
            show_flag = 1; //按下按键
            if (select > 1)
            {
                select = 0;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            select -= 1;
            show_flag = 1;
            if (select < 0)
            {
                select = 1;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            if (select == 1)
            {
                eeprom.data.led_on = 0;
                eeprom_write();
                set_keymenu_event(KEY_CANCEL);
                break;
            }
            else if (select == 0)
            {
                eeprom.data.led_on = 1;
                eeprom_write();
                set_keymenu_event(KEY_CANCEL);
                break;
            }
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}
#line 1 "e:\\ESP\\esp8266_oled\\LambdaTV\\LambdaTV_server.ino"
#include "LambdaTV.h"
/*
太极创客教程
*/
/*
函 数 名:void handleUserRequet()
功能说明:处理用户浏览器的HTTP访问
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
void handleUserRequet()
{
    // 获取用户请求网址信息
    String webAddress = esp8266_server.uri();
    // 通过handleFileRead函数处处理用户访问
    bool fileReadOK = handleFileRead(webAddress);
    // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
    if (!fileReadOK)
    {
        esp8266_server.send(404, "text/plain", "404 Not Found");
    }
}
/*
函 数 名:bool handleFileRead(String path) 
功能说明:处理浏览器HTTP访问
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
bool handleFileRead(String path)
{
    if (path.endsWith("/"))
    {                         // 如果访问地址以"/"为结尾
        path = "/index.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
    }
    String contentType = getContentType(path); // 获取文件类型
    if (SPIFFS.exists(path))
    {                                                 // 如果访问的文件可以在SPIFFS中找到
        File file = SPIFFS.open(path, "r");           // 则尝试打开该文件
        esp8266_server.streamFile(file, contentType); // 并且将该文件返回给浏览器
        file.close();                                 // 并且关闭文件
        return true;                                  // 返回true
    }
    return false; // 如果文件未找到，则返回false
}
/*
函 数 名:String getContentType(String filename)
功能说明:获取文件类型
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
String getContentType(String filename)
{
    if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define M5STICK

static const uint8_t TX = 43;
static const uint8_t RX = 44;

static const uint8_t SDA = 48;
static const uint8_t SCL = 47;

static const uint8_t SS = 41;
static const uint8_t MOSI = 39;
static const uint8_t MISO = 255;
static const uint8_t SCK = 40;

static const uint8_t G9 = 9;
static const uint8_t G10 = 10;
static const uint8_t G11 = 11;
static const uint8_t G12 = 12;
static const uint8_t G14 = 14;
static const uint8_t G15 = 15;
static const uint8_t G16 = 16;
static const uint8_t G17 = 17;
static const uint8_t G18 = 18;
static const uint8_t G21 = 21;
static const uint8_t G39 = 39;
static const uint8_t G40 = 40;
static const uint8_t G41 = 41;
static const uint8_t G45 = 45;
static const uint8_t G47 = 47;
static const uint8_t G48 = 48;

#define FP 1
#define FM 2
#define FG 3

#define SERIAL_TX 10
#define SERIAL_RX 9
#define GPS_SERIAL_TX 10
#define GPS_SERIAL_RX 9

#define GROVE_SDA 10
#define GROVE_SCL 9

#define SPI_SCK_PIN 40
#define SPI_MOSI_PIN 39
#define SPI_MISO_PIN -1
#define SPI_SS_PIN 41

#define LED 45
#define LED_ON HIGH
#define LED_OFF LOW

#define SMOOTH_FONT 1

#define DEEPSLEEP_WAKEUP_PIN 11
#define DEEPSLEEP_PIN_ACT LOW

#endif /* Pins_Arduino_h */

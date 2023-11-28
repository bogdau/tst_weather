#ifndef __OLED_H__
#define __OLED_H__

#include <dispay_sh1106/fontx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <wiringShift.h>

#define I2C_ADDRESS 0x3C

typedef struct
{
  uint8_t ank;
  uint8_t utf;
  uint8_t colum;
  uint8_t reverse;
  uint8_t enhance;
  uint8_t size;
  uint8_t ascii[16];
  uint16_t sjis[16];
} SaveData;

typedef struct
{
  SaveData save[4];
} SaveFrame;

class oled
{
private:
  FontxFile fx[2];
    fonxt f;
  static unsigned char init_command[];

  unsigned char frame[1024];
  int i2cd;
  int i = 0;
  char cpath[128] = "oled.conf";
  FILE *fp;
  SaveFrame sv;

  int offset = 0;
  int page = 8;
  int spos;
  char numc[5];
  int num;
  void init_i2c(uint8_t i2caddr);
  int drawSJISChar(FontxFile *fx, int x, int y, uint16_t sjis, uint8_t reverse, uint8_t enhance);
  void show_i2c(int page, int offset);
  void DumpSaveFrame(SaveFrame hoge);

public:
  void clear_display();
  void show_display();
  void init(double temp, double press);
  void update_display_temperature(double temperature);
  void update_display_pressure(double pressure);
  oled();
};

#endif /* __OLED_H__ */

#include "dispay_sh1106/oled.h"
#include <iostream>
#include <cstdio>

oled::oled()
{
}

void oled::display_info(double temp, double press)
{
  struct stat stat_buf;
  if (stat(cpath, &stat_buf) == 0)
  {
    fp = fopen(cpath, "rb");
    fread(&sv, sizeof(sv), 1, fp);
    fclose(fp);
  }
  else
  {
    memset(&sv, 0, sizeof(sv));
  }

  f.Fontx_init(fx, "./fontx/ILGH16XB.FNT", "./fontx/ILGZ16XB.FNT");
  int num = 0;

  memset(&sv, 0, sizeof(sv));

  fp = fopen(cpath, "wb");
  fwrite(&sv, sizeof(sv), 1, fp);
  fclose(fp);

  char elements[20];
  sprintf(elements, " Temp: %.2f C", temp);
  char elements2[20];
  sprintf(elements2, " Press: %d Pa", (int)press);

  num = 1;
  sv.save[num].size = f.String2SJIS((unsigned char *)elements, strlen(elements), sv.save[num].sjis, 16);
  sv.save[num].ank = 0;
  sv.save[num].utf = 1;
  fp = fopen(cpath, "wb");
  fwrite(&sv, sizeof(sv), 1, fp);

  num = 2;
  sv.save[num].size = f.String2SJIS((unsigned char *)elements2, strlen(elements2), sv.save[num].sjis, 16);
  sv.save[num].ank = 0;
  sv.save[num].utf = 1;

  fwrite(&sv, sizeof(sv), 1, fp);
  fclose(fp);

  init_i2c(I2C_ADDRESS);
  int y;
  for (num = 0; num < 4; num++)
  {
    if (sv.save[num].size == 0)
      continue;
    y = sv.save[num].colum + 1;
    for (i = 0; i < sv.save[num].size; i++)
    {
      if (sv.save[num].utf)
        y = drawSJISChar(fx, num + 1, y, sv.save[num].sjis[i], sv.save[num].reverse,
                         sv.save[num].enhance);
    }
  }

  show_i2c(page, offset);
}

unsigned char oled::init_command[] = {
    0xAE, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xA1, 0xC8,
    0xD5, 0x80, 0xDA, 0x12, 0x81, 0xFF,
    0xA4, 0xDB, 0x40, 0x20, 0x02, 0x00, 0x10, 0x8D,
    0x14, 0x2E, 0xA6, 0xAF};

void oled::init_i2c(uint8_t i2caddr)
{
  int byte;

  for (byte = 0; byte < 1024; byte++)
  {
    frame[byte] = 0x00;
  }

  i2cd = wiringPiI2CSetup(i2caddr);
  int i;
  for (i = 0; i < sizeof(init_command); i++)
  {
    unsigned int control = 0x00; // Co = 0, D/C = 0
    wiringPiI2CWriteReg8(i2cd, control, init_command[i]);
  }
}

int oled::drawSJISChar(FontxFile *fx, int x, int y, uint16_t sjis, uint8_t reverse, uint8_t enhance)
{
  unsigned char fonts[32];
  uint8_t bitmap[128];
  unsigned char pw, ph;
  bool rc;

  rc = f.GetFontx(fx, sjis, fonts, &pw, &ph);
  if (rc)
  {
    f.Font2Bitmap(fonts, bitmap, pw, ph, 1);
    if (enhance)
      f.UnderlineBitmap(bitmap, pw, ph);
    if (reverse)
      f.ReversBitmap(bitmap, pw, ph);
    memcpy(frame + ((x - 1) * 256) + ((y - 1) * 8), bitmap, pw);
    memcpy(frame + (128 + (x - 1) * 256) + ((y - 1) * 8), bitmap + 32, pw);
  }
  return y + (pw / 8);
}

void oled::show_i2c(int page, int offset)
{
  unsigned char page_command[3];

  for (int _page = 0; _page < page; _page++)
  {
    page_command[0] = 0x00 + offset;
    page_command[1] = 0x10;
    page_command[2] = 0xB0 + _page;

    for (int i = 0; i < sizeof(page_command); i++)
    {
      wiringPiI2CWriteReg8(i2cd, 0x00, page_command[i]);
    }

    for (int col = 0; col < 128; col++)
    {
      wiringPiI2CWriteReg8(i2cd, 0x40, frame[_page * 128 + col]);
    }
  }
}

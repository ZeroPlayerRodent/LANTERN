
#include <stdlib.h>
#include <string.h>
#include <cv.h>
#include <cvu.h>

#include "common.h"
//#link "common.c"
//#link "fonts.s"

const byte char_table[10][8] = {
  /*{w:8,h:8,brev:1,count:10}*/ 
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x7E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E},
  {0x00,0x00,0x44,0x55,0xFF,0xAA,0x08,0x00},
  {0x30,0x1C,0x30,0x18,0x70,0x18,0x30,0x1C},
  {0xC3,0xE7,0x7E,0x3C,0x3C,0x7E,0xE7,0xC3},
  {0x00,0x00,0x00,0x00,0x00,0x02,0x22,0x2A},
  {0xFF,0x99,0x81,0xC3,0xC3,0x81,0x99,0xFF},
  {0x08,0x0C,0xFE,0xFF,0xFF,0xFE,0x0C,0x08},
  {0x08,0x28,0x3A,0x3E,0x66,0x62,0x76,0x3C},
  {0x80,0x40,0x40,0x38,0x3E,0x3D,0x3E,0x3C},
};

const byte sprite_table[5][16*2] = {
  /*{w:8,h:16,brev:32,count:5}*/
  {
0x3C,0x6A,0x7E,0x3C,0x10,0x7C,0xD6,0x93,
0x99,0xAD,0x24,0x64,0x44,0x42,0xC2,0xC2,
  },
  {
0x3C,0x6A,0x7E,0x3C,0x10,0x7C,0x54,0x54,
0x52,0x52,0x5A,0x28,0x28,0x28,0x68,0x6C,
  },
  {
0x6A,0xEE,0xFF,0xDF,0xFB,0xFE,0x78,0x5C,
0x54,0x54,0x54,0x44,0x44,0x1C,0x16,0x32,
  },
  {
0x6A,0xEE,0xFF,0xFB,0xDF,0xFE,0x78,0x5C,
0x54,0x54,0x44,0x44,0x4C,0x4C,0x1C,0x12,
  },
  {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  },
};

bool over = false;
bool died = false;

byte score[6];

void draw_score(byte x, byte y) {
  byte i;
  for (i=0; i<6; i++) {
    putcharxy(x+i, y, (48+score[i]));
  }
}

void add_point() {
  score[5]++;
  if (score[5] > 9) {
    score[5] = 0;
  } else {
    return;
  }
  score[4]++;
  if (score[4] > 9) {
    score[4] = 0;
  } else {
    return;
  }
  score[3]++;
  if (score[3] > 9) {
    score[3] = 0;
  } else {
    return;
  }
  score[2]++;
  if (score[2] > 9) {
    score[2] = 0;
  } else {
    return;
  }
  score[1]++;
  if (score[1] > 9) {
    score[1] = 0;
  } else {
    return;
  }
  score[0]++;
}

void add_score(byte amount) {
  byte i;
  for (i=0; i<amount; i++) {
    add_point();
  }
}

void reset_score() {
  byte i;
  for (i=0; i<6; i++) {
    score[i] = 0;
  }
}

byte boing = 0;
byte crush = 0;

struct cvu_sprite thesprites[8];

static word bass[] = 
{
 261, 0, 0, 0, 261, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 261, 0, 0, 0, 261, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 261, 0, 0, 0, 261, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
 261, 0, 0, 0, 261, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,
};

byte note = 0;
byte notime = 0;

byte note2 = 0;
byte notime2 = 0;

const byte count = 16*4;

void play_note() {
  cv_set_attenuation(CV_SOUNDCHANNEL_2, 32);
  
  if (boing > 0) {
    boing--;
    cv_set_attenuation(CV_SOUNDCHANNEL_2, 10);
    cv_set_frequency(CV_SOUNDCHANNEL_2, 15000);
  }
  
  if (crush > 0) {
    crush--;
    cv_set_attenuation(CV_SOUNDCHANNEL_2, 10);
    cv_set_frequency(CV_SOUNDCHANNEL_2, 255*100);
  }
  
  cv_set_attenuation(CV_SOUNDCHANNEL_0, 32);
  cv_set_frequency(CV_SOUNDCHANNEL_0, bass[note]*100);
  if (bass[note] != 0){
    cv_set_attenuation(CV_SOUNDCHANNEL_0, 18);
  }
  
  cv_set_attenuation(CV_SOUNDCHANNEL_1, 32);
  
  if (notime2 > 0) {
    notime2--;
  }
  if (notime2 == 0) {
    note2++;
    if (note2 >= count) {
      note2 = 0;
    }
    notime2 = 2*2;
  }
  
  if (notime > 0) {
    notime--;
  }
  if (notime == 0) {
    note++;
    if (note >= count) {
      note = 0;
    }
    notime = 3*2;
  }
}

void draw_sprites() {
  cvu_memtovmemcpy(SPRITES, thesprites, sizeof(thesprites));
}

void wipe_screen() {
  byte x = 0;
  byte y = 0;
  byte i;
  while (y < 24) {
    while (x < 32) {
      putcharxy(x, y, ' ');
      x++;
    }
    x = 0;
    y += 1;
  }
  for (i=0; i<8; i++) {
    thesprites[i].y = 200;
    draw_sprites();
  }
}

void draw_stars() {
  byte x = 0;
  byte y = 3;
  while (y < 24) {
    while (x < 32) {
      if (rand()%255 > 230) {
      	putcharxy(x, y, 5);
      } else {
        putcharxy(x, y, 0);
      }
      x++;
    }
    x = 0;
    y += 1;
  }
}

word frames = 0;

void wait_for_frame() {
  while (((vint_counter ^ frames) & 3) == 0);
}

bool flickering = false;
bool flicker = false;
byte lines = 0;

void calc_lines() {
  byte i;
  byte a;
  lines = 0;
  for (i = 0; i < 8; i++) {
    for (a = 0; a < 8; a++) {
      if (thesprites[i].y >= thesprites[a].y && thesprites[i].y <= (thesprites[a].y + 16)) {
        if (a != i) {
          lines++;
        }
      }
    }
  }
  if (lines >= 10) {
    flickering = true;
  } else {
    flickering = false;
  }
}

void setup_32_column_font() {
  copy_default_character_set();
  cv_set_colors(0, CV_COLOR_BLACK);
  cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_GRAY), 8);
  cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_GRAY), 32-8);
}

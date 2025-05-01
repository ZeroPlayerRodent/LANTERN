
#include <stdlib.h>
#include <string.h>
#include <cv.h>
#include <cvu.h>
#include "globals.h"
#include "world.h"
#include "walls.h"
#include "enemy.h"
#include "players.h"
#include "levels.h"
#include "common.h"
//#link "common.c"

byte time_bonus;

void message(int level) {
  bool bounce = true;
  struct cv_controller_state cs;
  cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_GRAY), 8);
  cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_GRAY), 32-8);
  copy_default_character_set();
  if (level == 0) {
    putstringxy(0, 2, "You wake up on a farm");
    putstringxy(0, 4, "with nothing but a lantern.");
    putstringxy(0, 6, "Use JOYSTICK to walk.");
    putstringxy(0, 8, "Press FIRE to use lantern.");
    putstringxy(0, 10, "Reach the arrow to escape.");
  }
  if (level == 1) {
    putstringxy(0, 2, "Old barbed wire fences");
    putstringxy(0, 4, "stand in the freezing night.");
    putstringxy(0, 6, "They are rusty and");
    putstringxy(0, 8, "covered in dry blood.");
  }
  if (level == 2) {
    putstringxy(0, 2, "A large wooden gate blocks");
    putstringxy(0, 4, "your exit path. Maybe there");
    putstringxy(0, 6, "is a key or switch to open it?");
  }
  if (level == 3) {
    putstringxy(0, 2, "The foul stench of rotting");
    putstringxy(0, 4, "flesh dampens what little");
    putstringxy(0, 6, "optimism you had left.");
  }
  if (level == 4) {
    putstringxy(0, 2, "The gnashing of teeth grows");
    putstringxy(0, 4, "louder as the night");
    putstringxy(0, 6, "grows darker.");
  }
  if (level == 5) {
    putstringxy(0, 2, "They are hungry.");
  }
  if (level == 6) {
    putstringxy(0, 2, "Your sanity is slipping away.");
    putstringxy(0, 4, "Is this the end?");
  }
  if (level == 7) {
    putstringxy(0, 2, "You see a locked shed. Maybe");
    putstringxy(0, 4, "there's something useful inside?");
  }
  if (level == 8) {
    putstringxy(0, 2, "Your boots are slick and wet");
    putstringxy(0, 4, "from trudging through");
    putstringxy(0, 6, "bloody corpses.");
  }
  if (level == 9) {
    putstringxy(0, 2, "There is still no sign of");
    putstringxy(0, 4, "human life anywhere. Your");
    putstringxy(0, 6, "lantern is all you have.");
  }
  if (level == 10) {
    putstringxy(0, 2, "The sting of fear trickles");
    putstringxy(0, 4, "down your spine.");
  }
  if (level == 11) {
    putstringxy(0, 2, "You see a sliver of light");
    putstringxy(0, 4, "ahead. Is it finally over?");
  }
  if (level == 12) {
    putstringxy(0, 2, "You escaped the nightmare.");
    putstringxy(0, 4, "Continue playing to truly test");
    putstringxy(0, 6, "your survival skills.");
    current_level = 0;
  }
  if (level >= 100) {
    putstringxy(0, 2, "Your body grows cold as");
    putstringxy(0, 4, "you bleed out in the grass.");
    putstringxy(0, 6, "GAME OVER");
  }
  putstringxy(0, 18, "Time Bonus:");
  draw_score(2, 20);
  while (1) {
    cv_get_controller_state(&cs, 0);
    if (cs.joystick & CV_FIRE_0) {
      if (!bounce) {
        break;
      }
      bounce = true;
    } else {
      bounce = false;
    };
  }
}

bool two_player = false;

void menu() {
  struct cv_controller_state cs;
  bool bounce = false;
  cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_GRAY), 8);
  cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_GRAY), 32-8);
  copy_default_character_set();
  putstringxy(12, 5, "LANTERN");
  putstringxy(12, 7, "1P GAME");
  putstringxy(12, 9, "2P GAME");
  while (1) {
    rand();
    cv_get_controller_state(&cs, 0);
    if (cs.joystick & CV_DOWN || cs.joystick & CV_UP) {
      if (!bounce) {
        two_player = !two_player;
      }
      bounce = true;
    } else {
      bounce = false;
    }
    if (cs.joystick & CV_FIRE_0) break;
    if (two_player) {
      putcharxy(11, 9, '>');
      putcharxy(11, 7, ' ');
    } else {
      putcharxy(11, 9, ' ');
      putcharxy(11, 7, '>');
    }
  }
  light = false;
  current_level = 0;
  bat_time = 60;
  battery = 80;
  reset_score();
  init_player();
  died = false;
  over = false;
  if (!two_player) players[1].dead = true;
}

void main() {
  byte sn_intro = 200;
  vdp_setup();
  setup_32_column_font();
  cv_set_vint_handler(&vint_handler);
  cvu_memtovmemcpy(SPRITE_PATTERNS, sprite_table, sizeof(sprite_table));
  cv_set_screen_active(true);
  wipe_screen();
  died = true;
  cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_BLUE), 8);
  cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_BLUE), 32-8);
  putstringxy(4, 10, "Good Deal Games Presents");
  while (sn_intro > 0) {
    frames++;
    wait_for_frame();
    sn_intro--;
    if (sn_intro == 199) {
      cv_set_attenuation(CV_SOUNDCHANNEL_2, 10);
      cv_set_frequency(CV_SOUNDCHANNEL_2, 255*100);
    }
    if (sn_intro == 200-30) {
      cv_set_attenuation(CV_SOUNDCHANNEL_2, 10);
      cv_set_frequency(CV_SOUNDCHANNEL_2, 155*100);
    }
    if (sn_intro == 200-60) {
      cv_set_attenuation(CV_SOUNDCHANNEL_2, 10);
      cv_set_frequency(CV_SOUNDCHANNEL_2, 100*100);
    }
    if (sn_intro == 200-90) {
      cv_set_attenuation(CV_SOUNDCHANNEL_2, 32);
    }
  }
  wipe_screen();
  
  init_player();
  
  while (1) {
    wipe_screen();
    if (died) {
      menu();
      wipe_screen();
    }
    message(current_level);
    wipe_screen();
    cvu_memtovmemcpy(PATTERN, char_table, sizeof(char_table));
    cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_BLACK), 8);
    cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_BLACK), 32-8);
    draw_stars();
    gen_level();
    draw_wall();
    render_battery();
    
    time_bonus = 255;
    
    while(!over) {
      frames++;
      move_player();
      draw_player();

      move_enemies();
      draw_enemies();
      
      play_note();

      calc_lines();
      
      if (frames%5==0 && time_bonus) time_bonus--;

      if (flickering) {
        flicker = !flicker;
        if (flicker) {
          thesprites[0].y = 200;
          thesprites[1].y = 200;
          thesprites[2].y = 200;
          thesprites[3].y = 200;
        } else {
          thesprites[4].y = 200;
          thesprites[5].y = 200;
          thesprites[6].y = 200;
          thesprites[7].y = 200;
        }
      }

      render_world();
      
      if (players[0].dead && players[1].dead) {
        died = true;
        over = true;
      }

      wait_for_frame();
      draw_sprites();
    }
    cv_set_attenuation(CV_SOUNDCHANNEL_2, 32);
    cv_set_attenuation(CV_SOUNDCHANNEL_1, 32);
    cv_set_attenuation(CV_SOUNDCHANNEL_0, 32);
    if (died) {
      wipe_screen();
      message(100);
    } else {
      current_level++;
      add_score(time_bonus);
      over = false;
      light = false;
    }
  }
}

struct player {
  byte px;
  byte py;
  byte lastx;
  byte lasty;
  byte steptime;
  byte hp;
  byte invuln;
  bool step;
  bool light;
  bool bounce;
  bool dead;
};

struct player players[2];

bool light;

byte bat_time = 60;
byte battery = 60;

void render_battery() {
  putcharxy(0, 0, 8);
  if (battery > 10) {
    putcharxy(1, 0, 8);
  } else {
    putcharxy(1, 0, 0);
  }
  if (battery > 20) {
    putcharxy(2, 0, 8);
  } else {
    putcharxy(2, 0, 0);
  }
  if (battery > 30) {
    putcharxy(3, 0, 8);
  } else {
    putcharxy(3, 0, 0);
  }
  if (battery > 40) {
    putcharxy(4, 0, 8);
  } else {
    putcharxy(4, 0, 0);
  }
  if (battery > 50) {
    putcharxy(5, 0, 8);
  } else {
    putcharxy(5, 0, 0);
  }
}

void render_world() {
  if (!battery) {
    light = false;
  }
  
  if (light) {
    cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_GRAY), 8);
    cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_GRAY), 32-8);
    if (bat_time) {
      bat_time--;
    } else {
      if (battery) {
        battery--;
        render_battery();
      }
      bat_time = 60;
    }
  } else {
    cvu_vmemset(COLOR, COLOR_FG(CV_COLOR_BLACK), 8);
    cvu_vmemset(COLOR+8, COLOR_FG(CV_COLOR_BLACK), 32-8);
  }
}

struct wall {
  byte x;
  byte y;
  byte length;
  bool vertical;
  byte type;
};

/*

TYPES

0 = Brick
1 = Barbed
2 = Door
3 = Key
4 = Exit
5 = Clear
6 = Gas
7 = Adrenaline

*/

struct wall walls[8];
const byte amount = 8;

void reset_walls() {
  byte i;
  for (i=0; i<amount; i++) {
    walls[i].x = 0;
    walls[i].y = 0;
    walls[i].length = 0;
    walls[i].vertical = false;
    walls[i].type = 0;
  }
}

void draw_wall() {
  byte i;
  byte a;
  for (a=0; a<amount; a++) {
    if (walls[a].vertical) {
      for (i=0; i<walls[a].length; i++) {
        if (walls[a].type == 1) {
          putcharxy(walls[a].x, walls[a].y+i, 3);
        } else {
          putcharxy(walls[a].x, walls[a].y+i, 1);
        }
        if (walls[a].type == 2) {
          putcharxy(walls[a].x, walls[a].y+i, 4);
        }
        if (walls[a].type == 3) {
          putcharxy(walls[a].x, walls[a].y+i, 6);
        }
        if (walls[a].type == 4) {
          putcharxy(walls[a].x, walls[a].y+i, 7);
        }
        if (walls[a].type == 5) {
          putcharxy(walls[a].x, walls[a].y+i, 0);
        }
        if (walls[a].type == 6) {
          putcharxy(walls[a].x, walls[a].y+i, 9);
        }
      }
    } else {
      for (i=0; i<walls[a].length; i++) {
        if (walls[a].type == 1) {
          putcharxy(walls[a].x+i, walls[a].y, 2);
        } else {
          putcharxy(walls[a].x+i, walls[a].y, 1);
        }
        if (walls[a].type == 2) {
          putcharxy(walls[a].x+i, walls[a].y, 4);
        }
        if (walls[a].type == 3) {
          putcharxy(walls[a].x+i, walls[a].y, 6);
        }
        if (walls[a].type == 4) {
          putcharxy(walls[a].x+i, walls[a].y, 7);
        }
        if (walls[a].type == 5) {
          putcharxy(walls[a].x+i, walls[a].y, 0);
        }
        if (walls[a].type == 6) {
          putcharxy(walls[a].x, walls[a].y+i, 9);
        }
      }
    }
  }
}

byte wall_collision(byte x, byte y) {
  byte collision_type = 0;
  byte i;
  for (i=0; i<amount; i++) {
    if (walls[i].type != 5) {
      if (walls[i].vertical) {
        if (x >= walls[i].x*8 && x <= (walls[i].x*8)+8) {
          if (y >= walls[i].y*8 && y <= (walls[i].y*8)+(walls[i].length*8)) {
            collision_type = 1;
            if (walls[i].type == 1) {
              collision_type = 2;
            }
            if (walls[i].type == 3) {
              collision_type = 3;
            }
            if (walls[i].type == 4) {
              collision_type = 4;
            }
            if (walls[i].type == 6) {
              collision_type = 6;
            }
          }
        }
      } else {
        if (x >= walls[i].x*8 && x <= (walls[i].x*8)+(walls[i].length*8)) {
          if (y >= walls[i].y*8 && y <= (walls[i].y*8)+8) {
            collision_type = 1;
            if (walls[i].type == 1) {
              collision_type = 2;
            }
            if (walls[i].type == 3) {
              collision_type = 3;
            }
            if (walls[i].type == 4) {
              collision_type = 4;
            }
            if (walls[i].type == 6) {
              collision_type = 6;
            }
          }
        }
      }
    }
  }
  return collision_type;
}

void open_doors() {
  byte i;
  for (i=0; i<amount; i++) {
    if (walls[i].type == 2 || walls[i].type == 3) {
      walls[i].type = 5;
    }
  }
  draw_wall();
}

void collect_gas() {
  byte i;
  for (i=0; i<amount; i++) {
    if (walls[i].type == 6) {
      walls[i].type = 5;
    }
  }
  battery += 40;
  if (battery > 60) battery = 60;
  render_battery();
  draw_wall();
}

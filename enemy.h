struct enemy {
  byte x;
  byte y;
  bool active;
  byte lastx;
  byte lasty;
  byte dir;
  byte time;
  bool step;
};

struct enemy enemies[6];

void init_enemies() {
  byte i;
  for (i=0; i<6; i++) {
    enemies[i].lastx = enemies[i].x;
    enemies[i].lasty = enemies[i].y;
    enemies[i].dir = rand()%4;
    enemies[i].time = (rand()%10)+10;
    enemies[i].step = false;
  }
}

void move_enemies() {
  byte i;
  for (i=0; i<6; i++) {
    if (enemies[i].time > 0) {
      if (enemies[i].active) {
        enemies[i].time--;
      }
    } else {
      if (rand()%255 > 230) {
        enemies[i].dir = rand()%4;
      }
      
      if (!players[1].dead) {
        if (enemies[i].y >= players[1].py && enemies[i].y <= (players[1].py + 16)) {
          if (players[1].px > enemies[i].x) {
            enemies[i].dir = 3;
          } else {
            enemies[i].dir = 2;
          }
        }
        if (enemies[i].x >= players[1].px && enemies[i].x <= (players[1].px + 8)) {
          if (players[1].py > enemies[i].y) {
            enemies[i].dir = 1;
          } else {
            enemies[i].dir = 0;
          }
        }
      }
      
      if (!players[0].dead) {
        if (enemies[i].y >= players[0].py && enemies[i].y <= (players[0].py + 16)) {
          if (players[0].px > enemies[i].x) {
            enemies[i].dir = 3;
          } else {
            enemies[i].dir = 2;
          }
        }
        if (enemies[i].x >= players[0].px && enemies[i].x <= (players[0].px + 8)) {
          if (players[0].py > enemies[i].y) {
            enemies[i].dir = 1;
          } else {
            enemies[i].dir = 0;
          }
        }
      }
      
      if (enemies[i].dir == 0) {
        enemies[i].lasty = enemies[i].y;
        enemies[i].y -= 4;
      }
      if (enemies[i].dir == 1) {
        enemies[i].lasty = enemies[i].y;
        enemies[i].y += 4;
      }
      if (enemies[i].dir == 2) {
        enemies[i].lastx = enemies[i].x;
        enemies[i].x -= 4;
      }
      if (enemies[i].dir == 3) {
        enemies[i].lastx = enemies[i].x;
        enemies[i].x += 4;
      }
      
      if (wall_collision(enemies[i].x+8, enemies[i].y+12)) {
        enemies[i].x = enemies[i].lastx;
        enemies[i].y = enemies[i].lasty;
        enemies[i].dir = rand()%4;
      }
      
      if (enemies[i].x > 240) {
        enemies[i].x = 240;
        enemies[i].dir = 2;
      }
      if (enemies[i].x < 15) {
        enemies[i].x = 15;
        enemies[i].dir = 3;
      }
      if (enemies[i].y > 175) {
        enemies[i].y = 175;
        enemies[i].dir = 0;
      }
      if (enemies[i].y < 5) {
        enemies[i].y = 5;
        enemies[i].dir = 1;
      }
      
      enemies[i].step = !enemies[i].step;
      
      enemies[i].time = (rand()%10)+5;
    }
  }
}

bool enemy_collision(byte x, byte y) {
  bool collided = false;
  byte i;
  for (i=0; i<6; i++) {
    if (x >= enemies[i].x && x <= enemies[i].x+8) {
      if (y >= enemies[i].y+4 && y <= enemies[i].y+16) {
        collided = true;
      }
    }
  }
  return collided;
}

void draw_enemies() {
  byte i;
  for (i=0; i<6; i++) {
    if (enemies[i].active) {
      if (light && battery) {
        thesprites[i+2].tag = CV_COLOR_RED;
      } else {
        thesprites[i+2].tag = CV_COLOR_BLACK;
      }
      if (enemies[i].step) {
        thesprites[i+2].name = 4*2;
      } else {
        thesprites[i+2].name = 4*3;
      }
      thesprites[i+2].x = enemies[i].x;
      thesprites[i+2].y = enemies[i].y;
    } else {
      thesprites[i+2].name = 4*4;
      thesprites[i+2].y = 175+(i*25);
    }
  }
}

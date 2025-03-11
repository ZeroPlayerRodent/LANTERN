void init_player() {
  byte i;
  for (i=0; i<2; i++) {
    players[i].px = 10;
    players[i].py = 100;
    
    players[i].lastx = 10;
    players[i].lasty = 100;
    
    players[i].steptime = 10;
    players[i].step = false;
    
    players[i].hp = 3;
    players[i].invuln = 0;
    
    players[i].light = false;
    players[i].bounce = false;
    
    players[i].dead = false;
  }
}

void move_player() {
  struct cv_controller_state cs;
  byte i;
  byte wallhit;
  for (i=0; i<2; i++) {
    
    if (!players[i].dead) {
      cv_get_controller_state(&cs, i);
      
      if (frames%2 == 0) {
        if (cs.joystick & CV_UP) {
          players[i].lasty = players[i].py;
          players[i].py -= 1;
          if (players[i].steptime > 0) {
            players[i].steptime--;
          }
        } else if (cs.joystick & CV_DOWN) {
          players[i].lasty = players[i].py;
          players[i].py += 1;
          if (players[i].steptime > 0) {
            players[i].steptime--;
          }
        }

        if (cs.joystick & CV_LEFT) {
          players[i].lastx = players[i].px;
          players[i].px -= 1;
          if (players[i].steptime > 0) {
            players[i].steptime--;
          }
        } else if (cs.joystick & CV_RIGHT) {
          players[i].lastx = players[i].px;
          players[i].px += 1;
          if (players[i].steptime > 0) {
            players[i].steptime--;
          }
        }
      }

      if (cs.joystick & CV_FIRE_0) {
        if (!players[i].bounce) {
          if (bat_time >= 10) { 
            bat_time -= 10;
          } else {
            bat_time = 0;
          }
          light = !light;
          players[i].bounce = true;
        }
      } else {
        players[i].bounce = false;
      }
    }
    
    if (players[i].steptime == 0) {
      players[i].step = !players[i].step;
      players[i].steptime = 10;
    }
    
    wallhit = wall_collision(players[i].px+8, players[i].py+12);
    
    if (wallhit == 2) {
      if (!players[i].invuln) {
        if (players[i].hp) {
          players[i].hp--;
          boing = 5;
        }
        players[i].invuln = 60;
      }
    }
    
    if (wallhit == 3) {
      open_doors();
      crush = 5;
    }
    
    if (wallhit == 4) {
      over = true;
    }
    
    if (wallhit == 6) {
      collect_gas();
      crush = 5;
    }
    
    if (wallhit) {
      players[i].px = players[i].lastx;
      players[i].py = players[i].lasty;
    }
    
    if (players[i].invuln) {
      players[i].invuln--;
    }
    
    if (enemy_collision(players[i].px+8, players[i].py+12)) {
      if (!players[i].invuln) {
        if (players[i].hp) {
          players[i].hp--;
          boing = 5;
        }
        players[i].invuln = 60;
      }
    }
    
    if (players[i].hp == 0) {
      players[i].dead = true;
      players[i].light = false;
    }
    
    if (players[i].px > 240) {
      players[i].px = 240;
    }
    if (players[i].px < 5) {
      players[i].px = 5;
    }
    if (players[i].py > 175) {
      players[i].py = 175;
    }
    if (players[i].py < 5) {
      players[i].py = 5;
    }
  }
}

void draw_player() {
  byte i;
  for (i=0; i<2; i++) {
    thesprites[i].x = players[i].px+4;
    thesprites[i].y = players[i].py;
    
    if (players[i].hp == 3) {
      thesprites[i].tag = CV_COLOR_BLUE;
    } else if (players[i].hp == 2) {
      thesprites[i].tag = CV_COLOR_YELLOW;
    } else if (players[i].hp == 1) {
      thesprites[i].tag = CV_COLOR_LIGHT_RED;
    }
    
    if (players[i].dead) {
      thesprites[i].tag = CV_COLOR_BLACK;
    }
    
    if (players[i].step) {
      thesprites[i].name = 4*0;
    } else {
      thesprites[i].name = 4*1;
    }
  }
}


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

byte current_level = 0;

const struct wall wall_data[][8] =
{
  {
    {13, 18, 19, false, 0},
    {13, 1, 12, true, 0},
    {28, 15, 1, false, 4},
  },
  {
    {13, 12, 13, true, 1},
    {20, 1, 13, true, 1},
    {28, 18, 1, false, 4},
  },
  {
    {10, 1, 8, true, 0},
    {11, 8, 7, false, 1},
    {18, 8, 16, true, 2},
    {5, 4, 1, false, 3},
    {14, 4, 1, false, 4},
  },
  {
    {10, 1, 8, true, 0},
    {11, 8, 8, false, 1},
    {27, 20, 1, false, 3},
    {1, 8, 9, false, 2},
    {23, 15, 9, true, 1},
    {5, 2, 1, false, 4},
  },
  {
    {10, 5, 6, true, 0},
    {10, 5, 8, false, 0},
    {11, 10, 5, false, 1},
    {12, 7, 1, false, 3},
    {20, 17, 7, true, 2},
    {20, 17, 12, false, 0},
    {25, 19, 1, false, 4},
  },
  {
    {14, 1, 8, true, 2},
    {18, 10, 5, false, 1},
    {16, 15, 4, false, 0},
    {25, 15, 10, true, 0},
    {29, 22, 1, true, 3},
    {1, 8, 13, false, 1},
    {8, 2, 1, false, 4},
  },
  {
    {10, 12, 12, true, 1},
    {15, 10, 5, false, 1},
    {15, 18, 7, false, 1},
    {25, 15, 15, true, 1},
    {25, 1, 10, true, 1},
    {25, 11, 4, true, 2},
    {16, 23, 1, false, 3},
    {29, 20, 1, false, 4},
  },
  {
    {15, 10, 8, false, 0},
    {15, 18, 8, false, 0},
    {18, 13, 1, false, 6},
    {15+8, 10, 9, true, 0},
    {15, 11, 7, true, 2},
    {25, 20, 1, false, 3},
    {25, 3, 1, false, 4},
  },
  {
    {1, 10, 20, false, 1},
    {21, 5, 10, true, 1},
    {22, 12, 10, false, 2},
    {15, 13, 1, false, 3},
    {4, 4, 1, false, 4},
  },
  {
    {15, 14, 10, true, 1},
    {23, 14, 10, true, 1},
    {18, 23, 1, false, 3},
    {23, 1, 13, true, 2},
    {28, 23, 1, false, 4},
  },
  {
    {13, 18, 19, false, 0},
    {13, 1, 12, true, 0},
    {14, 5, 10, false, 1},
    {24, 5, 8, false, 2},
    {18, 23, 1, false, 3},
    {15, 3, 1, false, 4},
  },
  {
    {28, 10, 14, true, 2},
    {28, 10, 4, false, 2},
    {30, 15, 1, false, 4},
    {8, 3, 1, false, 3},
    {5, 5, 10, false, 1},
    {8, 6, 8, true, 1},
    {20, 10, 8, true, 1},
  },
};

const struct enemy enemy_data[][6] =
{
  { 
    {20, 5, true},
    {20, 15, true},
  },
  { 
    {25, 5, true},
    {20, 15, true},
  },
  { 
    {25, 5, true},
    {20, 15, true},
    {25, 5, true},
    {20, 15, true},
  },
  { 
    {13, 5, true},
    {25, 20, true},
    {25, 20, true},
    {15, 15, true},
    {16, 13, true},
  },
  { 
    {20, 5, true},
    {20, 15, true},
    {15, 11, true},
    {15, 18, true},
    {25, 20, true},
    {25, 24, true},
  },
  { 
    {20, 3, true},
    {21, 6, true},
    {20, 8, true},
    {15, 11, true},
    {10, 12, true},
    {20, 15, true},
  },
  { 
    {13, 5, true},
    {13, 5, true},
    {27, 20, true},
    {27, 20, true},
    {15, 15, true},
    {15, 15, true},
  },
  { 
    {8, 5, true},
    {8, 5, true},
    {27, 20, true},
    {27, 20, true},
    {17, 15, true},
    {17, 15, true},
  },
  { 
    {15, 5, true},
    {15, 5, true},
    {27, 20, true},
    {27, 20, true},
    {17, 15, true},
    {17, 15, true},
  },
  { 
    {10, 5, true},
    {10, 5, true},
    {27, 20, true},
    {27, 20, true},
    {20, 15, true},
    {8, 15, true},
  },
  { 
    {20, 5, true},
    {20, 15, true},
    {18, 10, true},
    {18, 10, true},
    {20, 23, true},
    {20, 24, true},
  },
  { 
    {20, 5, true},
    {20, 15, true},
    {18, 10, true},
    {18, 10, true},
    {20, 23, true},
    {20, 24, true},
  },
};


void gen_level() {
  byte i;
  
  players[0].px = 10;
  players[0].py = 100;

  players[0].lastx = 10;
  players[0].lasty = 100;
  
  players[1].px = 10;
  players[1].py = 100;
  
  players[1].lastx = 10;
  players[1].lasty = 100;
  
  for (i=0; i<6; i++) {
    enemies[i].active = false;
  }
  
  for (i=0; i<8; i++) {
    walls[i].x = wall_data[current_level][i].x;
    walls[i].y = wall_data[current_level][i].y;
    walls[i].length = wall_data[current_level][i].length;
    walls[i].vertical = wall_data[current_level][i].vertical;
    walls[i].type = wall_data[current_level][i].type;
  }
  for (i=0; i<6; i++) {
    enemies[i].x = enemy_data[current_level][i].x*8;
    enemies[i].y = enemy_data[current_level][i].y*8;
    enemies[i].active = enemy_data[current_level][i].active;
  }
  
  init_enemies();
}

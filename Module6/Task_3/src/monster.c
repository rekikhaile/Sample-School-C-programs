/* monster.c -- Implementation of monster actions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "dungeon.h"

// for defining some monster types below that can be used in the game
typedef struct {
    char name[20];  // Name of monster
    char sign;  // character to show it on map
    unsigned int hplow;  // lowest possible initial maxhp
    unsigned int hphigh;  // highest possible initial maxhp
} MonstType;

// Specifying three monster types to start with.
// Feel free to add more, or change the below
// Note that it is up to you to decide whether to use this array from createMonsters
// you may or may not use it
const MonstType types[] = {
    { "Goblin", 'G', 6, 10},
    { "Rat", 'R', 3, 5},
    { "Dragon", 'D', 15, 20}
};


/* One kind of attack done by monster.
 * The attack function pointer can refer to this.
 * 
 * Parameters:
 * game: the game state
 * monst: The monster performing attack
 */
void attackPunch(Game *game, Creature *monst) {
    printf("%s punches you! ", monst->name);
    int hitprob = 50;
    int maxdam = 4;
    if (rand() % 100 < hitprob) {
        printf("Hit! ");
        int dam = rand() % maxdam + 1;
        printf("Damage: %d ", dam);
        game->hp = game->hp - dam;
        if (game->hp <= 0)
            printf("You died!");
        printf("\n");
    } else {
        printf("Miss!\n");
    }
}

double calculateDistance(int srcX, int srcY, int dstX, int dstY)
{
  return sqrt(pow(dstX - srcX, 2) + pow(dstY - srcY, 2));
}

/* Exercise 6.3.c
 *
 * Move monster 'monst' towards the player character.
 * See exercise description for more detailed rules.
 */
void moveTowards(Game * game, Creature * monst)
{
  int newX = 0, newY = 0;
  int currentDistance = calculateDistance(monst->pos.x, monst->pos.y, game->position.x, game->position.y);
  int smallestDistance = currentDistance;
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y - 1) && calculateDistance(monst->pos.x - 1, monst->pos.y - 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = -1;
    newY = -1;
    smallestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x, monst->pos.y - 1) && calculateDistance(monst->pos.x, monst->pos.y - 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = 0;
    newY = -1;
    smallestDistance = calculateDistance(monst->pos.x, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y - 1) && calculateDistance(monst->pos.x + 1, monst->pos.y - 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = 1;
    newY = -1;
    smallestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y) && calculateDistance(monst->pos.x + 1, monst->pos.y, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = 1;
    newY = 0;
    smallestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y + 1) && calculateDistance(monst->pos.x + 1, monst->pos.y + 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = 1;
    newY = 1;
    smallestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x, monst->pos.y + 1) && calculateDistance(monst->pos.x, monst->pos.y + 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = 0;
    newY = 1;
    smallestDistance = calculateDistance(monst->pos.x, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y + 1) && calculateDistance(monst->pos.x - 1, monst->pos.y + 1, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = -1;
    newY = 1;
    smallestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y) && calculateDistance(monst->pos.x - 1, monst->pos.y, game->position.x, game->position.y) <= smallestDistance)
  {
    newX = -1;
    newY = 0;
    smallestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y, game->position.x, game->position.y);
  }

  monst->pos.x += newX;
  monst->pos.y += newY;
}

/* Exercise 6.3.d
 *
 * Move monster 'monst' away from the player character.
 * See exercise description for more detailed rules.
 */
void moveAway(Game *game, Creature *monst)
{
  /*
  int currentDistance = calculateDistance(monst->pos.x, monst->pos.y, game->position.x, game->position.y);
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y - 1) && calculateDistance(monst->pos.x - 1, monst->pos.y - 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x -= 1;
    monst->pos.y -= 1;
  }
  else if(!isBlocked(game, monst->pos.x, monst->pos.y - 1) && calculateDistance(monst->pos.x, monst->pos.y - 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.y -= 1;
  }
  else if(!isBlocked(game, monst->pos.x + 1, monst->pos.y - 1) && calculateDistance(monst->pos.x + 1, monst->pos.y - 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x += 1;
    monst->pos.y -= 1;
  }
  else if(!isBlocked(game, monst->pos.x + 1, monst->pos.y) && calculateDistance(monst->pos.x + 1, monst->pos.y, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x += 1;
  }
  else if(!isBlocked(game, monst->pos.x + 1, monst->pos.y + 1) && calculateDistance(monst->pos.x + 1, monst->pos.y + 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x += 1;
    monst->pos.y += 1;
  }
  else if(!isBlocked(game, monst->pos.x, monst->pos.y + 1) && calculateDistance(monst->pos.x, monst->pos.y + 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.y += 1;
  }
  else if(!isBlocked(game, monst->pos.x - 1, monst->pos.y + 1) && calculateDistance(monst->pos.x - 1, monst->pos.y + 1, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x -= 1;
    monst->pos.y += 1;
  }
  else if(!isBlocked(game, monst->pos.x - 1, monst->pos.y) && calculateDistance(monst->pos.x - 1, monst->pos.y, game->position.x, game->position.y) > currentDistance)
  {
    monst->pos.x -= 1;
  }
  else
  {
    // Do nothing
  }
  */
  
  int newX = 0, newY = 0;
  int currentDistance = calculateDistance(monst->pos.x, monst->pos.y, game->position.x, game->position.y);
  int greatestDistance = currentDistance;
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y - 1) && calculateDistance(monst->pos.x - 1, monst->pos.y - 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = -1;
    newY = -1;
    greatestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x, monst->pos.y - 1) && calculateDistance(monst->pos.x, monst->pos.y - 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = 0;
    newY = -1;
    greatestDistance = calculateDistance(monst->pos.x, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y - 1) && calculateDistance(monst->pos.x + 1, monst->pos.y - 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = 1;
    newY = -1;
    greatestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y - 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y) && calculateDistance(monst->pos.x + 1, monst->pos.y, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = 1;
    newY = 0;
    greatestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x + 1, monst->pos.y + 1) && calculateDistance(monst->pos.x + 1, monst->pos.y + 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = 1;
    newY = 1;
    greatestDistance = calculateDistance(monst->pos.x + 1, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x, monst->pos.y + 1) && calculateDistance(monst->pos.x, monst->pos.y + 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = 0;
    newY = 1;
    greatestDistance = calculateDistance(monst->pos.x, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y + 1) && calculateDistance(monst->pos.x - 1, monst->pos.y + 1, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = -1;
    newY = 1;
    greatestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y + 1, game->position.x, game->position.y);
  }
  
  if(!isBlocked(game, monst->pos.x - 1, monst->pos.y) && calculateDistance(monst->pos.x - 1, monst->pos.y, game->position.x, game->position.y) >= greatestDistance)
  {
    newX = -1;
    newY = 0;
    greatestDistance = calculateDistance(monst->pos.x - 1, monst->pos.y, game->position.x, game->position.y);
  }

  monst->pos.x += newX;
  monst->pos.y += newY;
}

int nextTo(int X1, int Y1, int X2, int Y2)
{
  if((X1 == X2 - 1 && Y1 == Y2 - 1) ||
     (X1 == X2 && Y1 == Y2 - 1) ||
     (X1 == X2 + 1 && Y1 == Y2 - 1) ||
     (X1 == X2 + 1 && Y1 == Y2) ||
     (X1 == X2 + 1 && Y1 == Y2 - 1) ||
     (X1 == X2 && Y1 == Y2 + 1) ||
     (X1 == X2 - 1 && Y1 == Y2 + 1) ||
     (X1 == X2 - 1 && Y1 == Y2))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/* Exercise 6.3.e
 *
 * Take action on each monster (that is alive) in 'monsters' array.
 * Each monster either attacks or moves (or does nothing if no action is specified)
 */
void monsterAction(Game * game)
{
  for(unsigned int i = 0; i < game->numMonsters; i++)
  {
    if(game->monsters[i].hp == 0)
    {
      continue;
    }
    
    if(nextTo(game->position.x, game->position.y, game->monsters[i].pos.x, game->monsters[i].pos.y))
    {
      game->monsters[i].attack(game, &game->monsters[i]);
    }
    else
    {
      game->monsters[i].move(game, &game->monsters[i]);
    }
  }
}


/* Exercise 6.3.b
 *
 * Create opts.numMonsters monsters and position them on valid position
 * in the the game map. The moster data (hitpoints, name, map sign) should be
 * set appropriately (see exercise instructions) 
 */
void createMonsters(Game * game)
{
  int x, y;
  int hpBonus;
  int digit100, digit10, digit1;
  
  game->numMonsters = 0;
  
  game->monsters = (Creature *)malloc(sizeof(Creature) * game->opts.numMonsters);
  
  for(unsigned int i = 0; i < game->opts.numMonsters; i++)
  {
    do
    {
      x = rand() % game->opts.mapWidth;
      y = rand() % game->opts.mapHeight;
    }
    while(isBlocked(game, x, y) || game->map.tile[y][x] != TILE_OPEN);
    
    game->monsters[i].pos.x = x;
    game->monsters[i].pos.y = y;
    
    hpBonus = rand() % 7;
    
    if(hpBonus < 2)
    {
      hpBonus = 0;
    }
    else if(hpBonus > 5)
    {
      hpBonus += 2;
    }
    else
    {
      hpBonus -= 2;
    }
    
    game->monsters[i].maxhp = 10 + hpBonus;
    game->monsters[i].hp = game->monsters[i].maxhp;
    
    game->monsters[i].sign = 'M';
    
    memset(game->monsters[i].name, 0, sizeof(char) * 20);
    
    digit100 = (i - (i % 100)) / 100;
    digit10 = (i - digit100 * 100) / 10;
    digit1 = i - digit100 * 100 - digit10 * 10;
    
    game->monsters[i].name[0] = 'M';
    game->monsters[i].name[1] = '0' + digit100;
    game->monsters[i].name[2] = '0' + digit10;
    game->monsters[i].name[3] = '0' + digit1;
    
    game->monsters[i].attack = attackPunch;
    game->monsters[i].move = moveTowards;
    
    game->numMonsters++;
  }
}

/* Determine whether monster moves towards or away from player character.
 */
void checkIntent(Game *game)
{
    for (unsigned int i = 0; i < game->numMonsters; i++) {
        Creature *m = &game->monsters[i];
        if (m->hp <= 2) {
            m->move = moveAway;
        } else {
            m->move = moveTowards;
        }
        if (m->hp < m->maxhp)
            m->hp = m->hp + 0.1;  // heals a bit every turn
    }
}


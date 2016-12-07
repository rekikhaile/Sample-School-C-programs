/* userif.c -- interacting with user, showing map and other output
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "dungeon.h"

/* Determine what character to show on the given map position.
 * 
 * Parameters:
 * game: the game state
 * x,y: map coordinates
 * 
 * Returns: character to be displayed
 */
char locationChar(const Game *game, int x, int y)
{
	const Map *map = &game->map;
	char c = '?';
	switch(map->tile[y][x]) {
	case TILE_WALL:
		c = '#';
		break;
	case TILE_OPEN:
		c = '.';
		break;
	case TILE_ROOM:
		c = '.';
		break;
	}

        for (unsigned int i = 0; i < game->numMonsters; i++) {
            if (game->monsters[i].pos.x == x && game->monsters[i].pos.y == y) {
                c = game->monsters[i].sign;
                break;
            }
        }
	
	if (game->position.x == x && game->position.y == y) {
		c = '*';
	}
	return c;
}


/* Show full map, without adhering to the visibility rules
 */
void printMap(const Game *game)
{
    const Options *opt = &game->opts;
	for (int i = 0; i < (int) opt->mapHeight; i++) {
		for (int j = 0; j < (int) opt->mapWidth; j++) {
			char c = locationChar(game, j, i);
			printf("%c", c);
		}
		printf("\n");
	}
}

// determine whether x1,y1 is within line of sight from x0,y0
// (assume |x1-x0| > |y1-y0|)
int isVisibleX(const Map *map,int x0, int x1, int y0, int y1)
{
	int x = x0;
	int y = y0;
	int dx = (x1 >= x0) ? 1 : -1;
	float cy = y0;
	int vis = 1;
	while (x != x1) {
		float dy = ((float)y1 - y) / abs(x1 - x);
		if (!vis) return 0;
		if (map->tile[y][x] == TILE_WALL) {
			vis = 0;
		}
		if (dy < 0)
			cy += ceil(dy);
		else
			cy += floor(dy);
		y = cy;
		x += dx;
	}
	return vis;
}

// determine whether x1,y1 is within line of sight from x0,y0
// (assume |y1-y0| > |x1-x0|)
int isVisibleY(const Map *map,int x0, int x1, int y0, int y1)
{
	int x = x0;
	int y = y0;
	int dy = (y1 >= y0) ? 1 : -1;
	float cx = x0;
	int vis = 1;
	while (y != y1) {
		float dx = ((float)x1 - x) / abs(y1 - y);
		if (!vis) return 0;
		if (map->tile[y][x] == TILE_WALL) {
			vis = 0;
		}
		if (dx < 0)
			cx += ceil(dx);
		else
			cx += floor(dx);
		x = cx;
		y += dy;
	}
	return vis;
}

// Crude algorithm for determining whether x1,y1 is within line of sight from x0,y0
int isVisible(const Map *map, int x0, int x1, int y0, int y1)
{
	if (x0 == x1 && y0 == y1)
		return 1;

	if (abs(x1-x0) > abs(y1-y0)) {
		return(isVisibleX(map, x0, x1, y0, y1));
	} else {
		return(isVisibleY(map, x0, x1, y0, y1));
	}
}


/* Show map around character, adhering to the visibility rules
 */
void printMe(const Game *game)
{      
    const Options *opt = &game->opts;
	for (int i = game->position.y - (int)opt->visibility; 
	     i <= game->position.y + (int)opt->visibility; i++) {
		for (int j = game->position.x - (int)opt->visibility;
		     j <= game->position.x + (int)opt->visibility; j++) {
			char c = ' ';
			if (i >= 0 && i < (int) opt->mapHeight &&
			    j >= 0 && j < (int) opt->mapWidth) {
				if (isVisible(&game->map, game->position.x, j,
					      game->position.y, i))
					c = locationChar(game, j, i);
			}
			printf("%c", c);
		}
		printf("\n");
	}
}

/* Exercise 6.3.a
 *
 * Returns 1, if player character is not allowed to move to
 * given position (x,y). Returns 0, if moving is allowed
 */
int isBlocked(Game * game, int x, int y)
{
  int retVal = 0;
  
  if(x < 0 || y < 0 || (unsigned int)x >= game->opts.mapWidth || (unsigned int)y >= game->opts.mapHeight)
  {
    retVal = 1;
  }
  
  if(!retVal)
  {
    if(game->position.x == x && game->position.y == y)
    {
      retVal = 1;
    }
  }
  
  if(!retVal)
  {
    if(game->map.tile[y][x] == TILE_WALL)
    {
      retVal = 1;
    }
  }
  
  if(!retVal)
  {
    for(unsigned int i = 0; i < game->numMonsters; i++)
    {
      if(game->monsters[i].pos.x == x && game->monsters[i].pos.y == y)
      {
	retVal = 1;
	break;
      }
    }
  }
  
  return retVal;
}

/* Attack the given monster
 */
void attackMonster(Creature *monst)
{
    int hitprob = 50;
    int maxdam = 4;
    if (rand() % 100 < hitprob) {
        printf("Hit! ");
        int dam = rand() % maxdam + 1;
        printf("Damage: %d ", dam);
        monst->hp = monst->hp - dam;
        if (monst->hp <= 0) {
            printf("Monster died!");
            monst->pos.x = -1;
            monst->pos.y = -1;
        } else {
            printf("Monster HP: %d (%d) ", (int)monst->hp, monst->maxhp);
        }

    } else {
        printf("Miss!");
    }
}

/* Check if there is monster in given location.
 * If yes, attack it.
 */
int checkMonster(Game *game, int x, int y)
{
    Creature *monst = game->monsters;
    for (unsigned int i = 0; i < game->numMonsters; i++) {
        if (monst[i].pos.x == x && monst[i].pos.y == y) {
            printf("Attacking monster! ");
            attackMonster(&monst[i]);
            printf("\n");
            return 1;
        }
    }
    return 0;
}

/* Move to given location, if possible
 */
void moveIfCan(Game *game, int x, int y)
{
    if (checkMonster(game, x, y)) {
        return;
    }
    if (!isBlocked(game, x, y)) {
        game->position.x = x;
        game->position.y = y;
    } else {
        printf("Blocked\n");
    }
}

/* Show map and player hit points
 * Either full map is shown, or just the visible portion around player,
 * depending on opts.showAll value
 */
void showMap(const Game *game)
{
    if (game->opts.showAll) {
        printMap(game);
    } else {
        printMe(game);
    }
    printf("HP: %d(%d)\n", (int)game->hp, game->maxhp);
}

/* Parse command given by user
 * 
 * Parameters:
 * game: game state
 * buf: one line read from user
 * 
 * Returns: 1, if game continues; 0 if game ends
 */
int doCommand(Game *game, char *buf)
{
    int newx = game->position.x;
    int newy = game->position.y;
	switch(tolower(buf[0])) {
	case 'n':
		newy--;
		break;
	case 's':
		newy++;
		break;
	case 'w':
		newx--;
		break;
	case 'e':
		newx++;
		break;
        case 'q':
                return 0; // game over
        case 't':
            save_game(game, &buf[2]);
            break;
                
	default:
		printf("what???\n");
		break;
	}
        moveIfCan(game, newx, newy);
        monsterAction(game);
        checkIntent(game);
        if (game->hp <= 0)
            return 0;  // game over!
        if (game->hp < game->maxhp)
            game->hp = game->hp + 0.1;  // heals a bit every turn
        
        return 1;
}

/* Exercise 6.3.f and 6.3.g
 *
 * Save game state to file
 * 
 * Parameters:
 * game: game state
 * fname: Name of the file to be saved
 * 
 * Returns: -1 if saving failed; 0 if saving was succesful
 */
int save_game(Game * game, const char * fname)
{
  /*
  char * filename = (char *)malloc(sizeof(char) * (strlen(fname) - 1));
  strncpy(filename, fname, (strlen(fname) - 1));
  filename[strlen(fname) - 1] = '\0';
  */
  
  FILE * f = fopen(fname, "w");
  
  if(f != NULL)
  {
    fprintf(f, "%ul", game->opts.mapHeight);
    fprintf(f, "%ul", game->opts.mapWidth);
    
    for(unsigned int i = 0; i < game->opts.mapHeight; i++)
    {
      for(unsigned int j = 0; j < game->opts.mapWidth; j++)
      {
	fprintf(f, "%dl", game->map.tile[i][j]);
      }
    }
    
    fprintf(f, "%ul", game->numMonsters);
    
    for(unsigned int i = 0; i < game->numMonsters; i++)
    {
      for(int j = 0; j < 20; j++)
      {
	fprintf(f, "%c", game->monsters[i].name[j]);
      }
      
      fprintf(f, "%c", game->monsters[i].sign);
      
      fprintf(f, "%dl", game->monsters[i].pos.x);
      fprintf(f, "%dl", game->monsters[i].pos.y);
      
      fprintf(f, "%fl", game->monsters[i].hp);
      
      fprintf(f, "%ul", game->monsters[i].maxhp);
      
      //fprintf(f, "%pl", (void *)game->monsters[i].move);
      //fprintf(f, "%pl", (void *)game->monsters[i].attack);
      
      /*
      unsigned char * temp = (unsigned char *)game->monsters[i].move;

      for(unsigned int j = 0; j < sizeof(game->monsters[i].move); j++)
      {
	fprintf(f, "%c", temp[j]);
      }
      
      temp = (unsigned char *)game->monsters[i].attack;
      
      for(unsigned int j = 0; j < sizeof(game->monsters[i].attack); j++)
      {
	fprintf(f, "%c", temp[j]);
      }
      */
      
      if(game->monsters[i].move == moveTowards)
      {
	fprintf(f, "%dl", 1);
      }
      else if(game->monsters[i].move == moveAway)
      {
	fprintf(f, "%dl", 2);
      }
      else
      {
	fprintf(f, "%dl", 0);
      }
      
      if(game->monsters[i].attack == attackPunch)
      {
	fprintf(f, "%dl", 1);
      }
      else
      {
	fprintf(f, "%dl", 0);
      }
    }
    
    fprintf(f, "%dl", game->position.x);
    fprintf(f, "%dl", game->position.y);

    fprintf(f, "%fl", game->hp);

    fprintf(f, "%ul", game->maxhp);
    
    fprintf(f, "%ul", game->opts.numRooms);
    fprintf(f, "%ul", game->opts.minRoomSize);
    fprintf(f, "%ul", game->opts.maxRoomSize);
    fprintf(f, "%ul", game->opts.mapWidth);
    fprintf(f, "%ul", game->opts.mapHeight);
    fprintf(f, "%ul", game->opts.numMonsters);
    fprintf(f, "%ul", game->opts.visibility);
    fprintf(f, "%dl", game->opts.showAll);
    
    fclose(f);
  
    return 0;
  }
  else
  {
    return -1;
  }
}

/* Exercise 6.3.f and 6.3.g
 *
 * Load game state from file.
 * 
 * Parameters:
 * game: game state
 * fname: Name of the file to be loaded
 * 
 * Returns: -1 if loading failed; 0 if loading was succesful
 */
int load_game(Game * game, const char * fname)
{
  FILE * f = fopen(fname, "r");
  
  if(f != NULL)
  {
    fscanf(f, "%ul", &game->opts.mapHeight);
    fscanf(f, "%ul", &game->opts.mapWidth);
    
    game->map.tile = (Tile **)malloc(sizeof(Tile *) * game->opts.mapHeight);
    
    for(unsigned int i = 0; i < game->opts.mapHeight; i++)
    {
      game->map.tile[i] = (Tile *)malloc(sizeof(Tile) * game->opts.mapWidth);
      
      for(unsigned int j = 0; j < game->opts.mapWidth; j++)
      {
	fscanf(f, "%dl", (int *)&game->map.tile[i][j]);
      }
    }
    
    fscanf(f, "%ul", &game->numMonsters);
    
    game->opts.numMonsters = game->numMonsters;
    
    game->monsters = (Creature *)malloc(sizeof(Creature) * game->numMonsters);
    
    for(unsigned int i = 0; i < game->numMonsters; i++)
    {
      for(int j = 0; j < 20; j++)
      {
	fscanf(f, "%c", &game->monsters[i].name[j]);
      }
      
      fscanf(f, "%c", &game->monsters[i].sign);
      
      fscanf(f, "%dl", &game->monsters[i].pos.x);
      fscanf(f, "%dl", &game->monsters[i].pos.y);
      
      fscanf(f, "%fl", &game->monsters[i].hp);
      
      fscanf(f, "%ul", &game->monsters[i].maxhp);
      
      //fscanf(f, "%pl", (void **)&game->monsters[i].move);
      //fscanf(f, "%pl", (void **)&game->monsters[i].attack);
      
      //game->monsters[i].move = moveTowards;
      //game->monsters[i].attack = attackPunch;
      
      /*
      for(unsigned int j = 0; j < sizeof(game->monsters[i].move); j++)
      {
	fscanf(f, "%c", ((char *)&game->monsters[i].move)[j]);
      }
      
      for(unsigned int j = 0; j < sizeof(game->monsters[i].attack); j++)
      {
	fscanf(f, "%c", ((char *)&game->monsters[i].attack)[j]);
      }
      */
      
      int temp = -1;
      
      fscanf(f, "%dl", &temp);
      
      if(temp == 1)
      {
	game->monsters[i].move = moveTowards;
      }
      else if(temp == 2)
      {
	game->monsters[i].move = moveAway;
      }
      else
      {
	game->monsters[i].move = NULL;
      }
      
      fscanf(f, "%dl", &temp);
      
      if(temp == 1)
      {
	game->monsters[i].attack = attackPunch;
      }
      else
      {
	game->monsters[i].attack = NULL;
      }
    }
    
    fscanf(f, "%dl", &game->position.x);
    fscanf(f, "%dl", &game->position.y);

    fscanf(f, "%fl", &game->hp);

    fscanf(f, "%ul", &game->maxhp);
    
    fscanf(f, "%ul", &game->opts.numRooms);
    fscanf(f, "%ul", &game->opts.minRoomSize);
    fscanf(f, "%ul", &game->opts.maxRoomSize);
    fscanf(f, "%ul", &game->opts.mapWidth);
    fscanf(f, "%ul", &game->opts.mapHeight);
    fscanf(f, "%ul", &game->opts.numMonsters);
    fscanf(f, "%ul", &game->opts.visibility);
    fscanf(f, "%dl", &game->opts.showAll);
    
    fclose(f);
    
    /*
    int discard;
    
    fscanf(stdin, "%d", &discard);
    */
    
    return 0;
  }
  else
  {
    return -1;
  }
}






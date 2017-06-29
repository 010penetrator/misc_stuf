#include <stdio.h>
#include <stdlib.h>
#include "conio.h" //need getch replacement under Linux

#define MAP_WIDTH 180
#define MAP_HEIGHT 120
#define SCR_WIDTH 40
#define SCR_HEIGHT 20
#define UP 65
#define DOWN 66
#define LEFT 68
#define RIGHT 67

int a=0; // mem keyb
int i,j,t,T,t0,t1,p=0;
int lvl=1;
int pos_x,pos_y=0;
//int pos_y=MAP_HEIGHT/2;
//int pos_x=MAP_WIDTH/2;
int hp=25;
int map [MAP_HEIGHT][MAP_WIDTH]; // тут нужен тип из двух 2 бит, так как всего 4 состояния: пусто, стена, еда, выход . но такого типа пока нет !!!

void DrawScreen();
void GenerateLevel();
void Move();

int main() {
	time_t t;
	srand((unsigned) time(&t));

	GenerateLevel();

	while (1) {
		DrawScreen();
		if (hp<1) {printf("\n");exit(0);}
		a=getch();
		// Exit on q
		if (a==113) {printf("\n");exit(0);}
		Move();
	}
}

void Move() {
	switch(a) {
		case UP :
			pos_y--;
			if (pos_y==-1) {pos_y=0;}
			break;
		case DOWN :
			pos_y++;
			if (pos_y==MAP_HEIGHT) {pos_y=MAP_HEIGHT-1;}
			break;
		case LEFT :
			pos_x--;
			if (pos_x==-1) {pos_x=0;}
			break;
		case RIGHT :
			pos_x++;
			if (pos_x==MAP_WIDTH) {pos_x=MAP_WIDTH-1;}
			break;
	}
	switch (map[pos_y][pos_x]) {
		case 1 :
			hp--;
			map[pos_y][pos_x]=0;
			break;
		case 2 :
			hp++;
			map[pos_y][pos_x]=0;
			break;
		case 3 :
			lvl++;
			GenerateLevel();
			break;
	}
}

void GenerateLevel() {
	switch(lvl) {
		case 1 :
			for (i=0; i<MAP_HEIGHT; i++) {
				for (j=0; j<MAP_WIDTH; j++) {
					// Ромбы
					if (abs(abs(i-MAP_HEIGHT/2)+abs(j-MAP_WIDTH/2))%10 < 4) {
						map[i][j]=1;
					}
					// Border of exits
					if ( i==0 || j==0 || i==MAP_HEIGHT-1 || j==MAP_WIDTH-1 ) {
						map[i][j]=3;
					}
					else {
						if ( rand() % 100 > 96 ) {
							// Put food
							map[i][j]=2;
						}
					}
				}
			}
			pos_x=MAP_WIDTH/2;
			pos_y=MAP_HEIGHT/2;
			map[pos_y][pos_x]=0;
			break;
		case 2 :
			// Border wall
			for (i=0; i<MAP_HEIGHT; i++) {
				for (j=0; j<MAP_WIDTH; j++) {
					if ( i==0 || j==0 || i==MAP_HEIGHT-1 || j==MAP_WIDTH-1 ) {
						map[i][j]=1;
					}
					else {map[i][j]=0;}
					if ( rand() % 1000 > 998 ) {
						// Put food
						map[i][j]=2;
					}
				}
			}
			// Параболы
			T=4; t=T; t0=1;
			for (j=0; j<MAP_WIDTH; j++) {
				for (i= (t0+p-abs(t0-p))/2; i <= (t0+p+abs(t0-p))/2; i++) {
					map[i%MAP_HEIGHT][j]=1;
					map[MAP_HEIGHT-i%MAP_HEIGHT][j]=1;
				}
				p=t0;
				t0+=t--;
				if (T+t<0) {T++;t=T;}
			}
			pos_x = 5;
			pos_y = MAP_HEIGHT/2;
			// Exit
			map[MAP_HEIGHT/2][MAP_WIDTH-3]=3;
			break;
		case 3 :
			// Paradise
			for (i=0; i<MAP_HEIGHT; i++) {
				for (j=0; j<MAP_WIDTH; j++) {
					map[i][j]=2;
					if ( i==0 || j==0 || i==MAP_HEIGHT-1 || j==MAP_WIDTH-1 ) {
						map[i][j]=1;
					}
				}
			}
			pos_x=MAP_WIDTH/2;
			pos_y=MAP_HEIGHT/2;
	}
}

void DrawScreen() {
	system("clear"); // BAD flashes !!!
	for (i=pos_y-SCR_HEIGHT/2; i<pos_y+SCR_HEIGHT/2; i++) {
		for (j=pos_x-SCR_WIDTH/2; j<pos_x+SCR_WIDTH/2; j++) {
			// Draw our spacedigger
			if ( i==pos_y && j==pos_x ) {
				if (hp==0) {
					printf("X");
				}
				else {
					printf("@");
				}
			}
			else {
				if ( i<0 || j<0 || i>MAP_HEIGHT || j>MAP_WIDTH) {
					// Draw noth outside map
					printf(" ");
				}
				else {
					switch(map[i][j]) {
						case 0 :
							printf(" ");
							break;
						case 1 : 
							printf("#");
							break;
						case 2 : 
							printf("o");
							break;
						case 3 : 
							printf("H");
							break;
					}
				}
			}
		}
		printf("\n");
	}
	// Status line
	printf("\nHealth:%d\t",hp);
	printf("Input:%d\t",a);
	printf("Level:%d\n",lvl);
	//printf("x:%d\t",pos_x);
}


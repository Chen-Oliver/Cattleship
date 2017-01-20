#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "networking.h"

int board [10][10]= {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
int *sdArr;
void addpiece(int y, int x){//adds the piece to board[][]
  int a = y-2;
  int b = x-5;
  a = a/2;
  b = b/4;
  board[a][b] = 1;
  }

struct coords{ //stores coordinates of on screen cursor(lowest is y=0,x=0)
  int x,y;
}cursor;
WINDOW *status_window;
int winHeight=5;
int winWidth=50;
//set cursor y and x to initY and initX if not already then move them by deltaY and deltaX
void movecursor(int initY,int initX, int deltaY,int deltaX){
    if((cursor.y)!= initY) cursor.y = initY;
    if((cursor.x)!= initX) cursor.x = initX;
    cursor.y = (cursor.y)+deltaY;
    cursor.x = (cursor.x)+deltaX;
    move(cursor.y,cursor.x);
}
WINDOW *create_newwin(int height, int width, int starty, int startx){
  WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}
//y and x: coords of where to print
//whichboard: either 1 or 2, specificies if "Your Board" or "Opponent Board" is printed
void printboard(int y,int x,int whichPlayer){
  movecursor(y,x,0,0);
  if(whichPlayer==1)printw("Your Board\n");
  else if(whichPlayer==2)printw("Opponent's Board\n");
  int i=0;
  while(i!=11){
    char str[50];
    movecursor(cursor.y,cursor.x,1,0);
    if(i==0)
      printw("|  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|\n");
    else{
      if(i==10)
      sprintf(str,"|%d|   |   |   |   |   |   |   |   |   |   |\n",i);
      else sprintf(str,"| %d|   |   |   |   |   |   |   |   |   |   |\n",i);
      printw(str);
      if(i!=10){
      movecursor(cursor.y,cursor.x,1,0);
      printw("____________________________________________\n");
      }
    }
    i++;
    }
    movecursor(y,x,2,5); //moves cursor to the first box on the board
    refresh();
  }
//starts graphics
void startGame(){
  initscr();
  noecho();
  keypad(stdscr,TRUE);
  printboard(0,0,1);
  refresh();
  status_window = create_newwin(winHeight,winWidth,23,0);
  wrefresh(status_window);
}

void showBoard(){//press e to show board[][]
  move(25, 0);  //move away from board //debugging
  int i,j=0;
  while (i!=10){
    j=0;
    while(j!=10){
      printw("| %d ", board[i][j]);
      j++;
    }
    printw("|\n");
    i++;
  }
  move(cursor.y, cursor.x);
}

int checkstack(int y,int x){//check board[][] if boats will stack
  int a = y-2;
  int b = x-5;
  a = a/2;
  b = b/4;
  if (board[a][b]==1)
    return 1;//return 1 if you will stack
  return 0;//return 0 if there is no piece there
}

int checkshipPH(int n) {//horizontal stacking check
  int ans=0;
  if(n==6){//check the pieces' placement
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y, cursor.x+4);
  }
  if(n==5 || n==4){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y, cursor.x+4);
    ans+=checkstack(cursor.y, cursor.x-4);
  }

  if(n==3){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y, cursor.x+4);
    ans+=checkstack(cursor.y, cursor.x-4);
    ans+=checkstack(cursor.y, cursor.x+8);
  }

  if(n==2){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y, cursor.x+4);
    ans+=checkstack(cursor.y, cursor.x-4);
    ans+=checkstack(cursor.y, cursor.x+8);
    ans+=checkstack(cursor.y, cursor.x-8);
  }
  if(ans>0)
    return 0;//return 0 if you will stack
  return 1; //return 1 if you will not stack
}


int checkshipPV(int n) {//vertical ship stacking check
  int ans=0;
  if(n==6){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y+2, cursor.x);
  }
  if(n==5 || n==4){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y+2, cursor.x);
    ans+=checkstack(cursor.y-2, cursor.x);
  }

  if(n==3){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y+2, cursor.x);
    ans+=checkstack(cursor.y-2, cursor.x);
    ans+=checkstack(cursor.y+4, cursor.x);
  }

  if(n==2){
    ans+=checkstack(cursor.y, cursor.x);
    ans+=checkstack(cursor.y+2, cursor.x);
    ans+=checkstack(cursor.y-2, cursor.x);
    ans+=checkstack(cursor.y+4, cursor.x);
    ans+=checkstack(cursor.y-4, cursor.x);
  }

  if (ans>0)
    return 0;
  return 1;
}

int checkshipV( int n){//vertical boundary ship check
  if(n==6){
    if (cursor.y+2>20){
      return 0;
    }
  }

  if(n==4||n==5){
    if (cursor.y+2>20 || cursor.y-2<1){
      return 0;
    }
  }
  if(n==3){
    if (cursor.y+4>20 || cursor.y-2<1){
      return 0;
    }
  }
  if(n==2){
    if (cursor.y+4>20 || cursor.y-4<1){
      return 0;
    }
    return 1;
  }
  return -1;
}

//chooses ship to be placed
void chooseShipV(int n){
  if(n==6){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y+2, cursor.x);
    printw( "O");
    addpiece(cursor.y+2, cursor.x);
  }
  if(n==5 || n==4){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y+2, cursor.x);
    printw( "O");
    addpiece(cursor.y+2, cursor.x);
    move(cursor.y-2, cursor.x);
    printw( "O");
    addpiece(cursor.y-2, cursor.x);
  }

  if(n==3){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y+2, cursor.x);
    printw( "O");
    addpiece(cursor.y+2, cursor.x);
    move(cursor.y-2, cursor.x);
    printw( "O");
    addpiece(cursor.y-2, cursor.x);
    move(cursor.y+4, cursor.x);
    printw( "O");
    addpiece(cursor.y+4, cursor.x);
  }

  if(n==2){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y+2, cursor.x);
    printw( "O");
    addpiece(cursor.y+2, cursor.x);
    move(cursor.y-2, cursor.x);
    printw( "O");
    addpiece(cursor.y-2, cursor.x);
    move(cursor.y+4, cursor.x);
    printw( "O");
    addpiece(cursor.y+4, cursor.x);
    move(cursor.y-4, cursor.x);
    printw( "O");
    addpiece(cursor.y-4, cursor.x);
  }

}
int checkshipH( int n){//horizontal boundary ship check
  if(n==6){
    if (cursor.x+4>43){
      return 0;
    }
  }

  if(n==4||n==5){
    if (cursor.x+4>43 || cursor.x-4<5){
      return 0;
    }
  }
  if(n==3){
    if (cursor.x+8>43 || cursor.x-4<5){
      return 0;
    }
  }
  if(n==2){
    if (cursor.x+8>43 || cursor.x-8<5){
      return 0;
    }
    return 1;
  }
  return -1;
}


//chooses ship to be placed
void chooseShipH(int n){
  if(n==6){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y, cursor.x+4);
    printw( "O");
    addpiece(cursor.y, cursor.x+4);
  }
  if(n==5 || n==4){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y, cursor.x+4);
    printw( "O");
    addpiece(cursor.y, cursor.x+4);
    move(cursor.y, cursor.x-4);
    printw( "O");
    addpiece(cursor.y, cursor.x-4);
  }

  if(n==3){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y, cursor.x+4);
    printw( "O");
    addpiece(cursor.y, cursor.x+4);
    move(cursor.y, cursor.x-4);
    printw( "O");
    addpiece(cursor.y, cursor.x-4);
    move(cursor.y, cursor.x+8);
    printw( "O");
    addpiece(cursor.y, cursor.x+8);
  }

  if(n==2){
    printw( "O");
    addpiece(cursor.y, cursor.x);
    move(cursor.y, cursor.x+4);
    printw( "O");
    addpiece(cursor.y, cursor.x+4);
    move(cursor.y, cursor.x-4);
    printw( "O");
    addpiece(cursor.y, cursor.x-4);
    move(cursor.y, cursor.x+8);
    printw( "O");
    addpiece(cursor.y, cursor.x+8);
    move(cursor.y, cursor.x-8);
    printw( "O");
    addpiece(cursor.y, cursor.x-8);
  }
}
void placeShips(){
  int x = 6;
  while(x){
    switch(getch()){
    case KEY_UP:
      if(cursor.y > 2) cursor.y -= 2;//at y=4(row 2) moves to y=2(row 1)
      break;
    case KEY_DOWN:
      if(cursor.y < 19) cursor.y += 2; //at y=18(row 9) moves to y=20(row 10)
      break;
    case KEY_LEFT:
      if(cursor.x > 5) cursor.x -= 4;//at x=9(column 2) moves to x=5(column 1)
      break;
    case KEY_RIGHT:
      if(cursor.x < 39) cursor.x += 4;//at x=37(column 9) moves to x=41(column 10)
      break;
    case 'h':
      if(checkshipH(x) && checkshipPH(x)){
	if(x>1){
	  chooseShipH(x);
	  x--;
	}
      }
      break;
    case 'v':
      if(checkshipV(x) && checkshipPV(x)){
	if(x>1){
	  chooseShipV(x);
	  x--;
	}
      }
      break;
  /*  case 'e':
      showBoard(); */
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
    if(x==1){
      printboard(38,0,2);
      break;
    }
  }
  //endwin();
}

//allows player to use arrow keys to move around board(s)
void moveNplace(){
  while(sdArr[0]==1){
  switch(getch()){
    case KEY_UP:
      if(cursor.y > 40) cursor.y -= 2;
      break;
    case KEY_DOWN:
      if(cursor.y < 57) cursor.y += 2;
      break;
    case KEY_LEFT:
      if(cursor.x > 5) cursor.x -= 4;
      break;
    case KEY_RIGHT:
      if(cursor.x < 39) cursor.x += 4;
      break;
    case 's':
      printw("X");
      sdArr[0]=0;
      break;
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
  }
}

int main( int argc, char *argv[] ){
  char *host;
  if (argc != 2) {
    printf("host not specified, attempting connection to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  char buffer[MESSAGE_BUFFER_SIZE];
  int sd;

  int semkey = ftok("makefile",23);
  int semid = semget(semkey,1,0);
  int availConnections = semctl(semid,0,GETVAL);
  if(availConnections==0)printf("A game is ongoing. Try again later.\n"); //semaphore is 0,no more connecting

 else{  //need a way to ensure that both players have closed game before new people can join
  sd = client_connect(host);
  int shmkey = ftok("makefile",6);
  int shmid = shmget(shmkey,3*sizeof(int),0);
  sdArr = (int*)shmat(shmid,0,0);
  startGame();
  placeShips();
  //mvwprintw(status_window,2,1,"READY FOR BATTLE!!!");
  //wrefresh(status_window);
  while(1){ //while(gameNotEnd)
  moveNplace();
}
  endwin();
}
  return 0;
}

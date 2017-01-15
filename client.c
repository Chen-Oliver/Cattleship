#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "networking.h"

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
//allows player to use arrow keys to move around board(s)
void moveNplace(){
  int a = 1;
  while(a){
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
    case 's':
      printw("O");
      a--;
      break;
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
  }
}

//useless function just for testing stuff out
void place(int deltaX){
  cursor.x = (cursor.x) + deltaX;
  move(cursor.y,cursor.x);
  printw("O");
  refresh();
  sleep(5);
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
  int i = 10;
  startGame();
  moveNplace();
  //following stuff is mostly useless, just testing stuff out
  write(sd,"graphics working a ok",100);
  while(i!=0){

  /*while (1) {
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;

    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  } */
    read(sd, buffer, sizeof(buffer));
    mvwprintw(status_window,2,1,buffer);
    wrefresh(status_window);
    place(4);
    write(sd,"received the bomb position",100);
    i--;
  }
  endwin();
}
  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

struct coords{ //stores coordinates of on screen cursor(lowest is y=0,x=0)
  int x,y;
}cursor;

//set cursor y and x to initY and initX if not already then move them by deltaY and deltaX
void movecursor(int initY,int initX, int deltaY,int deltaX){
    if((cursor.y)!= initY) cursor.y = initY;
    if((cursor.x)!= initX) cursor.x = initX;
    cursor.y = (cursor.y)+deltaY;
    cursor.x = (cursor.x)+deltaX;
    move(cursor.y,cursor.x);
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

void startGame(){
  initscr();
  noecho();
  keypad(stdscr,TRUE);
  printboard(0,0,1);
  refresh();
}
//allows player to use arrow keys to move around board(s)
void moveNplace(){
  while(1){
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
      break;
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
  }
}
int main(){
  startGame();
  moveNplace();
  return 0;
}

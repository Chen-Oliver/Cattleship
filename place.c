#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>



//This version place teh ships
//need to make a check function
//need to simplify the place pieces function
//need to put ship info into a file


struct coords{
  int x,y;
}cursor;

void printboard(){
  printw(
	  "|  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|\n"
	   "| 1|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 2|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 3|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 4|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 5|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 6|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 7|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 8|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "| 9|   |   |   |   |   |   |   |   |   |   |\n"
	   "____________________________________________\n"
	   "|10|   |   |   |   |   |   |   |   |   |   |\n"
	 );
}
void startGame(){
  initscr();
  noecho();
  keypad(stdscr,TRUE);
  printw("Board: \n");
  printboard();
  cursor.x = 5;
  cursor.y = 2;
  move(cursor.y,cursor.x);
  refresh();
}

//chooses ship to be placed
void chooseShip(int n){
  if (n==5){
    printw("O");
    move(cursor.y,cursor.x+4);
    printw("O");
  }
  if(n==4 || n==3){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
    move(cursor.y, cursor.x-4);
    printw( "O");
  }
  if(n==2){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
    move(cursor.y, cursor.x-4);
    printw( "O");
    move(cursor.y, cursor.x+8);
    printw( "O");
  }
  if(n==1){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
    move(cursor.y, cursor.x-4);
    printw( "O");
    move(cursor.y, cursor.x+8);
    printw( "O");
    move(cursor.y, cursor.x-8);
    printw( "O");
  }
}

void placeShips(){
  int x = 5;
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
    case 's':
      chooseShip(x);
      x--;
      break;
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
  }
  sleep(1);
  endwin();
}

int main(){
  startGame();
  placeShips();
  return 0;
}

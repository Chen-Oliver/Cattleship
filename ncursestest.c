#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

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

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>




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
	   "| 6|   |   |   |   |   | O | O | O |   |   |\n"
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

void putShipV(int x){
  int n=x;
  int m=1;
  printw("O");
  while (n){
    move(cursor.y+(2*m),cursor.x);
    printw("O");
    n--;
    if(n==0){
      move(cursor.y-(2*m),cursor.x);
      printw("O");
      n--;
      m++;
    }
  }
}

      
      
    
    

//chooses ship to be placed
void chooseShipV(int n){
  if (n==6){
    putShipH(2);
  }
  if(n==4 || n==5){
    putShipH(3);
  }
  if(n==3){
    putShipH(4);
  }
  if(n==2){
    putShipH(5);
  }
}


void putShipH(int x){
  int n=x;
  int m=1;
  printw("O");
  while (n){
    move(cursor.y,cursor.x+(4*m));
    printw("O");
    n--;
    if(n==0){
      move(cursor.y,cursor.x-(4*m));
      printw("O");
      n--;
      m++;
    }
  }
}

      
      
    
    

//chooses ship to be placed
void chooseShipH(int n){
  if (n==6){
    putShipH(2);
  }
  if(n==4 || n==5){
    putShipH(3);
  }
  if(n==3){
    putShipH(4);
  }
  if(n==2){
    putShipH(5);
  }
}

//void checkship(x){
  
  

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
      chooseShipH(x);
      x--;
    case 'h':
      chooseShipV(x);
      x--;
      break;
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
  }
  //endwin();
}

int main(){
  startGame();
  placeShips();
  return 0;
}

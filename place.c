#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>



//This version place teh ships
//need to make a check function
//need to simplify the place pieces function
//need to put ship info into a file

int board [10][10]= {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

void addpiece(int y, int x){//adds the piece to board[][]
  int a = y-2;
  int b = x-5;
  a = a/2;
  b = b/4;
  board[a][b] = 1;
  }

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
    case 'e':
      showBoard();
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

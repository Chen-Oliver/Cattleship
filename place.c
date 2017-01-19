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
<<<<<<< HEAD
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
=======
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
>>>>>>> 3c06739eaba38c97b60450460e8a5703a40a6069
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

<<<<<<< HEAD
int checkshipV( int n){
  if(n==6){
    if (cursor.y+2>20){
      return 0;
    }
  }

  if(n==4||5){
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
}

//chooses ship to be placed
void chooseShipV(int n){
  if(n==6){
    printw( "O");
    move(cursor.y+2, cursor.x);
    printw( "O");
  }
  if(n==5 || n==4){
    printw( "O");
    move(cursor.y+2, cursor.x);
    printw( "O");
    move(cursor.y-2, cursor.x);
    printw( "O");
  }

  if(n==3){
    printw( "O");
    move(cursor.y+2, cursor.x);
    printw( "O");
    move(cursor.y-2, cursor.x);
    printw( "O");
    move(cursor.y+4, cursor.x);
    printw( "O");
  }

  if(n==2){
    printw( "O");
    move(cursor.y+2, cursor.x);
    printw( "O");
    move(cursor.y-2, cursor.x);
    printw( "O");
    move(cursor.y+4, cursor.x);
    printw( "O");
    move(cursor.y-4, cursor.x);
    printw( "O");
  }
  move(cursor.y, cursor.x);

}
int checkshipH( int n){
  if(n==6){
    if (cursor.x+4>43){
      return 0;
    }
  }

  if(n==4||5){
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
}


//chooses ship to be placed
void chooseShipH(int n){
  if(n==6){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
  }
  if(n==5 || n==4){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
    move(cursor.y, cursor.x-4);
    printw( "O");
  }
  
  if(n==3){
    printw( "O");
    move(cursor.y, cursor.x+4);
    printw( "O");
    move(cursor.y, cursor.x-4);
    printw( "O");
    move(cursor.y, cursor.x+8);
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
    move(cursor.y, cursor.x-8);
    printw( "O");
=======
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
>>>>>>> 3c06739eaba38c97b60450460e8a5703a40a6069
  }
  move(cursor.y, cursor.x);
}

<<<<<<< HEAD


=======
//void checkship(x){
  
>>>>>>> 3c06739eaba38c97b60450460e8a5703a40a6069
  

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
<<<<<<< HEAD
      if(checkshipH(x)){
	if(x>1){
	  chooseShipH(x);
	  x--;
	}
      }
      break;
    case 'v':
      if(checkshipV(x)){
	if(x>1){
	  chooseShipV(x);
	  x--;
	}
      }
=======
      chooseShipH(x);
      x--;
    case 'h':
      chooseShipV(x);
      x--;
>>>>>>> 3c06739eaba38c97b60450460e8a5703a40a6069
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

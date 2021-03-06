#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "networking.h"
#include <signal.h>
int myboard [10][10]= {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
int oppboard[10][10]= {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

int movesdata[4]={0,0,0,0}; //non shared memory version of ^
char buffer[MESSAGE_BUFFER_SIZE];
int connection;
void statusprint(char *);
void readwrite();
struct coords{ //stores coordinates of on screen cursor(lowest is y=0,x=0)
  int x,y;
}cursor;
WINDOW *status_window;
int winHeight=5;
int winWidth=65;
int winY=23;
int winX=0;

static void sighandler(int signo){
  if(signo ==SIGINT){
    statusprint("Use 'q' to quit during turn");
  }
}

void readwritesetArray(int which){//which == 1read array and set, which == 2 write array
  if (which==1){
    readwrite(0,NULL);
    sscanf(buffer,"%d %d %d %d",&movesdata[0],&movesdata[1],&movesdata[2],&movesdata[3]);
  }
  else if(which == 2){
    char str[100];
    sprintf(str,"%d %d %d %d",movesdata[0],movesdata[1],movesdata[2],movesdata[3]);
    readwrite(1,str);
    }
}
void addpiece(int y, int x){//adds the piece to myboard[][]
  int a = y-2;
  int b = x-5;
  a = a/2;
  b = b/4;
  myboard[a][b] = 1;
  }
void makeMove(int y,int x,int hit){//add moves to oppboard[][], hit =1 means ship was hit, if 0 then miss
  int a = y-2;
  int b = x-5;
  a=a/2;
  b=b/4;
  if(hit==1)oppboard[a][b]=-2;
  if(hit==0)oppboard[a][b]=-1;
}
void cursortoActual(int y,int x){ //converts and sets move coordinates(in shm array) to coordinates they would have on opponent board
  movesdata[1]=y-38;
  movesdata[2]=x;
}
int isValid(int y,int x,int who){ //who = 1,check if your move is valid, who=2 check if opp move valid
      //y and x are the shared mem coordinates(see above function)
  int a = y-2;
  int b = x-5;
  a=a/2;
  b=b/4;
  if(who ==1){
  if(oppboard[a][b]<0)return 0;
  else if(oppboard[a][b]<2)
    return 1;
}
  else if(who == 2){
    if(myboard[a][b]==1){
      myboard[a][b]=-2; //if hit ship set to -2
      statusprint("Your ship was hit! Your turn.");
      readwrite(1,"You hit a ship! Opponent's turn...");
    }
    else if(myboard[a][b]==0){
      myboard[a][b]=-1;//if miss ship set to -1
      statusprint("Opponent missed ship! Your turn.");
      readwrite(1,"You missed! Opponent's turn...");
    }
    move(y,x);
    printw("X");
    move(cursor.y,cursor.x);
    return 1;
}
  return -1;
  }

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
void recreateWin(){
    delwin(status_window);
    status_window=create_newwin(winHeight,winWidth,winY,winX);
    wrefresh(status_window);
}

void statusprint(char *status){
  recreateWin();
  mvwprintw(status_window,2,1,status);
  wrefresh(status_window);
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
  status_window = create_newwin(winHeight,winWidth,winY,winX);
  wrefresh(status_window);
  statusprint("V to place vertically, H to place horizontally");
}
int checkstack(int y,int x){//check board[][] if boats will stack
  int a = y-2;
  int b = x-5;
  a = a/2;
  b = b/4;
  if (myboard[a][b]==1)
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
    default:
      break;
    }
    move(cursor.y,cursor.x);
    refresh();
    if(x==1){
      statusprint("waiting for other player...");
      readwrite(1,"ready");//write ready to opponent
      readwrite(0,NULL); //read from opponenet
      if(strcmp(buffer,"ready")==0){
      recreateWin();
      printboard(38,0,2);
      statusprint("You're going second: arrows to move,'S' to place");
      break;
    }
  }
  //endwin();
}
}
void quitGame(){
  endwin();
  sleep(2);
  kill(getpid(),SIGTERM);
}
//allows player to use arrow keys to move around board(s)
void moveNplace(){
  while(movesdata[0]==1){
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
        cursortoActual(cursor.y,cursor.x);
        if(isValid(movesdata[1],movesdata[2],1)==1){ //check if my move is valid
          movesdata[0]=3;//move made,wait for message
          readwritesetArray(2);
        }
        else{
          statusprint("Invalid move. Try again.");
        }
        break;
      case 'q':
        movesdata[3]=1;
        movesdata[0]=3;//move made,wait for message
        readwritesetArray(2);
        break;
      default:
        break;
      }
      move(cursor.y,cursor.x);
      refresh();
    }
    if(movesdata[0]==0){
    readwritesetArray(1);
    }
    else if(movesdata[0]==2){//after opponent move, before your move
      if(movesdata[3]==1){
        statusprint("Opponent quit, exiting game...");
        sleep(2);
        quitGame();
      }
      else{
      isValid(movesdata[1],movesdata[2],2);
      readwritesetArray(1);
    }
    }
    else if(movesdata[0]==3){
      if(movesdata[3]==1){
        statusprint("You quit");
        sleep(2);
        quitGame();
      }
      else{
        readwrite(0,NULL);
        if(strcmp(buffer,"You hit a ship! Opponent's turn...")==0){
          printw("H");
          refresh();
          makeMove(movesdata[1],movesdata[2],1);
        }
        else if(strcmp(buffer,"You missed! Opponent's turn...")==0){
          printw("M");
          refresh();
          makeMove(movesdata[1],movesdata[2],0);
        }
        statusprint(buffer);
        movesdata[0]=0;//opponent turn
        readwritesetArray(2);
      }
    }
    }
int allHit(int board[10][10]){
  int health = 17;
  int i;
  int j;
  for(i=0;i<10;i++){
    for(j=0;j<10;j++){
      if (board[i][j]==-2)health--;
    }
  }
  if(health==0)return 1;
    return 0;
}
int endGame(){
  if(allHit(myboard)||allHit(oppboard))return 1;
  return 0;
}

int main( int argc, char *argv[] ){
  signal(SIGINT,sighandler);
  int myturns;
  int myhitpct;
  int mymisspct;
  char *host;
  if (argc != 2) {
    printf("host not specified, attempting connection to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  connection = client_connect(host);
  startGame();
  placeShips();
  while(!endGame()){
  moveNplace();
}
  readwrite(0,NULL);
  sscanf(buffer,"%d %d %d",&myturns,&myhitpct,&mymisspct);
  if(allHit(myboard)){
    recreateWin();
    mvwprintw(status_window,2,1,"You lost :(, Turns:%d, Hit Pct: %d, Miss Pct: %d",myturns,myhitpct,mymisspct);
    wrefresh(status_window);
  }
  else{
    recreateWin();
    mvwprintw(status_window,2,1,"You won :D, Turns:%d, Hit Pct: %d, Miss Pct: %d",myturns,myhitpct,mymisspct);
    wrefresh(status_window);
  }

  sleep(7);
  endwin();
  return 0;
}

void readwrite(int readOrWrite,char *message) {
  if(readOrWrite==0){
  read(connection, buffer, sizeof(buffer));
}
  else if(readOrWrite==1){
    strcpy(buffer,message);
    write(connection, buffer, sizeof(buffer));
}
  }

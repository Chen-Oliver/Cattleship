Project title:
    Cattleship (Battleship written in C)
    
Project members:
    Oliver Chen, Dewan Sunnah, Ceming Huang

Compiling the project:
      make all
Libraries used:  ncurses
sudo apt-get install libncurses-dev

Using the project(Instructions):
      1) ./server to run the server (Player 1)
      2) ./client <IP> or ./client if localhost (Player 2)
      3) After the boards appear, each player begin placing ships by pressing V or H on a valid coordinate(Any that is not occupied on the board).
      	 a)V for vertical placement and H for horizontal placement.
	 b)To know if a coordinate is valid, keep these tips in mind:
	      I) The ships being placed are in size order from smallest to largest, starting with size 2, then 3, 3, 4, and 5. A total of 5 ships.
	      II) The system places the ships for the players after they choose the first coordinates.    
	      III) For odd-sized ships, The first coordinate players choose is the middle coordinate of the ship. For even-sized ships, the first coordinate players choose is the one rounded down from the middle. For example, for the size two ship, the first coordinate players choose is the first coordinate of the ship.
       4) A second board will appear as soon as both players place all of their ships down. That board will be where the players choose which coordinates they want to fire at their opponent.
       5) The game then begins. Server will go first, and the client will go second.
       6) The players have to press S to fire a cannon at opponent’s ships. 
       7) If the coordinate players choose to fire at hits the opponent's ship, that coordinate will be marked as H. Otherwise it will be marked as M for missed.
       8) Once all of a player's ships get struck down, the game will end. A message will be sent to each of the players, and the window will close itself in a moment.

Notes while running our project:
       1) Please run the server first, and make sure the client leaves first.
       2) Please do not rage quit, it is just a game.  If players do want to leave the game early, players can press q during their turn(not during ship placement) and both players will quit.
       3) The players can still move the cursor while it is their opponent's turn. Even though it may seem like their cursor is stuck. As a result, the players can buffer multiple inputs (premove). However, players won't be able to move more than once per turn. So multiple inputs will take place in the next turns.
       4) Minor bug that could result in a socket bind error when the server is closed before the client does, was not able to find a solution for it.
       
List of all the files that should be in the repository:
     makefile
     server.c
     client.c
     networking.c
     networking.h


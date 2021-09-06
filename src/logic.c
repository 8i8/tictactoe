/*
 ============================================================================
 Name        : logic.c
 Author      : Iain Hill
 Version     : 1
 Copyright   : GPL

 Description : naughts and crosses / tic tac toe

    Copyright (C) 2017 Iain Hill.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ============================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "ticTacToe.h"

/* The size of the playing board, the 'hash'. */
#define MATRIX	9
#define M_SQRT	3
#define AUGMENT	1
#define VALUE	2
#define REQUIRED_TO_WIN 17

/*
 * Moves that will put two pieces on one line.
 */
static int nextMoves[2][3][3];

/*
 * The game board.
 */
static int board[3][3];

/*
 * The first index, 0 contains the best overall state available to the
 * player or otherwise said, the status.
 * Following this is the state of each square in relation to its
 * neighbours, accounting for each horizontal, vertical and diagonal
 * direction. This data is used to evaluate the next move.
 *
 * indices 2,3,4 horizontal rows.
 * indices 5,6,7 vertical columns.
 * indices 8, 9 the two diagonals.
 *
 */
static int currentStateOfPlay[2][9];

/*
 * The scorboard.
 */
static int score[2];

/*
 * The number of moves made.
 */
static int movesMade;

/*
 * winingLine is the line that is to be played.
 */
static int winingLine;

/*
 * A temporary measure of nothingness.
 */
static int level;
static int myRandom;

/*
 * ACME debugging tool for ticTacToe. Is this perhaps the embryo of a
 * unit test?  See the header file, to activate -> DEBUG 1
 */
void printDebugMoves()
{
	/* Above the debug squares */
	if (myRandom)
		puts("Random mode on.");
	else
		puts("Random mode off.");
	printf("Level= %d\n", level);
	printf("Moves made = %d\n", keepCount(VALUE));
	printf("Player 1 status -> %d\n", currentStateOfPlay[PLAYER1][0]);
	printf("Player 2 status -> %d\n", currentStateOfPlay[PLAYER2][0]);
	printf("Player 1 score -> %d\n", score[PLAYER1]);
	printf("Player 2 score -> %d\n", score[PLAYER2]);
	/* The four matrices */
	printf("   board  p1nextM p2nextM\n");
	for (int i = 0; i < M_SQRT; i++) {
		/* board */
		printf(" |");
		for (int j = 0; j < M_SQRT; j++) {
			printf("%2d", board[i][j]);
		}

		/* Player one nextMoves*/
		printf(" |");
		for (int j = 0; j < M_SQRT; j++) {
			printf("%2d", nextMoves[PLAYER1][i][j]);
		}

		/* Player two nextMoves */
		printf(" |");
		for (int j = 0; j < M_SQRT; j++) {
			printf("%2d", nextMoves[PLAYER2][i][j]);
		}
		printf(" |");
		puts("");
	}

	/* Beneath the two matrices */
	printf("  ------- ------- -------\n");

        /* state of play */
        printf("p1{%d}(%d,%d,%d}{%d}{%d,%d,%d}{%d}\n", 
                        currentStateOfPlay[PLAYER1][0],
                        currentStateOfPlay[PLAYER1][1],
                        currentStateOfPlay[PLAYER1][2],
                        currentStateOfPlay[PLAYER1][3],
                        currentStateOfPlay[PLAYER1][4],
                        currentStateOfPlay[PLAYER1][5],
                        currentStateOfPlay[PLAYER1][6],
                        currentStateOfPlay[PLAYER1][7],
                        currentStateOfPlay[PLAYER1][8]);

        printf("p2{%d}(%d,%d,%d}{%d}{%d,%d,%d}{%d}\n", 
                        currentStateOfPlay[PLAYER2][0],
                        currentStateOfPlay[PLAYER2][1],
                        currentStateOfPlay[PLAYER2][2],
                        currentStateOfPlay[PLAYER2][3],
                        currentStateOfPlay[PLAYER2][4],
                        currentStateOfPlay[PLAYER2][5],
                        currentStateOfPlay[PLAYER2][6],
                        currentStateOfPlay[PLAYER2][7],
                        currentStateOfPlay[PLAYER2][8]);
	puts("\n");
}

/*
 * keepCount is used to inform the computer how many moves remain, this
 * function has several uses, each defined by the integer value entered.
 *
 * 0 -> return count
 * 1 -> count
 * 2 -> reset
 */
int keepCount(int option)
{
        switch (option) {
        case RESET:
		movesMade = 0;
		return 0;
        case AUGMENT:
		movesMade++;
		return movesMade;
        case VALUE:
		return movesMade;
        }
	return -1;
}

/*
 * The score for the score bars, triggers the end of a series of games
 * when a player reaches or surpasses the value of REQUIRED_TO_WIN.
 */
int keepScore(int player, int inc)
{
	if (inc == RESET) {
		score[player-1] = 0;
	}
        score[player-1] += inc;
        if (score[player-1] >= REQUIRED_TO_WIN) {
                return 1;
        }
        if (score[player-1] < 0) {
                score[player-1] = 0;
        }

	drawScore(score[0], score[1]);

        return 0;
}

/*
 * Resets the moves array to zero and then calls the function that
 * writes the new status quo to the grid.
 */
void resetBoard()
{
	for (int i = 0; i < MATRIX; i++)
		*(*board+i) = 0;
	writeMovesToBoard(*board);

	if (myRandom)
		level = coinToss(5)+1;

	redrawGrid(winingLine);
	winingLine = 0;
}

/*
 * Set the difficulty level.
 */
int setLevel(int newLevel)
{
	if (newLevel == 48) {
		myRandom = 1;
		return 1;
	}
	if (newLevel > 48 && newLevel < 54) {
		myRandom = 0;
		level = newLevel - 48;
		return 1;
	}
	return 0;
}

/*
 * Transfers the command from main.c, to draw the winning line, here it
 * simply picks up the line reference.
 */
void winningLine()
{
	drawWinningLine(winingLine);
}

/*
 * Translates the char values entered by the user into the integer
 * values that correspond to them, and then writes those values to the
 * board array.
 */
int traslateCharForMove(int x, int y, int player)
{
	if 	(x == 97) // a
		x = 0;
	else if (x == 98) // b
		x = 1;
	else if (x == 99) // c
		x = 2;

	if	(y == 49) // 1
		y = 0;
	else if (y == 50) // 2
		y = 1;
	else if (y == 51) // 3
		y = 2;

	// If the square is already in use return 0
	if (board[y][x] != EMPTY)
		return 0;

        // Place the move on the board.
        board[y][x] = player;
        return 1;
}

/*
 * updateGame refreshes the graphical display, the int entered
 * represents the player else a reset.
 */
int updateGame(int player)
{
	int status;

	if (player == PLAYER1 || player == PLAYER2) {
		status = updateStateOfPlay(player);
		writeMovesToBoard(*board);
	} else if (player == RESET) {
		clearStatusArrays();
		resetBoard();
	}

	drawScore(score[0], score[1]);
	drawGrid();

	return status;
}

/*
 * Ask player to input either heads or tails, an ancient form of
 * divination used by tribes that depend upon counting tokens as their
 * major deities.  This rite predicts the nature of the act that follow
 * in an uncannily precise manor.
 */
int headsOrTails()
{
	for(;;)
	{
		int c;
		drawGrid();
		sysOut(4, PLAYER1);

		while ((c = getchar()) != '\n')
		{
			if (c == 'h')
				return 0;
			else if (c == 't')
				return 1;
			else
				printf("h or t ?");
		}
	}
	return 1;
}

/*
 * Returns a n / n result, a virtual coin toss if you will humour me
 * that, about as random as a dice in space ...
 */
int coinToss(int howManySides)
{
	int coin;
	srand(time(NULL));
	coin = rand()%howManySides;
	return coin;
}

/*
 * The Computer or player two starts the game.
 */
void player2WinsCoinToss(int player)
{
	drawGrid();
	sysOut(5, player);
	usleep(1000);
	computerMove(player);
	updateGame(player);
}

/*
 * Player one starts the game, this is simply the text output.
 */
void player1WinsCoinToss(int player)
{
	drawGrid();
	sysOut(5, player);
	usleep(1000);
}

/*
 * Request move from player, x can be either 1, 2 or 3 and y a, b or c.
 */
int yourMove(int player)
{
	// If none can move, get out of here.
	if (checkStaleMate())
		return STALE_MATE;

	// Ok play.
	int status;
	status = updateGame(player);
	currentStateOfPlay[player-1][0] = status;
	sysOut(7, player);

	int x = 0;
	int y = 0;
	int i = 0;
	int c;

	int a;
	int b;

	// Do not accept '\n' until two char are entered.
	while ((c = getchar()) != '\n') {
		if (i == 0 && c != '\n')
			x = c;
		if (i == 1 && c != '\n')
			y = c;
		i++;
	}

	// Inverse the coordinates if reasonable values.
	if ((x > 48 && x < 52) && (y > 96 && y < 100))
	{
		a = x;
		b = y;
		x = b;
		y = a;
	}
	// Repeat until reasonable values are entered ...
	else if ((x < 97 || x > 99) || (y < 49 || y > 51))
	{
		yourMove(player);
		return 0;
	}

	// Write to the board array.
	if (!traslateCharForMove(x, y, player)) {
		yourMove(player);
		return 0;
	}

	keepCount(AUGMENT);
	
	status = updateGame(player);
	currentStateOfPlay[player-1][0] = status;
	return status;
}

/*
 * The twisted logic of my first ever AI, call me 'Frankenstein' if you
 * will but I shall have the last laugh yet. [Evil laughter ensues,
 * scene fades to black]
 */
int computerMove(int player)
{
	// If none can move, get out of here.
	if (checkStaleMate())
		return STALE_MATE;

	// Ok play.
	int status;
	int coin;
	status = updateGame(player);
	currentStateOfPlay[player-1][0] = status;
	sysOut(7, player);
	sleep(2);

	switch (level)
	{
		case 1: randomMove(player);
			break;
		case 2: coin = coinToss(3);
			if (coin) {
				randomMove(player);
			} else {
				bestPossibleMove(player);
			}
			break;
		case 3: coin = coinToss(2);
			if (coin) {
				randomMove(player);
			} else {
				bestPossibleMove(player);
			}
			break;
		case 4: coin = coinToss(3);
			if (coin) {
				bestPossibleMove(player);
			} else {
				randomMove(player);
			}
			break;
		case 5: bestPossibleMove(player);
			break;
		default:
                        printf("error: unknown level in computerMove");
			break;
	}

        keepCount(AUGMENT);
	status = updateGame(player);
	currentStateOfPlay[player-1][0] = status;
	return status;
}

/*
 * A totally random move using no logic other than, how many spaces
 * remain after n moves, the square is chosen on a 1..n random basis
 * where n is the number of empty squares remaining.
 */
int randomMove(int player)
{
	/*
	 * Generate randomly the choice of the next move.
	 */
	int movesLeft;
	int choice;
	srand(time(NULL));

	// Get the quantity of spaces left remaining.
	movesLeft = MATRIX - keepCount(VALUE);

	if (movesLeft > 1) {
		choice = rand()%(movesLeft-1) + 1;
	} else {
		choice = 1;
	}

	/*
	 * Count empty squares until move selection is reached.
	 */
	int count = 1;
	for (int i = 0; i < M_SQRT; i++) {
		for (int j = 0; j < M_SQRT; j++)
		{
			if ( board[i][j] == EMPTY )
			{
				if (count == choice) {
					board[i][j] = player;
					return 0;
				}
				count++;
			}
		}
	}
        printf("error: randomMove failed to make move keepCount returned %d", movesLeft);
        exit(2);
	return 0;
}

/*
 * For now this is the best possible move for the computer intelligence,
 * I am aware that there is another 'to win' condition that requires
 * programming.  For now it is sufficient, I will add this at a later
 * date.
 *
 * TODO add knowledge of the double winning move trap, to be chose if
 * the occasion arises.
 */
int bestPossibleMove(int player)
{
	int opponent;
	int value;
	int coin;
	int count;

        if (player == PLAYER1) {
                opponent = PLAYER2;
        } else {
                opponent = PLAYER1;
        }


	/*
         * If the center square is empty and there is no winning move,
         * move there.
	 */
	if (board[1][1] == EMPTY && currentStateOfPlay[player-1][0] != 3) {
		board[1][1] = player;
		return 0;
	}

	/*
	 * If there is a winning move, take it.
	 */
	if (currentStateOfPlay[player-1][0] == 3) {
		for (int i = 1; i <= 2*M_SQRT+2; i++) {
			value = currentStateOfPlay[player-1][i];
			if (value == 3 || value == 5 || value == 6 ) {
				calculateNextMove(value, i, player);
				return 4;
			}
		}
	}

        /*
	 * If opponent has a winning move, block them.
         */
	if (currentStateOfPlay[opponent-1][0] == 3) {
		for (int i = 1; i <= 2*M_SQRT+2; i++) {
			value = currentStateOfPlay[opponent-1][i];
			if (value == 3 || value == 5 || value == 6 ) {
				calculateNextMove(value, i, opponent);
				return 0;
			}
		}
	}

	clearNextMoves();

	// Evaluate next best moves.
	count = 0;
        // Fill the nextMoves grid with the available best moves of each
        // player.
	if (currentStateOfPlay[player-1][0] == 2) {
		for (int i = 1; i <= 2*M_SQRT+2; i++) {
			value = currentStateOfPlay[player-1][i];
			if (value == 1 || value == 2 || value == 4 ) {
				calculateNextMove(value, i, player);
			}
			value = currentStateOfPlay[opponent-1][i];
			if (value == 1 || value == 2 || value == 4 ) {
				calculateNextMove(value, i, opponent);
			}
		}
		if (DEBUG)
			updateGame(player);

		/*
                 * Count the number of available places, that are shared
                 * between the two, your move will also block and
                 * opponents move.
		 */
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (nextMoves[player-1][i][j] && nextMoves[opponent-1][i][j]) {
					count++;
				}
			}
		}

		// Make your move.
		if (count > 0) {

			// Pick a random move from within that scope.
			if (count > 1)
				coin = coinToss(count);
			else
				coin = 1;

			count = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (nextMoves[player-1][i][j] && nextMoves[opponent-1][i][j]) {
						if (coin == count) {
							board[i][j] = player + 1;
							return 0;
						}
						count++;
					}
				}
			}
		}

		/*
                 * So there were no shared choices, do the same for your
                 * own best possible moves.
		 * First count them ...
		 */
		count = 0;
		coin = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (nextMoves[player-1][i][j]) {
					count++;
				}
			}
		}

		// and move there.
		if (count < 0) {

			// Choose one.
			if (count > 1)
				coin = coinToss(count);
			else
				coin = 1;
			count = 0;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (nextMoves[player-1][i][j]) {
						if (count == coin) {
							board[i][j] = player;
							return 0;
						}
						count++;
					}
				}
			}
		}
	}

	// Otherwise just make a random move.
	randomMove(player);
	return 0;
}

void clearNextMoves()
{
	for (int i = 0; i < 18; i++)
		*(**nextMoves+i) = 0;
}

/*
 * Check to see if anyone can move.
 */
int checkStaleMate()
{
	if (MATRIX - keepCount(VALUE) == 0) {
		return 1;
	}
	else
		return 0;
}

/*
 * Erases the arrays which store each players game status; Called
 * between games.
 */
void clearStatusArrays()
{
	for (int i = 0; i < 2*M_SQRT+3; i++) {
		*(*currentStateOfPlay+i) = 0;
	}
}

/*
 * Fills the status arrays which are then in turn used to calculate the
 * AI's next move, first scanning the status of each horizontal row,
 * then the vertical column and finally the two diagonals.
 */
int updateStateOfPlay(int player)
{
	int state;
        int opponent;
	int max; // The best found state.
	int i;
	int j;
	int x;
	state = max = x = 0;

        if (player == PLAYER1) {
                opponent = PLAYER2;
        } else {
                opponent = PLAYER1;
        }

	// Horizontal status
	for (i = 0; i < M_SQRT; i++) {
		for (j = 0; j < M_SQRT; j++)
		{
                        // If the player is here, mark the square.
			if (board[i][j] == player) {
				x = setBit(j, x);
			}
                        // if the other player is here, mark the row as
                        // a loss.
			else if (board[i][j] == opponent) {
				x = 8;
				break;
			}
		}
                // Write the first 3 states, into indices 1 to 3.
		currentStateOfPlay[player-1][i+1] = x;
		state = getStatusValue(x);
		if (x == 7) {
                        winingLine = i+1;
                }
		if (state > max) {
			max = state;
		}
		x = 0;
	}

	// Vertical status
	for (j = 0; j < M_SQRT; j++) {
		for (i = 0; i < M_SQRT; i++)
		{
                        // If the player is here, mark the square.
			if (board[i][j] == player) {
				x = setBit(j, x);
			}
                        // if the other player is here, mark the row as
                        // a loss.
			else if (board[i][j] == opponent) {
				x = 8;
				break;
			}
		}
                // carry on from indices 5 to 7 with further states.
		currentStateOfPlay[player-1][j+5] = x;
		state = getStatusValue(x);
		if (x == 7) {
                        winingLine = j+5;
                }
		if (state > max) {
			max = state;
		}
		x = 0;
	}

	// Diagonal one
	for (i = 0; i < 3; i++)
	{
                // If the player is here, mark the square.
		if (board[i][i] == player) {
			x = setBit(i, x);
		}
                // if the other player is here, mark the row as
                // a loss.
		else if (board[i][j] == opponent) {
			x = 8;
			break;
		}
	}
        // Add the first diagonal, at index 4.
	currentStateOfPlay[player-1][4] = x;
        state = getStatusValue(x);
	if (x == 7) {
                winingLine = 4;
        }
	if (state > max) {
                max = state;
        }

	x = 0;
	j = 0;

	// Diagonal two
	for (i = 2; i >= 0; i--)
	{
                // If the player is here, mark the square.
		if (board[i][j] == player) {
			x = setBit(i, x);
		}
                // if the other player is here, mark the row as
                // a loss.
		else if (board[i][j] == opponent) {
			x = 8;
			break;
		}
		j++;
	}
        // Add the second diagonal, at index 8.
	currentStateOfPlay[player-1][8] = x;
        state = getStatusValue(x);
	if (x == 7) {
                winingLine = 8;
        }
	if (state > max) {
                max = state;
        }
	x = 0;
	return max;
}

/*
 * Returns the bit mask required to describe the state of the row.
 */
int setBit(int bit, int row)
{
	/*
	 * Use 3 bit binary word used to add the move to the rows state.
         *
         *   012
         *   ---
	 *   001 -> 1
	 *   010 -> 2
	 *   100 -> 4
         *
	 */
	switch(bit)
	{
		case 2:
                        row &= 1;
			return row;
		case 1:
                        row &= 2;
			return row;
		case 0:
                        row &= 4;
			return row;
	}
	return 0;
}

/*
 * The final count of each rows logic is given here, this value allows
 * the computer to understand the state of each row and is used in the
 * computation of the computers next move, when the difficulty is set to
 * the higher or the intermediate value.
 */
int getStatusValue(int x)
{
	// Act upon states 0 through 7
	switch(x)
	{
		case 0:
			// Anyone's
			return 1;
		case 1:
			// Good move
			return 2;
		case 2:
			// Good move
			return 2;
		case 3:
			// Move to win
			return 3;
		case 4:
			// Good move
			return 2;
		case 5:
			// Move to win
			return 3;
		case 6:
			// Move to win
			return 3;
		case 7:
			// Player wins!
			return 4;
		case 8:
			// Opponent present.
			return 0;
		default:
			return 0;
	}
}

int calculateNextMove(int state, int line, int player)
{
	/*
	 * Act upon states 0 through 7
	 * 0 -> 000
	 * 1 -> 001
	 * 2 -> 010
	 * 3 -> 011
	 * 4 -> 100
	 * 5 -> 101
	 * 6 -> 110
	 * 7 -> 111
	 */
	switch(state)
	{
		case 0:
			// Anyone's
			// 000
			break;
		case 1:
			// Good move
			// 001 -> 1 XOO
			if 	(line == 1) nextMoves[player-1][0][0] = player; // XOO 000 000
			else if (line == 2) nextMoves[player-1][1][0] = player; // 000 XOO 000
			else if (line == 3) nextMoves[player-1][2][0] = player; // 000 000 XOO
			/* */
			else if (line == 4) nextMoves[player-1][0][0] = player; // X00
										// 0O0
										// 00O
			/* */
			else if (line == 5) nextMoves[player-1][0][0] = player; // 00X 0X0 X00
			else if (line == 6) nextMoves[player-1][0][1] = player; // 00O 0O0 O00
			else if (line == 7) nextMoves[player-1][0][2] = player; // 00O 0O0 O00
			/* */
			else if (line == 8) nextMoves[player-1][0][2] = player; // 00X
										// 0O0
										// O00
			// 001 -> 2 OXO
			if 	(line == 1) nextMoves[player-1][0][1] = player; // OXO 000 000
			else if (line == 2) nextMoves[player-1][1][1] = player; // 000 OXO 000
			else if (line == 3) nextMoves[player-1][2][1] = player; // 000 000 OXO
			/* */
			else if (line == 4) nextMoves[player-1][1][1] = player; // O00
			                                                        // 0X0
										// 00O
			/* */
			else if (line == 5) nextMoves[player-1][1][0] = player;   // O00 0O0 00O
			else if (line == 6) nextMoves[player-1][1][1] = player;   // X00 0X0 00X
			else if (line == 7) nextMoves[player-1][1][2] = player;   // O00 0O0 00O
			/* */
			else if (line == 8) nextMoves[player-1][1][1] = player;   // 00O
                                                                                  // 0X0
			                                                          // O00
			break;
		case 2:
			// Good move
			// 010 -> 1 XOO
			if 	(line == 1) nextMoves[player-1][0][0] = player;   // XOO 000 000
			else if (line == 2) nextMoves[player-1][1][0] = player;   // 000 XOO 000
			else if (line == 3) nextMoves[player-1][2][0] = player;   // 000 000 XOO
			/* */
			else if (line == 4) nextMoves[player-1][0][0] = player;   // X00
							        		  // 0O0
							        		  // 00O
			/* */
			else if (line == 5) nextMoves[player-1][0][0] = player;   // X00 0X0 00X
			else if (line == 6) nextMoves[player-1][0][1] = player;   // O00 0O0 00O
			else if (line == 7) nextMoves[player-1][0][2] = player;   // O00 0O0 00O
			/* */
			else if (line == 8) nextMoves[player-1][0][2] = player;   // 00X
							        		  // 0O0
							        		  // O00
			// 010 -> 2 OOX
			if 	(line == 1) nextMoves[player-1][0][2] = player;   // OOX 000 000
			else if (line == 2) nextMoves[player-1][1][2] = player;   // 000 OOX 000
			else if (line == 3) nextMoves[player-1][2][2] = player;   // 000 000 OOX
			/* */
			else if (line == 4) nextMoves[player-1][2][2] = player;   // O00
                                                                                  // 0O0
                                                                                  // 00X
			/* */
			else if (line == 5) nextMoves[player-1][2][0] = player;   // O00 0O0 00O
			else if (line == 6) nextMoves[player-1][2][1] = player;   // O00 0O0 00O
			else if (line == 7) nextMoves[player-1][2][2] = player;   // X00 0X0 00X
			/* */
			else if (line == 8) nextMoves[player-1][2][0] = player;   // 00O
                                                                                  // 0O0
			                                                          // X00
			break;
		case 3:
			// Move to win
			// 011 -> 0 X00
			if 	(line == 1) board[0][0] = player; 		  // XOO 000 000
			else if (line == 2) board[1][0] = player;                 // 000 XOO 000
			else if (line == 3) board[2][0] = player;                 // 000 000 XOO
			/* */
			else if (line == 4) board[0][0] = player;                 // X00
                                                                                  // 0O0
                                                                                  // 00O
			/* */
			else if (line == 5) board[0][0] = player;                 // X00 0X0 00X
			else if (line == 6) board[0][1] = player;                 // O00 0O0 00O
			else if (line == 7) board[0][2] = player;                 // O00 0O0 00O
			/* */
			else if (line == 8) board[0][2] = player;                 // 00X
                                                                                  // 0O0
                                                                                  // O00
			break;
		case 4:
			// Good move
			// 100 -> 1 OXO
			if 	(line == 1) nextMoves[player-1][0][1] = player;   // OXO 000 000
			else if (line == 2) nextMoves[player-1][1][1] = player;   // 000 OXO 000
			else if (line == 3) nextMoves[player-1][2][1] = player;   // 000 000 OXO
			/* */
			else if (line == 4) nextMoves[player-1][1][1] = player;   // O00
                                                                                  // 0X0
                                                                                  // 00O
			/* */
			else if (line == 5) nextMoves[player-1][1][0] = player;   // O00 0O0 00O
			else if (line == 6) nextMoves[player-1][1][1] = player;   // X00 0X0 00X
			else if (line == 7) nextMoves[player-1][1][2] = player;   // O00 0O0 00O
			/* */
			else if (line == 8) nextMoves[player-1][1][1] = player;   // 00O
                                                                                  // 0X0
                                                                                  // O00
			// 100 -> 2 OOX
			if 	(line == 1) nextMoves[player-1][0][2] = player;   // OOX 000 000
			else if (line == 2) nextMoves[player-1][1][2] = player;   // 000 OOX 000
			else if (line == 3) nextMoves[player-1][2][2] = player;   // 000 000 OOX

                                                                                  // O00
			/* */                                                     // 0O0
			else if (line == 4) nextMoves[player-1][2][2] = player;   // 00X
			/* */
			else if (line == 5) nextMoves[player-1][2][0] = player;   // O00 0O0 00O
			else if (line == 6) nextMoves[player-1][2][1] = player;   // O00 0O0 00O
			else if (line == 7) nextMoves[player-1][2][2] = player;   // X00 0X0 00X
			/* */
			else if (line == 8) nextMoves[player-1][2][0] = player;   // 00O
                                                                                  // 0O0
			                                                          // X00
			break;
		case 5:
			// Move to win
			// 101 -> 0 OXO
			if 	(line == 1) board[0][1] = player; 		  // OXO 000 000
			else if (line == 2) board[1][1] = player;                 // 000 OXO 000
			else if (line == 3) board[2][1] = player;                 // 000 000 OXO
			/* */
			else if (line == 4) board[1][1] = player;                 // O00
                                                                                  // 0X0
                                                                                  // 00O
			/* */
			else if (line == 5) board[1][0] = player;                 // O00 0O0 00O
			else if (line == 6) board[1][1] = player;                 // X00 0X0 00X
			else if (line == 7) board[1][2] = player;                 // O00 0O0 00O
			/* */
			else if (line == 8) board[1][1] = player;                 // 00O
                                                                                  // 0X0
                                                                                  // O00
			break;
		case 6:
			// Move to win
			// 110 -> 0 OOX
			if 	(line == 1) board[0][2] = player;		  // OOX 000 000
			else if (line == 2) board[1][2] = player;                 // 000 OOX 000
			else if (line == 3) board[2][2] = player;                 // 000 000 OOX
			/* */
			else if (line == 4) board[2][2] = player;                 // O00
                                                                                  // 0O0
                                                                                  // 00X
			/* */
			else if (line == 5) board[2][0] = player;                 // O00 0O0 00O
			else if (line == 6) board[2][1] = player;                 // O00 0O0 00O
			else if (line == 7) board[2][2] = player;                 // X00 0X0 00X
			/* */
			else if (line == 8) board[2][0] = player;                 // 00O
                                                                                  // 0O0
                                                                                  // X00
			break;
		case 7:
			// Player wins!
			// 111
			break;
		case 8:
			// Opponent present.
			break;
		default:
			break;

	}
	return 0;
}


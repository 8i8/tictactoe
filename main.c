/*
 ============================================================================
 Name        : ticTacToe.c
 Author      : Iain Hill
 Version     : 1
 Copyright   : GPL
 Description : naughts and crosses / tic tac toe
 ============================================================================
 */

/* TODO 
 *  - Develop some form of instructions page,
 *  - Give access to settings.
 *  - Add better levels, ie weighted probability on all levels as the random
 *  game is a much better game than any of the set levels.
 *  - Add a way for players to quit whilst the game is running.
 *  - Add line thrpugh graphic at the end of the game.
 *  - automtic game end when stalemate is inevitable.
 */

#include "ticTacToe.h"
#include <unistd.h>

#define RESET	0

#define WIN		4
#define STALE_MATE	5
#define UN_SPUN		3

#define RESET	0
#define INC	1
#define MINUS2	2

int main(void)
{
/*
 * Value 1 represents the initial condition, any subsequent calls to play() can
 * be made with other values, to enable the storing of variables between games
 * and also to permit the launching of different commands upon the games first
 * launch.
 */
	menu();
	play(1);
	return EXIT_SUCCESS;
}

void menu()
{
	int c = 0;
	int verif = 0;

	clearScreen();
	ticTacToe();
	sysOutMenu();
	
	while (!verif)
	{
		c = getchar();
		verif = setLevel(c);
	}
	resetMoves();
}

/*
 * Oh dear, what a mess this function turned into, hmm ... One of those
 * 'things to do' that you keep ignoring, the tangled mess that is the
 * beginning of this program, bears witness to code which has yet to come; that
 * is my excuse, if you are buying.
 */
void play(int firstRun)
{
	int choice;
	int coin;
	int winner;
	int status;
	int stale_mate = 0;
	winner = 0;
	coin = 3;

	while(!winner)
	{
		while (firstRun)
		{
/*
 * updateGame refreshes the graphical display, the int entered represents the
 * player, 1 for player 1 and 2 for player 2, 0 initiates a total reset.
 */
			updateGame(RESET);
/*
 * keepCount is used to inform the computer how many moves remain, this function
 * has several uses, each defined by the interger value entered.
 *
 * 0 -> return count
 * 1 -> count
 * 2 -> reset
 */
			keepCount(RESET);

			firstRun = 0;
			if (coin == UN_SPUN) {
				choice = headsOrTails();
			}
			coin = coinToss(2);

			if (choice != coin) 
			{
				player2WinsToss(PLAYER2);
				break;
			}

			playet1WinsTheToss(PLAYER1);

			if (coin)
				coin = 0;
			else 
				coin = 1;
		}

		status = yourMove(PLAYER1);
		if(status == WIN) {
			updateGame(WIN);
/*
 * This function is found at the end of draw.c, it is simply a text output to
 * screen, the first int defines the situation and the second the player, 0
 * being anyone and 1 and 2, player one and two consecutivly.
 */
			// You win.
			sysOut(0, 0);
			keepScore(PLAYER1, INC);
			keepScore(PLAYER2, RESET);
			winner = 1;
		} else
			if (status == STALE_MATE)
				stale_mate = 1;

		if(!winner && !stale_mate)
			status = computerMove(PLAYER2);
		stale_mate = 0;

		if(status == WIN && !winner) {
			updateGame(WIN);
			// I win.
			sysOut(1, 0);
			keepScore(PLAYER2, INC);
			keepScore(PLAYER1, RESET);
			winner = 1;
		}

		if (status == STALE_MATE) {
			updateGame(PLAYER1);
			// Stale-mate
			sysOut(2, 0);
			keepScore(PLAYER1, MINUS2);
			keepScore(PLAYER2, MINUS2);
			winner = 1;
		}


		if(winner)
		{
			sleep(3);
			winner = 0;
			firstRun = 1;
			drawGrid(0);
		}
	}
}


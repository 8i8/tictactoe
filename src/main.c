/*
 ============================================================================
 Name        : main.c
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

/* TODO 
 *  - Develop some form of instructions page,
 *  - Give access to settings.
 *  - Add a way for players to quit whilst the game is running.
 *  - automatic game end when stalemate is inevitable.
 */

#include "ticTacToe.h"
#include <unistd.h>

#define WIN		4
#define STALE_MATE	5
#define UN_SPUN		3

#define RESET	        0
#define INCREMENT	4
#define DECREMENT	-4

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
	int verif = 0;

	clearScreen();
	ticTacToe();
	sysOutMenu();
	
	while (!verif)
	{
		getchar();
		verif = setLevel(48);
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

        // Game loop.
	while(!winner)
	{
                // Setup.
                while (firstRun)
                {
                        updateGame(RESET);
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

                        player1WinsTheToss(PLAYER1);

                        if (coin)
                                coin = 0;
                        else 
                                coin = 1;
                }

                // Player input.
		status = yourMove(PLAYER1);

                // Player win.
		if(status == WIN) {
                        // Print status
			sysOut(0, 0);
                        if (keepScore(PLAYER1, INCREMENT) > 0) {
                                winner = 1;
                                break;
                        }
			keepScore(PLAYER2, RESET);
			winner = 1;
		} else if (status == STALE_MATE)
			stale_mate = 1;

                // Computers move.
		if(!winner && !stale_mate)
			status = computerMove(PLAYER2);

		stale_mate = 0;

                // Computer wins.
		if(status == WIN && !winner) {
			sysOut(1, 0);
                        if (keepScore(PLAYER2, INCREMENT) > 0) {
                                winner = 2;
                                break;
                        }
			keepScore(PLAYER1, RESET);
			winner = 2;
		} else if (status == STALE_MATE) {
			updateGame(PLAYER1);
			// Stale-mate
			sysOut(2, 0);
			keepScore(PLAYER1, DECREMENT);
			keepScore(PLAYER2, DECREMENT);
			winner = 1;
		}

                // Game end.
		if(winner) {
			winner = 0;
			firstRun = 1;
			winningLine();
			sleep(2);
		}
	}

        // End of the game.
        if (winner == 1) {
                winningLine();
                sleep(2);
                playEnding();
        }
}


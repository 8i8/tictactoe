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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ticTacToe.h"
#include "logic.h"
#include "draw.h"

void menu(void);
void play(void);

int DEBUG = 0;

int main(void)
{
/*
 * Value 1 represents the initial condition, any subsequent calls to
 * play() can be made with other values, to enable the storing of
 * variables between games and also to permit the launching of different
 * commands upon the games first launch.
 */
	menu();
	play();
	return EXIT_SUCCESS;
}

void menu()
{
	srand(time(NULL));
        setLevel('0');
	clearScreen();
	ticTacToe();
	sysOutMenu();
        getchar();
}

/*
 * Oh dear, what a mess this function turned into, hmm ... One of those
 * 'things to do' that you keep ignoring, the tangled mess that is the
 * beginning of this program, bears witness to code which has yet to
 * come; that is my excuse, if you are buying.
 */
void play()
{
	int choice, status, winner, stale_mate;
        choice = status = winner = stale_mate = 0;
	int coin = 3;
        int firstRun = 1;

        // Game loop.
	while(!winner)
	{
                // Setup.
                while (firstRun)
                {
                        firstRun = 0;
                        keepCount(RESET);
                        resetBoard();

                        if (coin == COIN_UN_SPUN) {
                                choice = headsOrTails();
                        }
                        coin = coinToss(2);

                        if (choice != coin)
                        {
                                // Set up board and play a move.
                                player2WinsCoinToss(PLAYER2);
                                break;
                        }

                        // Set up board and then continue.
                        player1WinsCoinToss();
                }

                if (DEBUG)
                        logPlayerStates();

                // Player input.
		status = yourMove(PLAYER1);

                // Player win.
		if(status == PLAYER1_WIN) {
			winner = PLAYER1;
                        if (keepScore(PLAYER1, INCREMENT) > 0) {
                                break;
                        }
			keepScore(PLAYER2, RESET);
                        winningLine();
                        sysOut(PLAYER_WIN);
                        fflush(stdout);
		} else if (status == STALE_MATE)
			stale_mate = 1;

                // Computers move.
		if(!winner && !stale_mate)
			status = computerMove(PLAYER2);

		stale_mate = 0;

                // Computer wins.
		if(status == PLAYER1_WIN && !winner) {
                        winner = PLAYER2;
                        if (keepScore(PLAYER2, INCREMENT) > 0) {
                                break;
                        }
			keepScore(PLAYER1, RESET);
                        winningLine();
                        sysOut(OPONENT_WIN);
                        fflush(stdout);
		} else if (status == STALE_MATE) {
			keepScore(PLAYER1, DECREMENT);
			keepScore(PLAYER2, DECREMENT);
			updateGame(PLAYER1);
			sysOut(PLAYER_DRAW);
                        fflush(stdout);
                        sleep(2);
                        winner = 1;
		}

                // Game end.
		if(winner) {
                        sleep(2);
			firstRun = 1;
			winner = 0;
		}
	}

        // End of the game.
        if (winner == 1) {
                sysOut(SET_WIN);
                sleep(2);
                playEnding();
        }
}


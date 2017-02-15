/*
 ============================================================================
 Name        : ticTacToe.c
 Author      : Iain Hill
 Version     : 1
 Copyright   : GPL
 Description : naughts and crosses / tic tac toe
 ============================================================================
 */
#include "ticTacToe.h"

#define PLAYER 	 	1
#define COMPUTER	-1

int main(void)
{
	start();
	play();
}

void play()
{
	int winner = 0;

	while(!winner)
	{
		drawGrid(PLAYER);
		/* */
		player1Move(PLAYER);
		drawGrid(COMPUTER);
		/* */
		player2Move(COMPUTER);
	}
}

void player1Move(int player)
{
	printf("%d", yourMove(player));
}

void player2Move(int player)
{
	printf("%d", computerMove(player));
}

void start()
{
	int choice;
	int coin;
	int player = COMPUTER;

	drawGrid(player);
	choice = headsOrTails();
	coin = coinToss();

	if (choice != coin) 
	{
		computerWinsToss(player);
	}
	play();
}

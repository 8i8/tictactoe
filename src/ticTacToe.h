/*
 ============================================================================
 Name        : tickTacToe.h
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG			1
#define EMPTY			0
#define PLAYER1			1
#define PLAYER2			2
#define RESET 			0
#define COIN_UN_SPUN	3
#define PLAYER1_WIN		4
#define STALE_MATE		5

#define INCREMENT	4
#define DECREMENT	-4

// ticTacToe.c
void menu(void);
void play(int firstRun);

//logic.c
int  keepCount(int option);
int keepScore(int player, int inc);
void resetBoard(void);
int  setLevel(int newLevel);
void winningLine(void);
int  traslateCharForMove(int x, int y, int player);
void printDebugMoves(void);
void refreshMoves(int);
int  updateGame(int player);
int  headsOrTails(void);
int  coinToss(int howManySides);
void player2WinsCoinToss(int player);
void player1WinsCoinToss(int player);
int  yourMove(int player);
int  computerMove(int player);
int  randomMove(int player);
int  bestPossibleMove(int player);
void clearNextMoves(void);
int  checkStaleMate(void);
void clearStatusArrays(void);
int  updateStateOfPlay(int player);
int  setBitForPlayerInSquare(int j);
int  getStatusValue(int x);
int  calculateNextMove(int state, int line, int player);

// draw.c
void ticTacToe(void);
void writeMovesToBoard(int *moves);
void writeGlyphToGrid(int* glyph, int j);
void drawScore(int score1, int score2);
void drawWinningLine(int line);
void drawGrid(void);
void redrawGrid(int line);
int clearScreen(void);
void sysOutMenu(void);
void sysOut(int write, int player);
void playEnding(void);


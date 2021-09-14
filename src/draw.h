/*
 * Write commands for the sysOut function.
 */
enum SYSDISP { 
        PLAYER_WIN,
		OPONENT_WIN,
        PLAYER_DRAW,
        PLAY_AGAIN,
        HEADS_OR_TAILS,
        PLAYER_WINS_TOSS,
		OPONENT_WINS_TOSS,
        PLAYER_MOVE,
		OPONENT_MOVE,
};

void ticTacToe(void);
void writeMovesToBoard(int *moves);
void drawScore(int score1, int score2);
void drawWinningLine(int line);
void drawGrid(void);
void redrawGrid(int line);
int clearScreen(void);
void sysOutMenu(void);
void sysOut(enum SYSDISP write);
void playEnding(void);


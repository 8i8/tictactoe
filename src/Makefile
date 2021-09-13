$(OUT)ticTacToe.o: $(SRC)ticTacToe.c $(SRC)ticTacToe.h $(SRC)logic.h $(SRC)draw.h
	$(CC) -o $@ -c $(CFLAGS) $(SRC)ticTacToe.c
$(OUT)logic.o: $(SRC)logic.c $(SRC)ticTacToe.h $(SRC)logic.h $(SRC)draw.h $(SRC)error.h
	$(CC) -o $@ -c $(CFLAGS) $(SRC)logic.c
$(OUT)draw.o: $(SRC)draw.c $(SRC)ticTacToe.h $(SRC)logic.h $(SRC)draw.h $(SRC)endscene.h $(SRC)error.h
	$(CC) -o $@ -c $(CFLAGS) $(SRC)draw.c
$(OUT)endscene.o: $(SRC)endscene.c $(SRC)endscene.h $(SRC)error.h
	$(CC) -o $@ -c $(CFLAGS) $(SRC)endscene.c

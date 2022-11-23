
CC = gcc
OBJS = RLEList.o tool/AsciiArtTool.o tool/main.o
EXEC = AsciiArtTool
DEBUG_FLAG = 
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

RLEList.o : RLEList.c RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

tool/AsciiArtTool.o : tool/AsciiArtTool.c tool/AsciiArtTool.h RLEList.h
	$(CC) -c -I. $(DEBUG_FLAG) $(COMP_FLAG) $*.c -o $*.o

tool/main.o : tool/main.c tool/AsciiArtTool.h RLEList.h
	$(CC) -c -I. $(DEBUG_FLAG) $(COMP_FLAG) $*.c -o $*.o

clean:
	rm -f $(OBJS) $(EXEC)

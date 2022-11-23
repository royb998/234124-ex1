
CC = gcc
OBJS = RLEList.o AsciiArtTool.o main.o
EXEC = AsciiArtTool
DEBUG_FLAG = 
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

RLEList.o : RLEList.c RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

AsciiArtTool.o : AsciiArtTool.c AsciiArtTool.h RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

main.o : main.c AsciiArtTool.h RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)

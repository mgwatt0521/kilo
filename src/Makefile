CFLAGS=-Wall -Wextra -pedantic -std=c99
BUILD_FOLDER=../build
OBJECTS=syntax.o abuf.o erow.o terminal.o config.o editor.o ui.o io.o search.o
OBJECT_PATHS=$(BUILD_FOLDER)/syntax.o $(BUILD_FOLDER)/abuf.o $(BUILD_FOLDER)/erow.o \
$(BUILD_FOLDER)/terminal.o $(BUILD_FOLDER)/config.o $(BUILD_FOLDER)/editor.o \
$(BUILD_FOLDER)/ui.o $(BUILD_FOLDER)/io.o $(BUILD_FOLDER)/search.o


kilo: main.c $(OBJECTS)
	$(CC) main.c $(OBJECT_PATHS) -o ../build/kilo $(CFLAGS)

erow.o: erow.c
	$(CC) erow.c -c -o $(BUILD_FOLDER)/erow.o $(CFLAGS)

syntax.o: syntax.c
	$(CC) syntax.c -c -o $(BUILD_FOLDER)/syntax.o $(CFLAGS)

abuf.o: abuf.c
	$(CC) abuf.c -c -o $(BUILD_FOLDER)/abuf.o $(CFLAGS)

terminal.o:
	$(CC) terminal.c -c -o $(BUILD_FOLDER)/terminal.o $(CFLAGS)

config.o:
	$(CC) config.c -c -o $(BUILD_FOLDER)/config.o $(CFLAGS)

editor.o:
	$(CC) editor.c -c -o $(BUILD_FOLDER)/editor.o $(CFLAGS)

ui.o:
	$(CC) ui.c -c -o $(BUILD_FOLDER)/ui.o $(CFLAGS)

io.o:
	$(CC) io.c -c -o $(BUILD_FOLDER)/io.o $(CFLAGS)

search.o:
	$(CC) search.c -c -o $(BUILD_FOLDER)/search.o $(CFLAGS)

clean:
	rm $(BUILD_FOLDER)/*

all: keylogger

keylogger: src/keylogger.o src/keylogger.cpp.o
	g++ -o keylogger src/keylogger.o src/keylogger.cpp.o -luser32

src/keylogger.o: src/keylogger.c
	gcc -c src/keylogger.c -o src/keylogger.o

src/keylogger.cpp.o: src/keylogger.cpp include/keylogger.h
	g++ -c src/keylogger.cpp -o src/keylogger.cpp.o

clean:
	rm -f keylogger src/keylogger.o src/keylogger.cpp.o

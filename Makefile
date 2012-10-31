scheduling: scheduling.c
	gcc -o ./bin/scheduling.exe scheduling.c

clean:
	rm *.o
	rm ./bin/*.exe

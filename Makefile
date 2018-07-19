all: 
	clear
	gcc ex.c -o a -pthread	
	./a
comp:	
	gcc ex.c -o a -pthread
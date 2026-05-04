all:
	gcc main.c proc.c ui.c sort.c -o sysmonitor -lncurses

run:
	./sysmonitor

clean:
	rm -f sysmonitor

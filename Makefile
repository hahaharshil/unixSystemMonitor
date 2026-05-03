all:
	gcc main.c proc.c ui.c -o sysmonitor -lncurses

run:
	./sysmonitor

clean:
	rm -f sysmonitor

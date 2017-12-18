CFLAGS  = -Wall -O2 `gtk-config --cflags`
LDFLAGS = `gtk-config --libs`

all: protwin2

protwin1: protwin1.c griddisp1.o griddisp1.h
	gcc $(CFLAGS) -o $@ protwin1.c griddisp1.o $(LDFLAGS)

protwin2: protwin2.c griddisp2.o griddisp2.h
	gcc $(CFLAGS) -o $@ protwin2.c griddisp2.o $(LDFLAGS)

griddisp1.o: griddisp1.c griddisp1.h
	gcc $(CFLAGS) -c $<

griddisp2.o: griddisp2.c griddisp2.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f protwin1 protwin2 sample griddisp1.o griddisp2.o core

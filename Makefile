
all: libtipsy.a

libtipsy.a: tipsy.o
	$(AR) rcs $@ $<

clean:
	rm -f libtipsy.a *.o

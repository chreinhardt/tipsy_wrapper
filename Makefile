
all: libtipsy.a

libtipsy.a: tipsy.o
	$(AR) -ltirpc rcs $@ $<

clean:
	rm -f libtipsy.a *.o

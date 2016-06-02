# A simple Makefile for CPSC 360

ifndef MAC
include Makefile.unix
else
include Makefile.mac
endif

.PHONY: realclean
realclean:
	find . -type f -name "*.o" -exec rm '{}' \;
	find . -type f -name "*.exe" -exec rm '{}' \;

# this line required by make - don't delete

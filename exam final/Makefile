TARGETS=sixquiprend
all: $(TARGETS)
clean: 
	$(RM) *.o $(TARGETS)
tests: $(addprefix test_, $(TARGETS))
test_%: %
	@echo ==== $< ====
	@./$<
	@echo
.PHONY: all clean tests $(addprefix test-, $(TARGETS))
sixquiprend: sixquiprend.c
	@echo Compiling: sixquiprend.c
	@make -sC exam/sixquiprend -f ../lib/Makefile ../../sixquiprend

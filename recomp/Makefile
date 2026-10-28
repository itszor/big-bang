# Static recompilation of Big Bang: the translated program (gen/ buuni, the
# mode module genmod/ z15, from superspider gen) and game.c, linked with
# libroger (the RISC OS runtime).
ROGER   ?= $(HOME)/stuff/libroger
CC      ?= gcc
CFLAGS  ?= -O2 -g -w -fno-strict-aliasing $(if $(TRACE),-DRECOMP_TRACE)
GEN     := $(wildcard gen/code*.c) gen/entries.c $(wildcard genmod/code*.c) genmod/entries.c
OBJ     := $(GEN:.c=.o) game.o
LIBS    := $(shell pkg-config --libs sdl3) -ljpeg -lm -lpthread
INC     := -I$(ROGER)/include

bigbang: $(OBJ) $(ROGER)/libroger.a
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(ROGER)/libroger.a $(LIBS)

$(ROGER)/libroger.a: FORCE
	$(MAKE) -C $(ROGER) libroger.a

gen/%.o: gen/%.c $(ROGER)/include/roger_cpu.h gen/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

genmod/%.o: genmod/%.c $(ROGER)/include/roger_cpu.h genmod/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

game.o: game.c $(ROGER)/include/roger.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Coverage build (~/stuff/superspider/phase0): "make cov" builds bigbang-cov
# from separate *.cov.o objects; it writes $$RECOMP_COV (default cov.out).
COVDIR  ?= $(HOME)/stuff/superspider/phase0/cov
COVOBJ  := $(OBJ:.o=.cov.o) cov.cov.o

cov: bigbang-cov

bigbang-cov: $(COVOBJ) FORCE
	$(MAKE) -C $(ROGER) cov COV=$(COVDIR)
	$(CC) $(CFLAGS) -o $@ $(COVOBJ) $(ROGER)/libroger-cov.a $(LIBS)

%.cov.o: %.c $(ROGER)/include/roger_cpu.h $(COVDIR)/cov.h
	$(CC) $(CFLAGS) -DRECOMP_COV $(INC) -I$(COVDIR) -c -o $@ $<

cov.cov.o: $(COVDIR)/cov.c $(COVDIR)/cov.h
	$(CC) $(CFLAGS) -c -o $@ $<

SS  ?= $(HOME)/stuff/superspider
# the files, whatever their ,load-exec suffixes
G       := ../game/!BigBang
BUUNI   := $(firstword $(wildcard $(G)/buuni,*) $(G)/buuni)
Z15     := $(firstword $(wildcard $(G)/z15,*) $(G)/z15)

regen:
	cd ../analysis && SUPERSPIDER=$(SS) ./run.sh
	rm -rf gen genmod
	$(SS)/_build/default/bin/main.exe gen '$(Z15)' 1800000 ../analysis/z15.json --c genmod \
	    --prefix=M --tabprefix=recompM --files=1 --covtag=M
	$(SS)/_build/default/bin/main.exe gen '$(BUUNI)' 8000 ../analysis/buuni.json --c gen

# "make roots" merges the gap entries a --trace-gaps run logged
# (untranslated.log, or LOG=...) into ../analysis/buuni.roots, so the next
# "make regen" translates them.
LOG ?= untranslated.log

roots:
	python3 $(SS)/tools/gap_roots.py '$(BUUNI)' 8000 ../analysis/buuni.roots $(LOG)

clean:
	rm -f gen/*.o genmod/*.o game.o bigbang bigbang-cov *.cov.o gen/*.cov.o genmod/*.cov.o

.PHONY: FORCE cov regen roots clean

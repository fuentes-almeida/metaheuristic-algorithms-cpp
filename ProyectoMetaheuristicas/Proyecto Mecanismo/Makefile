# Makefile for compiling a the idraw
CC=g++
LD=g++
RM=rm -f
#Production flags
CFLAGS=-Wno-unused-result `wx-config --cflags` 
LDFLAGS=`wx-config --libs` 
UBUNTUFLAGS=-lm $(LDFLAGS) -lstdc++ -lfftw3 -lfftw3f 
OPENSUSEFLAGS=-lm $(LDFLAGS) -lstdc++ -lfftw3 -lfftw3f 
DEBUG=-O2
OBJS:=main.o shell.o
MAIN=cshell
all:$(MAIN)
ubuntu:
	$(MAKE) "OPENSUSEFLAGS="
opensuse:
	$(MAKE) "UBUNTUFLAGS="
$(MAIN):$(OBJS)
	$(LD) $(DEBUG) $(OPENSUSEFLAGS) $(OBJS) -o $(MAIN) $(UBUNTUFLAGS)
%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) $(DEBUG) -c $<
%.o: %.c %.h
	$(CC) $(CFLAGS) $(DEBUG) -c $<	
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -c $<	
%.o: %.cpp 
	$(CC) $(CFLAGS) $(DEBUG) -c $<	
debug:
	$(MAKE) "DEBUG=-g"
clean:
	$(RM) $(OBJS) *.*~
cleanall:
	$(RM) $(OBJS) $(MAIN) *.*~

run:
	./cshell

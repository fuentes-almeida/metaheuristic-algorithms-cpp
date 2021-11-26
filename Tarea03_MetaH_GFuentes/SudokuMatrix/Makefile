CCOMP=gcc		#compilador
CLINK=gcc		#linkeo
LFLAGS=-std=c99 	#bandera de debuggeo
CFLAGS=-std=c99	-c	#banderas de compilacion
EXEC=ejecutable
OBJDIR:=obj
CFILEDIR:=src
BINDIR:=bin
CFILES:=$(wildcard $(CFILEDIR)/*.c)
LIBS:=$(wildcard $(CFILEDIR)/*.h)
OBJS:=$(patsubst $(CFILEDIR)/%.c,$(OBJDIR)/%.o,$(CFILES))
DIREXEC:=$(addprefix $(BINDIR)/,$(EXEC))

$(DIREXEC): $(OBJS) $(LIBS)
	$(CCOMP) $(LFLAGS) -o $@ $(OBJS) -lm

$(OBJDIR)/%.o: $(CFILEDIR)/%.c $(LIBS)
	$(CCOMP) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(DIREXEC) 

run:
	./bin/ejecutable ${args}

	


CCOMP=g++		#compilador
CLINK=g++		#linkeo
LFLAGS=-std=c++11 	#bandera de debuggeo
CFLAGS=-std=c++11 -fopenmp -c	#banderas de compilacion
EXEC=ejecutable
OBJDIR:=obj
CFILEDIR:=src
BINDIR:=bin
CFILES:=$(wildcard $(CFILEDIR)/*.cpp)
LIBS:=$(wildcard $(CFILEDIR)/*.hpp)
OBJS:=$(patsubst $(CFILEDIR)/%.cpp,$(OBJDIR)/%.o,$(CFILES))
DIREXEC:=$(addprefix $(BINDIR)/,$(EXEC))

$(DIREXEC): $(OBJS) $(LIBS)
	$(CCOMP) $(LFLAGS) -o $@ $(OBJS) -lgomp

$(OBJDIR)/%.o: $(CFILEDIR)/%.cpp $(LIBS)
	$(CCOMP) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(DIREXEC) 

run:
	./bin/ejecutable ${args}

	

CC = g++
CFLAGS = -Wc++11-extensions -Wmissing-declarations

SRCEXT = cpp

BINDIR = bin
EXE = train

SRCDIR = src
INCDIR = include
OBJDIR = obj
LIBDIR = lib

USINGLIB = -lgame2048

$(EXE):$(SRCDIR)/*.$(SRCEXT)
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ -I $(INCDIR) -L $(LIBDIR) $(USINGLIB) $(CFLAGS)

run:
	./$(BINDIR)/$(EXE)

clean:
	rm -r $(BINDIR)
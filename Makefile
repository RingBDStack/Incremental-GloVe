CC = gcc
#edit by bmj
#For older gcc, use -O3 or -O2 instead of -Ofast
CFLAGS = -lm -pthread -Ofast -march=native -funroll-loops -Wno-unused-result
BUILDDIR := build
SRCDIR := src

all: dir glove2 shuffle2 cooccur vocab_count combine shuffle mse

dir :
	mkdir -p $(BUILDDIR)
glove2 : $(SRCDIR)/glove2.c
	$(CC) $(SRCDIR)/glove2.c -o $(BUILDDIR)/glove2 $(CFLAGS)
shuffle2 : $(SRCDIR)/shuffle2.c
	$(CC) $(SRCDIR)/shuffle2.c -o $(BUILDDIR)/shuffle2 $(CFLAGS)
shuffle : $(SRCDIR)/shuffle.c
	$(CC) $(SRCDIR)/shuffle.c -o $(BUILDDIR)/shuffle $(CFLAGS)
cooccur : $(SRCDIR)/cooccur.c
	$(CC) $(SRCDIR)/cooccur.c -o $(BUILDDIR)/cooccur $(CFLAGS)
vocab_count : $(SRCDIR)/vocab_count.c
	$(CC) $(SRCDIR)/vocab_count.c -o $(BUILDDIR)/vocab_count $(CFLAGS)
combine : $(SRCDIR)/combine.c
	$(CC) $(SRCDIR)/combine.c -o $(BUILDDIR)/combine $(CFLAGS)
mse		: $(SRCDIR)/mse.c
	$(CC) $(SRCDIR)/mse.c -o $(BUILDDIR)/mse $(CFLAGS)
clean:
	rm -rf glove2 shuffle2 cooccur vocab_count build combine shuffle mse
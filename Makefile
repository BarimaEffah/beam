CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c2x -g
SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/beam
SRCFILES := $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c) 
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))


$(TARGET): $(OBJFILES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY:clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: print
print:
	@echo $(SRCFILES)
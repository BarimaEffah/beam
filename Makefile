CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c2x -g
SRCDIR = src
OBJDIR = obj
BINDIR = bin
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
RAYLIB = ./lib/libraylib.a

TARGET = $(BINDIR)/beam
SRCFILES := $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c) 
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))


TESTDIR = tests
TESTS = $(wildcard $(TESTDIR)/**/*.c $(TESTDIR)/*.c)
TESTTARGET = $(BINDIR)/beam_test
MAIN_OBJ = $(OBJDIR)/main.o
TEST_OBJS = $(filter-out $(MAIN_OBJ), $(OBJFILES))


$(TARGET): $(OBJFILES)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(FRAMEWORKS) $(RAYLIB) 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TESTTARGET): $(TESTS) $(TEST_OBJS) $(TESTDIR)/test.h
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(TESTDIR)/test.c $(TEST_OBJS)

.PHONEY:test
test: $(TESTTARGET)
	./$(TESTTARGET)

.PHONY: run
run: $(TARGET) 
	./$(TARGET)

.PHONY:clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: print
print:
	@echo $(SRCFILES)
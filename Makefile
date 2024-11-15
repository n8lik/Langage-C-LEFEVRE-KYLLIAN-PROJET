CC= gcc
CFLAGS= -O2 -Wall -Wextra -Werror -std=c99
CLIBS= -lm
EXE= executable
OBJ= obj/
SRC_DIR = .
MODE_DIR = mode
FILEC:= $(wildcard $(SRC_DIR)/*.c) $(wildcard $(MODE_DIR)/*.c)
FILEH:= $(wildcard $(SRC_DIR)/*.h) $(wildcard $(MODE_DIR)/*.h)
FILEO:= $(patsubst %.c,$(OBJ)%.o,$(notdir $(FILEC)))

all: $(OBJ) $(EXE)

$(OBJ):
	mkdir -p $(OBJ)

$(EXE) : $(FILEO)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

$(OBJ)%.o : $(SRC_DIR)/%.c $(FILEH)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)%.o : $(MODE_DIR)/%.c $(FILEH)
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf $(OBJ)*.o
	rm -rf $(EXE)
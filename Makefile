# build binary_tree executable when user executes "make"

LDFLAGS = -lm

APP_NAME = binary_tree

OBJ += binary_tree.o

$(APP_NAME): $(OBJ)
	$(CC) $^ -o $(APP_NAME) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

# remove object files and executable when user executes "make clean"
clean:
	rm *.o $(APP_NAME)

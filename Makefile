# build binary_tree executable when user executes "make"

APP_NAME = binary_tree

OBJ += binary_tree.o

$(APP_NAME): $(OBJ)
	$(CC) $^ -o $(APP_NAME) $(LDFLAGS)

%.o:%.c
	$(CC) -Wall $(CFLAGS) -c $^ -o $@

# remove object files and executable when user executes "make clean"
clean:
	rm *.o $(APP_NAME)

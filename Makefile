
CFLAGS=-Wall -Wshadow
OBJS=obj/main.o obj/cursesHelpers.o obj/menu.o obj/list.o obj/mainWinManager.o
INC=-Isrc
DEF=
LIB=-lncurses

all: directory curses

directory:
	@mkdir -p bin/
	@mkdir -p obj/


# compile sources to bins and deps.
obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo 'Building: $<'
	gcc $(CFLAGS) $(INC) $(DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" -c "$<"

clean:
	@rm -rf obj/*
	@rm -rf bin/*

curses: $(OBJS)
	gcc $(DEF) -o bin/$@.out $^ $(LIB)
	@echo 'Binaries at ./bin/$@.out'

.PHONY: all directory curses clean

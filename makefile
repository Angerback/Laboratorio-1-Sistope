OBJDIR := build

todo: ./src/padre.c ./src/suma.c
	mkdir -p $(OBJDIR)
	gcc ./src/padre.c -o ./build/padre
	gcc ./src/suma.c -o ./build/s
	gcc ./src/trans.c -o ./build/trans

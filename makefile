OBJDIR := build

padre: ./src/padre.c ./src/operacion.c
	mkdir -p $(OBJDIR)
	gcc ./src/padre.c -o ./build/padre
	gcc ./src/operacion.c -o ./build/s

CFLAGS := -I/home/elronnd/.perl6install/include/moar -I/home/elronnd/.perl6install/include/dyncall -g -fPIC
OBJ := port.o

default: all

all: $(OBJ)
	$(CC) $(OBJ) -Wl,-rpath=/home/elronnd/.perl6install/lib/ -lmoar -L/home/elronnd/.perl6install/lib -o port
	$(CC) port-support.c -shared -Wl,-rpath=/home/elronnd/.perl6install/lib/ -lmoar -L/home/elronnd/.perl6install/lib -o libport.so

clean:
	rm -f $(OBJ) port

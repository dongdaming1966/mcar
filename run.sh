rm a.out
gcc -g mcar.c -lm -lpthread -I header -I lib
if [ "$1" == "-d" ]; then
	gdb a.out
else
	./a.out
fi

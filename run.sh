if [ "$1" == "-r" ]; then
	make rebuild
else
	make
fi

if [ "$1" != "-d" ]; then
	./mcar
else
	gdb mcar
fi

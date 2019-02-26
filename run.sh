make
if [ "$#" == 0 ]; then
	./mcar
else
	if [ "$1" == "-d" ]; then
		gdb mcar
	fi
fi

jstest.exe: jstest2.c
	i486-mingw32-gcc $^ -lwinmm -o $@
clean:
	rm *.exe

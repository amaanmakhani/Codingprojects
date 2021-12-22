diskOperations: diskinfo.c disklist.c diskget.c diskput.c
	gcc -Wall diskinfo.c utility.c -o diskinfo
	gcc -Wall disklist.c utility.c -o disklist
	gcc -Wall diskget.c utility.c -o diskget
	gcc -Wall diskput.c utility.c -o diskput
.PHONY clean:
clean:
	-rm -rf *.o *.exe
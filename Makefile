ww: strbld.c wrap.c ww.c
	gcc -g strbld.c wrap.c ww.c -o ww

wrap: strbld.c wrap.c wrapTest.c
	gcc -g strbld.c wrap.c wrapTest.c -o wrap

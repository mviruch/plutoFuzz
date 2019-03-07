scr = main.c debugger.c breakPoint.c disasm.c msg.c
path = pluto

main : $(scr)
	gcc $(scr) -o $(path) -ludis86 -std=c99 -w

clean :
	rm -rf $(path)

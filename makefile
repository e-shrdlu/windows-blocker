windows-blocker.bpf.o: windows-blocker.bpf.c
	clang -g -target bpf -O2 -c windows-blocker.bpf.c -o windows-blocker.bpf.o

loader: loader.c
	gcc loader.c -o loader -lbpf -lelf


clean:
	rm ./loader ./windows-blocker.bpf.o

#include <bpf/libbpf.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct bpf_link *prog_link = NULL;

void cleanup(int sig) {
	if (prog_link) {
		bpf_link__destroy(prog_link);
	}
	printf("detached. exiting...\n");
	exit(0);
}

int main() {
	struct bpf_object *obj;
	struct bpf_program *prog;
	int err;

	// read file
	obj = bpf_object__open("windows-blocker.bpf.o");
	if (!obj) {
		fprintf(stderr, "failed to open bpf object, error: %s\n", strerror(errno));
		return 1;
	}

	// load into kernel
	err = bpf_object__load(obj);
	if (err) {
		fprintf(stderr, "failed to load bpf object, error: %s\n", strerror(-err));
		bpf_object__close(obj);
		return 1;
	}


	prog = bpf_object__find_program_by_name(obj, "handle_tp"); // change this to attach point
	if (!prog) {
		fprintf(stderr, "Failed to find program\n");
		bpf_object__close(obj);
		return 1;
	}

	prog_link = bpf_program__attach(prog);
	if (!prog_link) {
		fprintf(stderr, "failed to attach program: %s\n", strerror(errno));
		bpf_object__close(obj);
		return 1;
	}

	printf("bpf prog is attached I think\n");

	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);

	while (1) {
		sleep(1);
	}

	bpf_link__destroy(prog_link);
	bpf_object__close(obj);
	return 0;
}

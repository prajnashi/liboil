#include <liboil/liboilfault.h>
#include <signal.h>
#include <stdio.h>
#include <setjmp.h>
void blip(void *far) {
	write(2,"hip\n",4);
	asm("ud2");
	write(2,"hop\n",4);
}

jmp_buf buf;
void handle(int num) {
	fprintf(stderr,"my catch!\n");
	longjmp(buf, 1);
}
int main(void) {
#if 0
	struct sigaction act;
	int ret;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &handle;
	act.sa_flags = SA_NOMASK;
	sigaction(SIGILL, &act, NULL);

	ret = setjmp(buf);
	if (!ret) {
		fprintf(stderr,"soo..\n");
		blip(NULL);
	}
	if (ret)
		fprintf(stderr,"failed\n");

	ret = setjmp(buf);
	if (!ret) {
		fprintf(stderr,"soo..\n");
		blip(NULL);
	}
	if (ret)
		fprintf(stderr,"failed\n");

#else
	oil_fault_check_enable();
	fprintf(stderr,"soo..\n");
	if (!oil_fault_check_try(blip, NULL))
		fprintf(stderr,"failed\n");
	fprintf(stderr,"soo..\n");
	if (!oil_fault_check_try(blip, NULL))
		fprintf(stderr,"failed\n");
	fprintf(stderr,"done..\n");
#endif
}

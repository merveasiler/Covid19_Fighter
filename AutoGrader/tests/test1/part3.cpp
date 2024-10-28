// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp tests/test1/part3.cpp -o tests/test1/part3

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	Particle* p1 = new Particle(2,2);
	Particle* p2 = new Particle(0,2);

	Particle* q1 = new Particle(0,2);
	Particle* q2 = new Particle(-2,2);
	Particle* q3 = new Particle(-1,2);

	Wall* pWall = new Wall(*p1, *p2);
	Wall* qWall = new CurvyWall(*q1, *q2, *q3);

	if (pWall->IsContinuousLinear(*qWall))
		result = 0;
	else
		result = 1;

	delete p1;
	delete p2;
	delete q1;
	delete q2;
	delete q3;
	delete pWall;
	delete qWall;

	if (result == 0)
		return EXIT_FAIL;

	append_result_into_file(result);
	return 0;
}

int main(int argc, char* argv[]){

	// signal handlers
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags   = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);

	struct sigaction sa_sigabort;
	memset(&sa_sigabort, 0, sizeof(struct sigaction));
	sigemptyset(&sa_sigabort.sa_mask);
	sa_sigabort.sa_sigaction = segfault_sigaction_sigabort;
	sa_sigabort.sa_flags   = SA_SIGINFO;
	sigaction(SIGABRT, &sa_sigabort, NULL);

	struct rlimit rl;
	getrlimit (RLIMIT_CPU, &rl);
	rl.rlim_cur = 30;
	setrlimit(RLIMIT_CPU, &rl);

	struct sigaction sa_sigcpukill;
	memset(&sa_sigcpukill, 0, sizeof(struct sigaction));
	sigemptyset(&sa_sigcpukill.sa_mask);
	sa_sigcpukill.sa_sigaction = segfault_sigaction_sigcpukill;
	sa_sigcpukill.sa_flags   = SA_SIGINFO;
	sigaction(SIGXCPU, &sa_sigcpukill, NULL);

	setjmp(env);

	return tester();
}

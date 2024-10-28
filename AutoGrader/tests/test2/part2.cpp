// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp tests/test2/part2.cpp -o tests/test2/part2

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	Particle* p1 = new Particle(3,5);
	Particle* p2 = new Particle(7,9);

	Particle* q1 = new Particle(7,9);
	Particle* q2 = new Particle(5,9);

	Wall* pWall = new Wall(*p1, *p2);
	Wall* qWall = new Wall(*q1, *q2);

	// Test core
	try {
		const Wall* newWall = &(*pWall + *qWall);
		result = 0;
		delete newWall;
	} catch (exception& e) {
		if (((string)e.what()).compare(ApplePearException().what()) == 0)
			result = 1;
		else
			result = 0;
	}

	// Clean-up
	delete p1;
	delete p2;
	delete q1;
	delete q2;
	delete pWall;
	delete qWall;

	if (result == 0)
		return EXIT_FAIL;

	// Note the points into a file
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

// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp tests/test2/part5.cpp -o tests/test2/part5

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	Particle* p1 = new Particle(2,2);
	Particle* p2 = new Particle(0,2);
	Particle* p3 = new Particle(1,-1);

	Particle* q1 = new Particle(0,2);
	Particle* q2 = new Particle(2,2);

	Wall* pWall = new CurvyWall(*p1, *p2, *p3);
	Wall* qWall = new CurvyWall(*q1, *q2, *p3);

	// Test core
	try {
		const Wall* newWall = &(*pWall + *qWall);
		const Particle& p = newWall->GetInitialParticle();
		if (abs (p.FindDistance(*p3) - 3.162) < 0.01 && newWall->GetFinalParticle() == p && ((CurvyWall*)newWall)->GetCenter() == *p3)
			result = 1;
		else
			result = 0;
		delete newWall;
	} catch (exception& e) {
		result = 0;
	}

	// Clean-up
	delete p1;
	delete p2;
	delete p3;
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

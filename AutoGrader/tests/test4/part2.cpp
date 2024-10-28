// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/NucleoBase.cpp submission/Triangular.cpp submission/Tissue.cpp tests/test4/part2.cpp -o tests/test4/part2

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/Cell.h"
#include "../../submission/MicroOrganism.h"
#include "../../submission/Triangular.h"
#include "../../submission/Tissue.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	// define cell particles on the cell wall
	vector<Particle*> p;
	p.push_back(new Particle(0, -1));		// vertex-1
	p.push_back(new Particle(1, 0));
	p.push_back(new Particle(3, 2));		// vertex-2
	p.push_back(new Particle(-6, 2));		// vertex-3
	p.push_back(new Particle(-4, 1));

	// define cell walls
	vector<Wall*> walls;
	for (int i=0; i < p.size(); i++)
		walls.push_back(new Wall(*p[i], *p[(i+1)%p.size()]));

	// define triangular microorganism walls
	vector<Wall*> walls2;
	walls2.push_back(new Wall(3.0));
	walls2.push_back(new Wall(4.0));
	walls2.push_back(new Wall(5.0));

	// define cell
	Tissue* tissue = new Tissue();
	Cell* cell = new Cell(0, walls, tissue);

	// define triangular microorganism
	string stringChain = "AGUS";
	NucleoBase* RNA = new NucleoBase(stringChain, NULL);
	MicroOrganism* mo = new Triangular(10, walls2, *RNA);

	// Test core
	if (mo->DoesFitInto(*cell))
		result = 0;
	else
		result = 2;

	// Clean-up
	delete tissue;
	delete cell;
	delete mo;
	delete RNA;
	for (int i=0; i < walls.size(); i++)
		delete walls[i];
	for (int i=0; i < walls2.size(); i++)
		delete walls2[i];
	for (int i=0; i < p.size(); i++)
		delete p[i];

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

// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/Tissue.cpp tests/test3/part5.cpp -o tests/test3/part5

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/Cell.h"
#include "../../submission/Tissue.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	// define cell particles on the cell wall
	vector<Particle*> p;
	p.push_back(new Particle(7, 0));
	Particle* c = new Particle(0, 0);	// center

	// define cell walls
	vector<Wall*> walls;
	for (int i=0; i < p.size(); i++)
		walls.push_back(new CurvyWall(*p[i], *p[(i+1)%p.size()], *c));

	// define cell
	Tissue* tissue = new Tissue();
	Cell* cell = new Cell(0, walls, tissue);
	cell->StrengthenCellWall();
	const vector<Wall*>&  newCellWall = cell->GetCellWall();

	// Test core
	if (newCellWall.size() != 1)
		result = 0;
	else {
		const Particle& q = newCellWall[0]->GetInitialParticle();
		if (abs (q.FindDistance(*c) - 7) < 0.01 && newCellWall[0]->GetFinalParticle() == q && ((CurvyWall*)newCellWall[0])->GetCenter() == *c)
			result = 1;
		else
			result = 0;
	}

	// Clean-up
	delete tissue;
	delete cell;
	for (int i=0; i < walls.size(); i++)
		delete walls[i];
	for (int i=0; i < p.size(); i++)
		delete p[i];
	delete c;

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

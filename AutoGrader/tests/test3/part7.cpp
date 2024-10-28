// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/Tissue.cpp tests/test3/part7.cpp -o tests/test3/part7

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
	p.push_back(new Particle(1, 3.828));
	p.push_back(new Particle(2, -4));
	p.push_back(new Particle(3, -3.828));	// vertex-1
	p.push_back(new Particle(5, -1.828));
	p.push_back(new Particle(7, -3.828));	// vertex-2
	p.push_back(new Particle(-1, -1));		// vertex-3

	vector<Particle*> c;					// centers
	c.push_back(new Particle(2, -1));
	c.push_back(new Particle(5, -3.828));
	c.push_back(new Particle(4, 0.837));

	// define cell walls
	vector<Wall*> walls;
	walls.push_back(new CurvyWall(*p[0], *p[1], *c[0]));
	walls.push_back(new CurvyWall(*p[1], *p[2], *c[0]));
	walls.push_back(new CurvyWall(*p[2], *p[3], *c[1]));
	walls.push_back(new CurvyWall(*p[3], *p[4], *c[1]));
	walls.push_back(new CurvyWall(*p[4], *p[5], *c[2]));
	walls.push_back(new CurvyWall(*p[5], *p[0], *c[0]));

	// define cell
	Tissue* tissue = new Tissue();
	Cell* cell = new Cell(0, walls, tissue);
	cell->StrengthenCellWall();
	const vector<Wall*>&  newCellWall = cell->GetCellWall();

	// Test core
	if (newCellWall.size() != 3)
		result = 0;
	else {
		if (newCellWall[0]->GetInitialParticle() == *p[2] && newCellWall[0]->GetFinalParticle() == *p[4] && ((CurvyWall*)newCellWall[0])->GetCenter() == *c[1] &&
			newCellWall[1]->GetInitialParticle() == *p[4] && newCellWall[1]->GetFinalParticle() == *p[5] && ((CurvyWall*)newCellWall[1])->GetCenter() == *c[2] &&
			newCellWall[2]->GetInitialParticle() == *p[5] && newCellWall[2]->GetFinalParticle() == *p[2] && ((CurvyWall*)newCellWall[2])->GetCenter() == *c[0])
			result = 2;
		else if (newCellWall[1]->GetInitialParticle() == *p[2] && newCellWall[1]->GetFinalParticle() == *p[4] && ((CurvyWall*)newCellWall[1])->GetCenter() == *c[1] &&
				 newCellWall[2]->GetInitialParticle() == *p[4] && newCellWall[2]->GetFinalParticle() == *p[5] && ((CurvyWall*)newCellWall[2])->GetCenter() == *c[2] &&
				 newCellWall[0]->GetInitialParticle() == *p[5] && newCellWall[0]->GetFinalParticle() == *p[2] && ((CurvyWall*)newCellWall[0])->GetCenter() == *c[0])
			result = 2;
		else if (newCellWall[2]->GetInitialParticle() == *p[2] && newCellWall[2]->GetFinalParticle() == *p[4] && ((CurvyWall*)newCellWall[2])->GetCenter() == *c[1] &&
				 newCellWall[0]->GetInitialParticle() == *p[4] && newCellWall[0]->GetFinalParticle() == *p[5] && ((CurvyWall*)newCellWall[0])->GetCenter() == *c[2] &&
				 newCellWall[1]->GetInitialParticle() == *p[5] && newCellWall[1]->GetFinalParticle() == *p[2] && ((CurvyWall*)newCellWall[1])->GetCenter() == *c[0])
			result = 2;
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
	for (int i=0; i < c.size(); i++)
		delete c[i];

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

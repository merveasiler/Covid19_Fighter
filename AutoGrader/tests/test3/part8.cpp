// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/Tissue.cpp tests/test3/part8.cpp -o tests/test3/part8

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
	p.push_back(new Particle(-2, 0));		// vertex-1
	p.push_back(new Particle(-1, -1));
	p.push_back(new Particle(0, -2));		// vertex-2
	p.push_back(new Particle(1, -2));
	p.push_back(new Particle(2, -2));
	p.push_back(new Particle(3, -2));		// vertex-3
	p.push_back(new Particle(2.438, 0));
	p.push_back(new Particle(3, 1));		// vertex-4
	p.push_back(new Particle(0, 2));		// vertex-5
	p.push_back(new Particle(-1, 1.73));
	p.push_back(new Particle(-1.5, 1.32));

	vector<Particle*> c;					// centers
	c.push_back(new Particle(4.5, -0.5));
	c.push_back(new Particle(0, 0));

	// define cell walls
	vector<Wall*> walls;
	walls.push_back(new Wall(*p[0], *p[1]));
	walls.push_back(new Wall(*p[1], *p[2]));
	walls.push_back(new Wall(*p[2], *p[3]));
	walls.push_back(new Wall(*p[3], *p[4]));
	walls.push_back(new Wall(*p[4], *p[5]));
	walls.push_back(new CurvyWall(*p[5], *p[6], *c[0]));
	walls.push_back(new CurvyWall(*p[6], *p[7], *c[0]));
	walls.push_back(new Wall(*p[7], *p[8]));
	walls.push_back(new CurvyWall(*p[8], *p[9], *c[1]));
	walls.push_back(new CurvyWall(*p[9], *p[10], *c[1]));
	walls.push_back(new CurvyWall(*p[10], *p[0], *c[1]));

	// define cell
	Tissue* tissue = new Tissue();
	Cell* cell = new Cell(0, walls, tissue);
	cell->StrengthenCellWall();
	const vector<Wall*>&  newCellWall = cell->GetCellWall();

	// Test core
	if (newCellWall.size() != 5)
		result = 0;
	else {
		for (int i=0; i<5; i++)
			if (newCellWall[i]->GetInitialParticle() == *p[0] && newCellWall[(i+1)%5]->GetInitialParticle() == *p[2])
			 	if (newCellWall[(i+2)%5]->GetInitialParticle() == *p[5] && newCellWall[(i+2)%5]->GetFinalParticle() == *p[7] && ((CurvyWall*)newCellWall[(i+2)%5])->GetCenter() == *c[0] &&
					newCellWall[(i+3)%5]->GetInitialParticle() == *p[7] &&
					newCellWall[(i+4)%5]->GetInitialParticle() == *p[8] && newCellWall[(i+4)%5]->GetFinalParticle() == *p[0] && ((CurvyWall*)newCellWall[(i+4)%5])->GetCenter() == *c[1]) {
						result = 3;
						break;
				}
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

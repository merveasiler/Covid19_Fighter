// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/Tissue.cpp tests/test3/part2.cpp -o tests/test3/part2

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
	p.push_back(new Particle(-1, 4));
	p.push_back(new Particle(-4, 4));	// vertex-1
	p.push_back(new Particle(-4, 2));
	p.push_back(new Particle(-4, -3));	// vertex-2
	p.push_back(new Particle(3, -3));	// vertex-3
	p.push_back(new Particle(3, -1));
	p.push_back(new Particle(3, 1));
	p.push_back(new Particle(3, 4));	// vertex-4
	p.push_back(new Particle(0, 4));

	// define cell walls
	vector<Wall*> walls;
	for (int i=0; i < p.size(); i++)
		walls.push_back(new Wall(*p[i], *p[(i+1)%p.size()]));

	// define cell
	Tissue* tissue = new Tissue();
	Cell* cell = new Cell(0, walls, tissue);
	cell->StrengthenCellWall();
	const vector<Wall*>&  newCellWall = cell->GetCellWall();

	// Test core
	if (newCellWall.size() != 4)
		result = 0;
	else {
		if (newCellWall[0]->GetInitialParticle() == *p[1] && newCellWall[1]->GetInitialParticle() == *p[3] && newCellWall[2]->GetInitialParticle() == *p[4] && newCellWall[3]->GetInitialParticle() == *p[7])
			result = 2;
		else if (newCellWall[1]->GetInitialParticle() == *p[1] && newCellWall[2]->GetInitialParticle() == *p[3] && newCellWall[3]->GetInitialParticle() == *p[4] && newCellWall[0]->GetInitialParticle() == *p[7])
			result = 2;
		else if (newCellWall[2]->GetInitialParticle() == *p[1] && newCellWall[3]->GetInitialParticle() == *p[3] && newCellWall[0]->GetInitialParticle() == *p[4] && newCellWall[1]->GetInitialParticle() == *p[7])
			result = 2;
		else if (newCellWall[3]->GetInitialParticle() == *p[1] && newCellWall[0]->GetInitialParticle() == *p[3] && newCellWall[1]->GetInitialParticle() == *p[4] && newCellWall[2]->GetInitialParticle() == *p[7])
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

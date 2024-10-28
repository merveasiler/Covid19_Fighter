// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/Tissue.cpp tests/test3/part6.cpp -o tests/test3/part6

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
	p.push_back(new Particle(0, 5));
	p.push_back(new Particle(-3, 4));	// vertex-1
	p.push_back(new Particle(-4, 3));	// vertex-2
	p.push_back(new Particle(-3, 2));
	p.push_back(new Particle(-2, 1));
	p.push_back(new Particle(-1, 0));	// vertex-3
	p.push_back(new Particle(0, 2));	// vertex-4
	p.push_back(new Particle(-1, 2));	// vertex-5
	p.push_back(new Particle(0, 4));	// vertex-6
	p.push_back(new Particle(3, 3));	// vertex-7
	p.push_back(new Particle(3, 5));
	p.push_back(new Particle(3, 6));	// vertex-8

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
	if (newCellWall.size() != 8)
		result = 0;
	else {
		Particle* vertices[8] = {p[1], p[2], p[5], p[6], p[7], p[8], p[9], p[11]};
		for (int i=0; i<8; i++) {
			bool is_matched = true;
			for (int j=0; j<8; j++) {
				if (newCellWall[(i+j)%8]->GetInitialParticle() == *vertices[j])
				 	continue;
				else {
					is_matched = false;
					break;
				}
			}
			if (is_matched) {
				result = 2;
				break;
			}
		}
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

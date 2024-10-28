// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/NucleoBase.cpp submission/Triangular.cpp submission/Tissue.cpp tests/test5/part1.cpp -o tests/test5/part1

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/Cell.h"
#include "../../submission/MicroOrganism.h"
#include "../../submission/Triangular.h"
#include "../../submission/Tissue.h"
#include "../../submission/Main.h"
#include "../../grader/common.h"

int tester(const char* cellFile) {

	int result = 0;
	int cellID = 3;	// id of the cell which will be used to connect the microorganism in this test case

	// Read Cells
	Tissue* tissue = new Tissue();
	Cell* cell = ReadCells(cellFile, *tissue, cellID);

	// define triangular microorganism walls
	vector<Wall*> walls;
	walls.push_back(new Wall(9.0));
	walls.push_back(new Wall(6.708));
	walls.push_back(new Wall(4.24));

	// define triangular microorganism
	string stringChain = "AGUS";
	NucleoBase* RNA = new NucleoBase(stringChain, NULL);
	MicroOrganism* mo = new Triangular(10, walls, *RNA);

	// Test core
	mo->ConnectToCell(cell);
	mo->React();
	try {
		const Cell& cell2 = tissue->GetCell(cellID);
		result = 0;
	}
	catch (exception& e) {
		if (((string)e.what()).compare(DeadCellException().what()) == 0)
			result = 4;
		else
			result = 0;
	}

	// Clean-up
	delete tissue;
	delete mo;
	delete RNA;
	for (int i=0; i < walls.size(); i++)
		delete walls[i];

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

	return tester(argv[1]);
}

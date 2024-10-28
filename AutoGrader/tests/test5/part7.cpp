// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/Circular.cpp submission/Tissue.cpp tests/test5/part7.cpp -o tests/test5/part7

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/Cell.h"
#include "../../submission/MicroOrganism.h"
#include "../../submission/Circular.h"
#include "../../submission/Tissue.h"
#include "../../submission/Main.h"
#include "../../grader/common.h"

int tester(const char* cellFile) {

	int result = 0;
	int cellID = 5;	// id of the cell which will be used to connect the microorganism in this test case

	// Read Cells
	Tissue* tissue = new Tissue();
	Cell* cell = ReadCells(cellFile, *tissue, cellID);

	// define circular microorganism
	float radius = 11.31;
	MicroOrganism* mo = new Circular(10, radius);

	// Test core
	mo->ConnectToCell(cell);
	mo->React();
	const vector<Wall*>& cellWall = cell->GetCellWall();
	Particle center(-1, -1);
	int i;
	for (i=0; i < cellWall.size(); i++) {
		const CurvyWall* curvyWall = (CurvyWall*)cellWall[i];
		if (abs(curvyWall->GetInitialParticle().FindDistance(center) -radius*2) < 0.01 &&
			abs(curvyWall->GetFinalParticle().FindDistance(center) -radius*2) < 0.01 &&
			curvyWall->GetCenter() == center &&
			curvyWall->GetFinalParticle() == cellWall[(i+1)%cellWall.size()]->GetInitialParticle())
				continue;
		else
			break;
	}

	if (i == cellWall.size())
		result = 4;

	// Clean-up
	delete tissue;
	delete mo;

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

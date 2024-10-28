// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/NucleoBase.cpp submission/Triangular.cpp submission/Tissue.cpp tests/test5/part2.cpp -o tests/test5/part2

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
	int cellID = 7;	// id of the cell which will be used to connect the microorganism in this test case

	// Read Cells
	Tissue* tissue = new Tissue();
	Cell* cell = ReadCells(cellFile, *tissue, cellID);

	// define cell walls of the cell which will be used in the next steps
	vector<Wall*> walls2;
	walls2.push_back(new Wall(Particle(-3, 7), Particle(12, 7)));
	walls2.push_back(new Wall(Particle(12, 7), Particle(12, 15)));
	walls2.push_back(new Wall(Particle(12, 15), Particle(-3, 7)));

	// define triangular microorganism walls
	vector<Wall*> walls;
	walls.push_back(new Wall(8.0));
	walls.push_back(new Wall(15.0));
	walls.push_back(new Wall(17.0));

	// define triangular microorganism
	string stringChain = "AGUS";
	NucleoBase* RNA = new NucleoBase(stringChain, NULL);
	MicroOrganism* mo = new Triangular(10, walls, *RNA);

	// Test core
	mo->ConnectToCell(cell);
	mo->React();
	try {
		const Cell& cell2 = tissue->GetCell(cellID);
	} catch (exception& e) {
		if (((string)e.what()).compare(DeadCellException().what()) == 0) {
			int newCellID = 11;
			Cell* newCell = new Cell(newCellID, walls2, tissue);
			tissue->AddCell(*newCell);
			mo->ConnectToCell(newCell);
			mo->React();
			try {
				const Cell& cell3 = tissue->GetCell(newCellID);
			} catch (exception& e2) {
				if (((string)e2.what()).compare(DeadCellException().what()) == 0)
					result = 3;
			}
		}
	}

	// Clean-up
	delete tissue;
	delete mo;
	delete RNA;
	for (int i=0; i < walls.size(); i++)
		delete walls[i];
	for (int i=0; i < walls2.size(); i++)
		delete walls2[i];

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

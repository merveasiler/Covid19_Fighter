// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/Squadratic.cpp submission/Tissue.cpp tests/test5/part5.cpp -o tests/test5/part5

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/Cell.h"
#include "../../submission/MicroOrganism.h"
#include "../../submission/Squadratic.h"
#include "../../submission/Tissue.h"
#include "../../submission/Main.h"
#include "../../grader/common.h"

int tester(const char* cellFile) {

	int result = 0;
	int cellID = 8;	// id of the cell which will be used to connect the microorganism in this test case

	// Read Cells
	Tissue* tissue = new Tissue();
	Cell* cell = ReadCells(cellFile, *tissue, cellID);

	// define squadratic microorganism walls
	MicroOrganism* mo = new Squadratic(10, 45.25);

	// Test core
	mo->ConnectToCell(cell);
	mo->React();
	try {
		Squadratic& child = ((Squadratic*)mo)->GetChild(34, 66, -34, -2);
	} catch (exception& e) {
		result = 2;
		try {
			Squadratic& child1 = ((Squadratic*)mo)->GetChild(34, 66, -18, 14);
			child1.React();
			Squadratic& child2 = ((Squadratic*)mo)->GetChild(50, 82, -34, -2);
			child2.React();
			Squadratic& grandchild1 = child1.GetChild(42, 58, -2, 14);
			grandchild1.React();
			Squadratic& grandgrandchild1 = ((Squadratic*)mo)->GetChild(46, 54, -2, 6);
			Squadratic& sameofgrandchild1 = ((Squadratic*)mo)->GetChild(42, 58, -2, 14);
			try {
				Squadratic grandchild2 = child2.GetChild(46, 54, -2, 6);
			} catch (exception& e) {
				try {
					Squadratic grandchild3 = child2.GetChild(58, 66, -22, -14);
				} catch (exception& e) {
					if (((string)e.what()).compare(NotBornChildException().what()) == 0)
						result = 5;
				}
			}
		}
		catch (exception& e) {}
	}

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

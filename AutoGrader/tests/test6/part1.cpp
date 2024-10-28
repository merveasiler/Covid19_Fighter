// g++ -ansi -pedantic evaluator/common.cpp submission/Particle.cpp submission/Wall.cpp submission/CurvyWall.cpp submission/Cell.cpp submission/MicroOrganism.cpp submission/NucleoBase.cpp submission/Triangular.cpp submission/Tissue.cpp tests/test6/part1.cpp -o tests/test6/part1

#include "../../submission/Exception.h"
#include "../../submission/Particle.h"
#include "../../submission/Wall.h"
#include "../../submission/CurvyWall.h"
#include "../../submission/MicroOrganism.h"
#include "../../submission/Triangular.h"
#include "../../submission/Tissue.h"
#include "../../submission/Main.h"
#include "../../grader/common.h"

int tester() {

	int result = 0;

	// define triangular microorganism walls
	vector<Wall*> walls;
	walls.push_back(new Wall(3));
	walls.push_back(new Wall(4));
	walls.push_back(new Wall(5));

	// define triangular microorganisms
	string stringChain1 = "AGUSAUGAASGAUUUUGSSUGA";
	NucleoBase* RNA1 = new NucleoBase(stringChain1, NULL);
	MicroOrganism* mo1 = new Triangular(10, walls, *RNA1);
	string stringChain2 = "AGUSGUSAASSUUAUUGUSUGA";
	NucleoBase* RNA2 = new NucleoBase(stringChain2, NULL);
	MicroOrganism* mo2 = new Triangular(20, walls, *RNA2);

	// Test core
	((Triangular*)mo1)->Mutate(*((Triangular*)mo2));
	const NucleoBase& mutatedRNA1 = ((Triangular*)mo1)->GetRNA();
	const NucleoBase& mutatedRNA2 = ((Triangular*)mo2)->GetRNA();
	ostringstream os1("");
	os1 << mutatedRNA1;
	ostringstream os2("");
	os2 << mutatedRNA2;
	if (os1.str() == "AGUSSSUASUGA" && os2.str() == "AGUSGGAUSUGA")
		result = 5;

	// Clean-up
	delete mo1;
	delete mo2;
	delete RNA1;
	delete RNA2;
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

	return tester();
}

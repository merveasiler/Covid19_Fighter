#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

// g++ -ansi -pedantic run_tests.cpp -o run_tests

#define _NUMBER_OF_TESTS 7			// 6+1	(+1 is for making the first array element dummy)
#define _MAX_NUMBER_OF_PARTS 10		// 9+1	(+1 is for making the first array element dummy)
#define _PENALTY_PERCENTAGE 0.15	// 15% memory leak penalty

int _NUMBER_OF_PARTS[_NUMBER_OF_TESTS] = {-1, 6, 6, 8, 9, 7, 4};
int max_points[_NUMBER_OF_TESTS][_MAX_NUMBER_OF_PARTS] =
{
	{},
	{0, 1, 1, 1, 1, 2, 2},
	{0, 2, 1, 2, 2, 1, 2},
	{0, 2, 2, 2, 1, 1, 2, 2, 3},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 4, 3, 5, 5, 5, 4, 4},
	{0, 5, 5, 5, 4}
};
int points[_NUMBER_OF_TESTS][_MAX_NUMBER_OF_PARTS];
float penalties[_NUMBER_OF_TESTS][_MAX_NUMBER_OF_PARTS];

void printDetailedResults();
int readResult();
float checkMemoryLeaks(int testId, int partId);

int main(int argc, char* argv[]) {

	const char* COMPILER = "make test%d_part%d";
	const char* RUNNER = "make run_test%d_part%d";
	char buffer[1024];
	int status = 0;
	int retval_compile_check = 0;
	int retval_run_check = 0;
	float grade = 0;
	int STUDENT_ID;

	for (int i=1; i <= _NUMBER_OF_TESTS; i++) {
		for (int j=1; j <= _NUMBER_OF_PARTS[i]; j++) {
			sprintf(buffer, COMPILER, i, j);
			status = system(buffer);
			retval_compile_check = WEXITSTATUS(status);

			if (retval_compile_check == 0) {
				sprintf(buffer, RUNNER, i, j);
				status = system(buffer);
				retval_run_check = WEXITSTATUS(status);

				if (retval_run_check == 0) {
					points[i][j] = readResult();
					if (points[i][j] > 0) {
						float penalty_rate = checkMemoryLeaks(i, j);
						penalties[i][j] = points[i][j] * penalty_rate;
						grade += points[i][j] + penalties[i][j];
					}
				}
 			}
		}
	}

	//printDetailedResults();

	sscanf(argv[1], "%d", &STUDENT_ID);
	fstream myfile("grades.txt", ios::app);;
	myfile << STUDENT_ID << " " << grade << endl;
	myfile.close();


	return 0;
}

int readResult() {

	int amount = 0;
	ifstream myfile("result.txt");
	if (myfile.is_open()) {
		myfile >> amount;
		myfile.close();
		remove("result.txt");
	}

	return amount;
}

float checkMemoryLeaks(int testID, int partID) {

	// memory leak detection
	const char* VALGRIND_CMD = "make valgrind_test%d_part%d";
	char buffer[1024];
	int status = 0;
	int retval_memcheck = 0;
	float penalty_rate = 0;

	sprintf(buffer, VALGRIND_CMD, testID, partID);
	status = system(buffer);
	retval_memcheck = WEXITSTATUS(status);

	/* retval_memcheck values:
	   142 : memory leak problem
	   127 : executable file does not exist
	   139 : executable file gives segmentation fault
	*/
	if (retval_memcheck == 142)
		penalty_rate = -1.0 * _PENALTY_PERCENTAGE;
	else if ( retval_memcheck == 0 || retval_memcheck == 127)
		penalty_rate = 0;
	else
		penalty_rate = -1.0 * _PENALTY_PERCENTAGE;

	remove("result.txt");
	return penalty_rate;
}

void printDetailedResults() {

	float grade = 0, penalty = 0;
	fstream logFile("logs.txt", ios::app);

	logFile << endl << endl;
	for (int i=1; i <= _NUMBER_OF_TESTS; i++) {
		for (int j=1; j <= _NUMBER_OF_PARTS[i]; j++) {
				grade += points[i][j];
				grade += penalties[i][j];

				if (points[i][j] < max_points[i][j]) {
					logFile << "You got " << points[i][j] << " out of " << max_points[i][j];
					logFile << " points from test" << i << "/part" << j << endl;
				}
				if (penalties[i][j] > 0) {
					logFile << "PENALTY: " << penalties[i][j];
					logFile << " point(s) cut-off was applied due to memory leaks from test";
					logFile << i << "/part" << j << endl;
				}
			}
	}

	logFile << "TOTAL GRADE: " << grade << endl;
	logFile.close();
}

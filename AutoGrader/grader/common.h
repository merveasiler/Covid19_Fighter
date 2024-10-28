#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

#include <csignal>
#include <cstring>
#include <csetjmp>
#include <sys/time.h>
#include <sys/resource.h>

static jmp_buf  env;

using namespace std;

#define EXIT_FAIL -1
#define RESULT_FILE_NAME "result.txt"

// Segmentation Fault Methods
void segfault_sigaction(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

// Result Noting Method
void append_result_into_file(int result);

#endif

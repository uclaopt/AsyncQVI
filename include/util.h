#ifndef UTIL_H
#define UTIL_H

#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <random>
#include "util.h"
using namespace std;
extern std::default_random_engine generator;
extern std::mt19937 rng; 

struct Params{
	int max_outer_iter;
	int max_inner_iter;
	int sample_num;			
	double gamma = 0.99;	// discounted factor
	double epsilon = 0.3;	// Q-learning exploration parameter
	double alpha;           // Q-learning learning rate
	int len_state;			// dimension of state space
	int len_action;			// dimension of action space
	int style;         		// sample style
	int total_num_threads;  // total number of threads
	int algo;				// which algorithm to run
	double time;
	double test_time=0;
	int save = 0;			// save final policy if 1
	int test_max_episode = 100; // test episodes
	int test_max_step = 200;	// how many steps to go in one test episode
	int check_step;				// how often to check policy
	int stop=0;
	int threshold=0;
};

// load parameters from makefile
void parse_input_argv(Params* para, int argc, char *argv[]){
	
	if (argc < 2) {
		cout << "Input number error: [0]" << endl;
		return;
	}

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-') {
			break;
		}
		if (++i >= argc) {
			cout << "Input number error: [1]" << endl;
			return;
		}
		else if (std::string(argv[i - 1]) == "-len_state") {
			para->len_state = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-len_action") {
			para->len_action = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-max_outer_iter") {
			para->max_outer_iter = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-max_inner_iter") {
			para->max_inner_iter = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-test_max_episode") {
			para->test_max_episode = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-test_max_step") {
			para->test_max_step = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-sample_num") {
			para->sample_num = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-check_step") {
			para->check_step = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-style") {
			para->style = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-save") {
			para->save = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-alpha") {
			para->alpha = atof(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-algo") {
			para->algo = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-nthreads") {
			para->total_num_threads = atoi(argv[i]);
		}
		else {
			cout << "Input number error: [2]" << endl;
			return;
		}
	}
	return;
}

//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time(){
  LARGE_INTEGER time,freq;
  if (!QueryPerformanceFrequency(&freq)){
    //  Handle error
    return 0;
  }
  if (!QueryPerformanceCounter(&time)){
    //  Handle error
    return 0;
  }
  return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time(){
  FILETIME a,b,c,d;
  if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
    //  Returns total user time.
    //  Can be tweaked to include kernel times as well.
    return
        (double)(d.dwLowDateTime |
                 ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
  }else{
    //  Handle error
    return 0;
  }
}
// generate a uniformly random integer in [start, end]
int uniformInt(int start, int end){
	return start + rand() % (end-start+1);
}
// generate a uniformly distributed double in (start, end)
double uniformDouble(double start, double end){
	return start + ((double) rand() / (RAND_MAX))*(end-start);
}
// generate a normally distributed double with mean and variance
double normalDouble(double mean, double var){
	std::normal_distribution<double> distribution(mean, var);
	return distribution(generator);
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time(){
  struct timeval time;
  if (gettimeofday(&time,NULL)){
    //  Handle error
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
  return (double)clock() / CLOCKS_PER_SEC;
}

// generate a uniformly random integer in [start, end]
int uniformInt(int start, int end){
	std::uniform_int_distribution<int> uni(start, end); // guaranteed unbiased
	return uni(rng);
}

// generate a uniformly random double in (start, end)
double uniformDouble(double start, double end){
	std::uniform_real_distribution<double> unif(0,1);
	return start + unif(rng)*(end-start);
}

// generate a normally distributed double with mean and variance
double normalDouble(double mean, double var){ 
	std::normal_distribution<double> normal(mean, var);
	return normal(rng);
}

#endif
		
#endif
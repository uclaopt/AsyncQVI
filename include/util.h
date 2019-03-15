#ifndef UTIL_H
#define UTIL_H

#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <random>
#include "util.h"
using namespace std;

extern std::mt19937 global_rng; 

struct Params{
	/* sample oracle hyperparameters */
	int len_state;				// dimension of state space
	int len_action;				// dimension of action space
	double probs = 0.;  		// probability of being trapped in vortex in sailing problem
	double d = 0.05;            // reward scale
	double gamma = 0.99;		// discounted factor
	int test_max_episode = 100; // test episodes
	int test_max_step = 200;	// how many steps to go in one test episode
	
	/* algorithms hyperparameters */
	int algo;					// which algorithm to run
	int style;         			// sample style: 0 uniform, 1 cyclic, 2 markovian
	int total_num_threads = 1;  // total number of threads
	int max_outer_iter = 1;
	int max_inner_iter = 1;
	int sample_num_1 = 1;
	int sample_num_2 = 1;
	double explore = 0.3;		// QL exploration rate in Markovian sampling
	double alpha = 1.;          // QL learning rate
	double alpha1 = 0.;         // \alpha_1 in Alg.1, VRQVI
	double epsilon = 0.;        // monotonic parameter of QVI and VRVI
	int save = 0;				// save final policy if 1
	int check_step;			    // how often to check policy
	
	/* fixed setting */
	int stop = 0;
	int threshold = 0;
	double time;
	double test_time = 0;
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
		else if (std::string(argv[i - 1]) == "-probs") {
			para->probs = atof(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-d") {
			para->d = atof(argv[i]);
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
		else if (std::string(argv[i - 1]) == "-sample_num_1") {
			para->sample_num_1 = atoi(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-sample_num_2") {
			para->sample_num_2 = atoi(argv[i]);
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
		else if (std::string(argv[i - 1]) == "-alpha1") {
			para->alpha1 = atof(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-explore") {
			para->explore = atof(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-gamma") {
			para->gamma = atof(argv[i]);
		}
		else if (std::string(argv[i - 1]) == "-epsilon") {
			para->epsilon = atof(argv[i]);
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

// generate a uniformly random integer in [start, end]
int uniformInt(int start, int end){
	std::uniform_int_distribution<int> uni(start, end); // guaranteed unbiased
	return uni(global_rng);
}

// generate a uniformly random double in (start, end)
double uniformDouble(double start, double end){ 
	std::uniform_real_distribution<double> unif(start,end);
	return unif(global_rng);
}

// generate a normally distributed double with mean and standard deviation
double normalDouble(double mean, double sd){ 
	std::normal_distribution<double> normal(mean, sd);
	return normal(global_rng);
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
#endif
		
#endif

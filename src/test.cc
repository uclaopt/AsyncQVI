#include <iostream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <vector>
#include <cmath>
#include "async.h"
#include "algo.h"
#include "oracle.h"
#include <time.h>
using namespace std;

std::atomic<int> iter(1);    // global iteration counter
pthread_mutex_t writelock;   // writing lock
pthread_barrier_t barrier;   // sync barrier
pthread_barrierattr_t attr;

// random number generator on Windows
std::default_random_engine generator;
// random number generator on Linux
std::random_device rd;     
std::mt19937 rng(rd()); 

int main(int argc, char** argv){
	
	/* Step 0: load parameters from makefile.(defined in util.h) */
	Params params;
	parse_input_argv(&params, argc, argv);
	
	/* Step 1: choose an algorithm in makefile -algo, 0 is AsyncQVI, 1 is Qlearning, 2 is VRVI */
	params.time = get_wall_time(); 
	// policy vector
	std::vector<int> pi(params.len_state, 0.);
	pthread_barrier_init(&barrier, &attr, params.total_num_threads);
	cout<<"iter time reward flag"<<endl;
	
	// run AsyncQVI 
	if(params.algo == 0){
		// state value vector, V[i] = argmax_a Q(i,a)
		std::vector<double> V(params.len_state, 0.); 
		
		// QVI object (defined in algo.h)
		QVI obj(&V, &pi, &params);
		
		// launch parallel threads
		std::vector<std::thread> mythreads;
		for (size_t i = 0; i < params.total_num_threads; i++) {
			mythreads.push_back(std::thread(asyncQVI, i, obj, &params));
		} 
		for (size_t i = 0; i < params.total_num_threads; i++) {
			mythreads[i].join();
		}
	}
	// run Async Q-learning
	else if(params.algo == 1){	
		// Q value vector, Q[i,a] = Q(i,a)
		std::vector<std::vector<double>> Q(params.len_state, std::vector<double>(params.len_action, 0.));
		// state value vector, V[i] = argmax_a Q(i,a)
		std::vector<double> V(params.len_state);
		
		// Qlearning object (defined in algo.h)
		Qlearning obj(&Q, &V, &pi, &params);
		
		// launch parallel threads
		std::vector<std::thread> mythreads;
		for (size_t i = 0; i < params.total_num_threads; i++) {
			mythreads.push_back(std::thread(asyncQL, i, obj, &params));
		} 
		for (size_t i = 0; i < params.total_num_threads; i++) {
			mythreads[i].join();
		}
	}
	// run VRVI
	else{
		// the x matrix in paper (Algo 3)
		std::vector<std::vector<double>> x(params.len_state, std::vector<double>(params.len_action, 0.));
		// input v_0 of Randomized VI 
		std::vector<double> v_outer(params.len_state, 0.);
		// input v_l of ApxVal
		std::vector<double> v_inner(params.len_state, 0.);
		
		// VRVI object (defined in algo.h)
		VRVI obj(&x, &v_outer, &v_inner, &pi, &params); 
		obj.solve();
	}
	
	// Step 2: save results
	if(params.save){
		std::ofstream outFile("policy.txt");
		for (int i = 0; i < params.len_state; i++){
			outFile << pi[i] << "\n";
		}
		outFile.close();
	}
	return 0;
}
			
#ifndef ASYNC_H_
#define ASYNC_H_

#include <iostream>
#include <thread>
#include <atomic>
#include "algo.h"
#include "oracle.h"
using namespace std;
extern std::atomic<int> iter;
extern pthread_barrier_t barrier; 

// asynchronous running with multiple QVI objects 
void asyncQVI(int thread_id, QVI qvi, Params* params) {
	
	qvi.randGenerator(thread_id);
	
	while(!params->stop){
		qvi.update(iter);
		iter++;
		
		// evaluate policy every check_step iterations  
		if(iter > params->threshold){
			// let one thread check policy quality
			pthread_barrier_wait(&barrier);
			if(thread_id == 0){
				cout<<iter<<' ';
				qvi.test();
				params->threshold += params->check_step;
				if(iter > params->max_outer_iter)
					params->stop = 1;
			}
			pthread_barrier_wait(&barrier);
		}			
	}
	return;
}

// asynchronous running with multiole Qlearning objects
void asyncQL(int thread_id, Qlearning ql, Params* params) {
	
	ql.randGenerator(thread_id);
	
	while(!params->stop){
		ql.update(iter);
		iter++;
		
		// evaluate policy every check_step iterations
		if(iter > params->threshold){
			// let one thread check policy quality
			pthread_barrier_wait(&barrier);
			if(thread_id == 0){
				cout<<iter<<' ';
				ql.test();
				params->threshold += params->check_step;
				if(iter > params->max_outer_iter)
					params->stop = 1;
			}
			pthread_barrier_wait(&barrier);
		}	
	}
	return;
}
#endif
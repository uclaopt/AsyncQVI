#ifndef ORACLE_H
#define ORACLE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <random>
#include "util.h"
#define DIMWIND 8
using namespace std; 

// sample orable for sailing problem
class Sailing{
    
	private:
		int x; 					// x coordinate of current position
		int y; 					// y coordinate of current position
		int wind; 				// current wind direction
		int DIMX; 				// range of x
		int DIMY; 				// range of y
		int GOALX;				// x coordinate of Goal state
		int GOALY;				// y coordinate of Goal state
		double probs; 			// probability of being trapped in vortex
		double d;               // reward scale parameter
		
		// local random generator, faster for parallel computing
		std::mt19937 local_rng; 
		
		// transition matrix for wind direction
		float wind_transition[DIMWIND][DIMWIND] = {
			{0.3, 0.2, 0.1, 0.04, 0.02, 0.04, 0.1, 0.2},
			{0.2, 0.3, 0.2, 0.1, 0.04, 0.02, 0.04, 0.1},
			{0.1, 0.2, 0.3, 0.2, 0.1, 0.04, 0.02, 0.04},
			{0.04, 0.1, 0.2, 0.3, 0.2, 0.1, 0.04, 0.02},
			{0.02, 0.04, 0.1, 0.2, 0.3, 0.2, 0.1, 0.04},
			{0.04, 0.02, 0.04, 0.1, 0.2, 0.3, 0.2, 0.1},
			{0.1, 0.04, 0.02, 0.04, 0.1, 0.2, 0.3, 0.2},
			{0.2, 0.1, 0.04, 0.02, 0.04, 0.1, 0.2, 0.3}
		};
	
	public:
		
		void setValues(Params* params){
			DIMX = (int)sqrt(params->len_state/DIMWIND);
			DIMY = DIMX;
			GOALX = (int)DIMX/2; // the target place is the center of the grid
			GOALY = GOALX;
			probs = params->probs;
			d = params->d;
			std::random_device rd; 
			local_rng.seed(rd());
		}
		
		double localNormalDouble(double mean, double sd){ 
			std::normal_distribution<double> normal(mean, sd);
			return normal(local_rng);
		}
		
		double localUniformDouble(double start, double end){
			std::uniform_real_distribution<double> unif(start, end);
			return unif(local_rng);
		}
		
		
		// map the ith state to position and wind
		void indexToState(int index){
			wind = index / (DIMX * DIMY);
			x = (index - DIMX * DIMY * wind) / DIMY;
			y = index - DIMX * DIMY * wind - DIMY * x;
		}
		
		// map position and wind to the ith state
		int stateToIndex(){
			return wind * DIMX * DIMY + x * DIMY + y;
		}
		
		// actions
		std::pair<int, int> direction(int a){
			switch( a ) {
				case 0: return std::make_pair(0, 1);
				case 1: return std::make_pair(1, 1);
				case 2: return std::make_pair(1, 0);
				case 3: return std::make_pair(1, -1);
				case 4: return std::make_pair(0, -1);
				case 5: return std::make_pair(-1, -1);
				case 6: return std::make_pair(-1, 0);
				case 7: return std::make_pair(-1, 1);
				default: return std::make_pair(-1, -1);
			}
		}
		
		// apply action to current state 
		void apply(int a){
			std::pair<int, int> dir = direction(a);
			x = max(0, min(x + dir.first, DIMX-1));
			y = max(0, min(y + dir.second, DIMY-1));
			
			// some noise in positioning
			// simulate wind
			x = max(0, min(x + (int)localNormalDouble(0.,0.1), DIMX-1));
			y = max(0, min(y + (int)localNormalDouble(0.,0.1), DIMY-1));

			// simulate vortex
			if(localUniformDouble(0,1) < probs){
				x = max(0, min(x + (int)localNormalDouble(0.,1.), DIMX-1));
				y = max(0, min(y + (int)localNormalDouble(0.,1.), DIMY-1));
			}
		}
		
		// instant reward
		double reward(int a){
			if( x == GOALX && y == GOALY){
				return 1.;
			}
			else if(x==0 & y==0)
				return 0.;
			else{	
				int angle = abs(a - wind);
				angle = angle < 8 - angle ? angle : 8 - angle;
				return angle * d;
			}
		}
		
		void windTransition(){
			double prob = localUniformDouble(0, 1);
			double start = 0;
			for(int nwind = 0; nwind < DIMWIND; nwind++){
				start += wind_transition[wind][nwind];
				if(start > prob){
					wind = nwind;
					break;
				}
			}
		}		
		
		// sample oracle function: given init_state[i], init_action[a], rewrite next_state[j] and reward[r]
		void SO(int i, int a, int& j, double& r){
			indexToState(i);
			apply(a);
			r = reward(a);
			windTransition();
			j = stateToIndex();
		}
		
};


// policy evaluation
void test_sailing(Sailing& s, std::vector<int>* pi, Params* params){
	
	double start_time = get_wall_time();
	s.setValues(params);
	// total discounted reward
	double total_reward = 0.;
	// how many times has the goal state been reached.
	int flag = 0;
	int isflag = 0;
	// run text_max_episode episodes
	for (int episode = 0; episode < params->test_max_episode; episode++){
		// start from an arbitrary state
		int i = uniformInt(0,params->len_state-1);
		int j = 0;
		double r = 0;
		isflag = 0;
		for (int step = 0; step < params->test_max_step; step++){
			s.SO(i,(*pi)[i],j,r);
			total_reward += pow(params->gamma,step)*r;
			if(r == 1 && !isflag){
				flag += 1;
				isflag = 1;
			}
			i = j;
		}
	}
	params->test_time += get_wall_time() - start_time;
	// average total reward
	total_reward /= params->test_max_episode;
	cout<<get_wall_time()-params->test_time-params->time<<' '<<total_reward<<' '<<flag<<endl;
	return;
}
#endif
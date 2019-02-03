# User Guide

A light c++11 package for three reinforment learning algorithms under generative model:
- AsyncQVI https://arxiv.org/abs/1812.00885
- AsyncQL  http://www.mit.edu/~jnt/Papers/J052-94-jnt-q.pdf
- VRVI     https://arxiv.org/abs/1710.09988

where the first two are implemented in asynchronous parallel fashion and the last one is single-threaded. Users should read papers for algorithm descriptions.

## Usage
We implement parallel computing with c++11 and the <pthread.h> lib. A gcc 4.8+ compiler is required. One can either run on Linux or Windows with MinGW (http://www.math.ucla.edu/~wotaoyin/windows_coding.html). After downloading the package, redirect to where the makefile locates. To compile, insert the command

    make

To execute, insert the command

    make run

## Parameter Settings
Users can set parameter abc with value xyz either in util.h -> struct Params (recompile after modification) or in makefile (Line 38) with 

    -abc xyz 
The parameter dictionary for three algorithms are listed below:

### General ###
Name | Meaning
-----|--------
params.len_state| dimension of state space
params.len_action| dimension of action space
params.gamma | discounted factor
params.algo | algorithm (0: AsyncQVI, 1: AsyncQL, 2: VRVI)
params.style | sample style (0: uniformly random, 1: globally cyclic, 2: Markovian)
params.total_num_threads | total number of parallel threads
params.check_step | how often to evaluate policy while running
params.save | save final policy in file (0: no, 1: yes)
params.test_max_episode | number of episodes for testing
params.test_max_step | number of steps to go in one test episode


### AsyncQVI ###
  Paper | Code
  ------|------
  L     | params.max_outer_iter
  K     | params.max_inner_iter
  epsilon | params.epsilon
  
### AsyncQL ###
  Paper | Code
  ------|------
  alpha(learning rate) | params.alpha
  maximal iterations | params.max_outer_iter
  
### VRVI ###
  Paper | Code
  ------|------
  m | params.sample_num
  L | params.max_inner_iter
  K | params.max_outer_iter
  
## Sample Oracle
A sample oracle should be defined separately as a class structure in one header file and included by algo.h to have the algorithms take samples. For instance, we built one sample oracle, a sailing problem, in oracle.h for testing. Users can follow the same structure.

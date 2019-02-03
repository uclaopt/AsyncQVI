# User Guide

This is a C++11 package for three reinforcement learning algorithms that call use a sample oracle (also known as a generative model):
- AsyncQVI: [AsyncQVI: Asynchronous-Parallel Q-Value Iteration for Reinforcement Learning with Near-Optimal Sample Complexity.](https://arxiv.org/abs/1812.00885) by Yibo Zeng, Fei Feng, Wotao Yin
- VRVI:    [Variance Reduced Value Iteration and Faster Algorithms for Solving Markov Decision Process.](https://arxiv.org/abs/1710.09988) by Aaron Sidford, Mengdi Wang, Xian Wu, Yinyu Ye
- AsyncQL: [Asynchronous Stochastic Approximation and Q-Learning.](http://www.mit.edu/~jnt/Papers/J052-94-jnt-q.pdf) by John N. Tsitsiklis

The first and last algorithms (AsyncQVI and AsyncQL) are implemented in the asynchronous parallel fashion. The second one (VRVI) is single-threaded. 

## Install
We implemented parallel computing in C++11 using the pthread lib and <pthread.h>. A gcc (version 4.8+) compiler is required. 

This package can be compiled on either Linux or Windows. On Windows, one can use MinGW and follow the instructions in  http://www.math.ucla.edu/~wotaoyin/windows_coding.html. 

To compile, run the command

    make

## Usage

To run a demo, call

    make run

It runs an executable with the following  parameters, specified on Line 38 of makefile,

    -algo 0 -nthreads 4 -check_step 100000 -style 1 -len_state 20000 -len_action 8 -max_outer_iter 10000000 -max_inner_iter 5

The demo solves the sailing reinforcement learning problem with 20000 states and 8 actions using AsyncQVI (-algo 0) with 4 parallel threads (-nthreads 4), globally cyclic sampling (-style 1), and 5 samples per approximation (-max_inner_iter 5) for maximally 1000000 iterations (-max_outer_iter 1000000). 

You can change these parameters and also try the other two algorithms. The supported parameter settings are given below.

## Parameter Settings
There are 15 parameters. Users can set their values either in util.h -> struct Params (then you must recompile after each modification), or modify Line 38 of makefile, or use command-line options like

    -abc xyz 

The supported parameter settings are given below.

### Common for all three algorithms ###
Name | Description
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


### AsyncQVI specific ###
  Name (in paper) | Field (in code)
  ------|------
  L     | params.max_outer_iter
  K     | params.max_inner_iter
  epsilon | params.epsilon
  
### AsyncQL specific ###
  Name (in paper) | Field (in code)
  ------|------
  alpha (learning rate) | params.alpha
  maximal iterations | params.max_outer_iter

To use adaptive learning rate, one should tune by modifying algo.h, Line 139. 
To use constant learning rate, one can comment Line 135 and tune in makefile (-alpha) without recompiling.

### VRVI specific ###
  Name (in paper) | Field (in code)
  ------|------
  m | params.sample_num
  L | params.max_inner_iter
  K | params.max_outer_iter
  
## Sample Oracle
All the three algorithms call an oracle that takes samples. Therefore, a sample oracle (as a class structure) must be defined in a header file and included in algo.h. For the sailing problem, we built a sample oracle in oracle.h. The user can use it as a template to run the three algorithms with their own sample oracles.

# User Guide

A light c++11 package for three reinforment learning algorithms under generative model:
- AsyncQVI: [AsyncQVI: Asynchronous-Parallel Q-Value Iteration for Reinforcement Learning with Near-Optimal Sample Complexity.](https://arxiv.org/abs/1812.00885) Yibo Zeng, Fei Feng, Wotao Yin
- AsyncQL: [Asynchronous Stochastic Approximation and Q-Learning.](http://www.mit.edu/~jnt/Papers/J052-94-jnt-q.pdf) John N. Tsitsiklis
- VRVI:     [Variance Reduced Value Iteration and Faster Algorithms for Solving Markov Decision Process.](https://arxiv.org/abs/1710.09988) Aaron Sidford, Mengdi Wang, Xian Wu, Yinyu Ye

where the first two are implemented in asynchronous parallel fashion and the last one is single-threaded. Users should read papers for algorithm descriptions.

## Usage
We implement parallel computing with c++11 and the <pthread.h> lib. A gcc 4.8+ compiler is required. One can either run on Linux or Windows with MinGW (http://www.math.ucla.edu/~wotaoyin/windows_coding.html). After downloading the package, redirect to where the makefile locates. To compile, insert the command

    make

To execute with certain input parameters (see makefile, Line 38), insert the command

    make run

In the current makefile example, we test with parameters

    -algo 0 -nthreads 4 -check_step 100000 -style 1 -len_state 20000 -len_action 8 -max_outer_iter 10000000 -max_inner_iter 5

which represents solving a sailing problem (our test example) with 20000 states and 8 actions using AsyncQVI (-algo 0) with 4 threads ( -nthreads 4), globally cyclic sampling (-style 1), and 5 samples per approximation (-max_inner_iter 5) for at most 1000000 global iterations (-max_outer_iter 1000000). Read below for more details about parameter settings.

## Parameter Settings
There are 15 parameters that can be tuned. Users can set values either in util.h -> struct Params (recompile after modification) or in makefile (Line 38) with 

    -abc xyz 
In the example makefile, we did not list all parameters. Feel free to add by your needs. 

The parameter dictionary for three algorithms are listed below:

### Common for all three algorithms ###
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


### AsyncQVI specific ###
  Paper | Code
  ------|------
  L     | params.max_outer_iter
  K     | params.max_inner_iter
  epsilon | params.epsilon
  
### AsyncQL specific ###
  Paper | Code
  ------|------
  alpha(learning rate) | params.alpha
  maximal iterations | params.max_outer_iter

To use adaptive learning rate, one should tune by modifying algo.h, Line 135. 
To use constant learning rate, one can comment Line 135 and tune in makefile (-alpha) without recompiling.

### VRVI specific ###
  Paper | Code
  ------|------
  m | params.sample_num
  L | params.max_inner_iter
  K | params.max_outer_iter
  
## Sample Oracle
A sample oracle should be defined separately as a class structure in one header file and included by algo.h to have the algorithms take samples. For instance, we built one sample oracle, a sailing problem, in oracle.h for testing. Users can follow the same structure.

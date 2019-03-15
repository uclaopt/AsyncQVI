# User Guide

This is a C++11 package for four reinforcement learning algorithms that call use a sample oracle (also known as a generative model):
- AsyncQVI: [AsyncQVI: Asynchronous-Parallel Q-Value Iteration for Reinforcement Learning with Near-Optimal Sample Complexity.](https://arxiv.org/abs/1812.00885) by Yibo Zeng, Fei Feng, Wotao Yin
- AsyncQL: [Asynchronous Stochastic Approximation and Q-Learning.](http://www.mit.edu/~jnt/Papers/J052-94-jnt-q.pdf) by John N. Tsitsiklis
- VRVI: [Variance Reduced Value Iteration and Faster Algorithms for Solving Markov Decision Process.](https://arxiv.org/abs/1710.09988) by Aaron Sidford, Mengdi Wang, Xian Wu, Yinyu Ye
- VRQVI: [Near-Optimal Time and Sample Complexities for Solving Discounted Markov Decision Process with a Generative Model.](https://arxiv.org/pdf/1806.01492.pdf) by Aaron Sidford, Mengdi Wang, Xian Wu, Lin F. Yang, Yinyu Ye

AsyncQVI and AsyncQL are implemented in the asynchronous parallel fashion. VRQVI and VRVI are single-threaded. 

## Install
We implemented parallel computing in C++11 using the pthread lib and <pthread.h>. A gcc (version 4.8+) compiler is required. 

This package can be compiled on either Linux or Windows. On Windows, one can use MinGW and follow the instructions in  http://www.math.ucla.edu/~wotaoyin/windows_coding.html. 

To compile, run the command

    make

## Usage

To run a demo, call

    make run

It runs an executable with the following  parameters, specified on Line 38 of makefile,

    -algo 0 -nthreads 1 -check_step 100000 -style 1 -len_state 80000 -len_action 8 -max_outer_iter 10000000 -max_inner_iter 1 -sample_num_1 1 -sample_num_2 1

The demo solves the sailing reinforcement learning problem with 80000 states and 8 actions using AsyncQVI (-algo 0) with 1 parallel threads (-nthreads 1), globally cyclic sampling (-style 1), and 1 samples per approximation (-max_inner_iter 1) for maximally 10000000 iterations (-max_outer_iter 10000000). 

You can change these parameters and also try the other two algorithms. The supported parameter settings are given below.

## Parameter Settings
There are 20 parameters. Users can set their values either in util.h -> struct Params (then you must recompile after each modification), or modify Line 38 of makefile, or use command-line options like

    -abc xyz 

The supported parameter settings are given below.

### Common for all three algorithms ###
Name | Description
-----|--------
params.len_state| dimension of state space
params.len_action| dimension of action space
params.gamma | discounted factor
params.algo | algorithm (0: AsyncQVI, 1: AsyncQL, 2: VRVI, 3: VRQVI)
params.style | sample style (0: uniformly random, 1: globally cyclic, 2: Markovian)
params.total_num_threads | total number of parallel threads
params.check_step | how often to evaluate policy while running
params.save | save final policy in file (0: no, 1: yes)
params.test_max_episode | number of episodes for testing
params.test_max_step | number of steps to go in one test episode


### AsyncQVI specific ###
  Name (in paper) | Field (in code)
  ------|------
  L (Alg.2) | params.max_outer_iter
  K (Alg.3) | params.max_inner_iter
  epsilon (Alg.2) | params.epsilon
  
### AsyncQL specific ###
  Name (in paper) | Field (in code)
  ------|------
  alpha (learning rate) | params.alpha
  maximal iterations | params.max_outer_iter

To use adaptive learning rate, one should tune by modifying algo.h, Line 139. 
To use constant learning rate, one can comment Line 135 and tune in makefile (-alpha) without recompiling.

### VRVI specific ###
  Name (in paper) | Field (in code)
  ------| ------
  number of samples used per state-action to approximate \tilde{x} (Alg.8) | params.sample_num_1
  m (Alg.1) | params.sample_num_2
  T (Alg.8)| params.max_inner_iter
  K (Alg.9)| params.max_outer_iter
  epsilon (Alg.7) | params.epsilon
  
### VRQVI specific ###
  Name (in paper) | Field (in code)
  ------| ------
  m_1 (Alg.1) | params.sample_num_1
  m_2 (Alg.1) | params.sample_num_2
  R (Alg.1)| params.max_inner_iter
  R (Alg.2)| params.max_outer_iter
  alpha_1 (Alg.1) | params.alpha1
  
## Sample Oracle
All the four algorithms call an oracle that takes samples. Therefore, a sample oracle (as a class structure) must be defined in a header file and included in algo.h. For the sailing problem, we built a sample oracle in oracle.h. The user can use it as a template to run the three algorithms with their own sample oracles.

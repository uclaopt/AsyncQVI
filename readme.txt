A light c++11 package for three reinforment learning algorithms under generative model:

AsyncQVI, AsyncQL, and VRVI,

where the first two are implemented in asynchronous parallel fashion and the last one is single-threaded. Find our paper at 

https://arxiv.org/abs/1812.00885. 

Users should read the paper for algorithm descriptions. Parameters are defined in util.h -> struct Params. One can either set parameter value in util.h or in makefile.

As for sample oracles, we seperate them from RL algorithms. A sample oracle should be defined independently as a class structure in one header file and included by algo.h to have the algorithms take samples. For instance, we built one sample oracle, a sailing problem, in oracle.h for testing. Users can follow the same structure as in oracle.h.

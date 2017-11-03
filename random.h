#include <time.h>

#ifndef RANDOM_H
#define RANDOM_H

#define OPEN_INTERVAL 0
#define CLOSED_INTERVAL 1
#define LEFT_OPEN_RIGHT_CLOSED_INTERVAL 2
#define LEFT_CLOSED_RIGHT_OPEN_INTERVAL 3

/** 启动随机数生成器 */
void start_up_random();

/**
生成（0, 1）之间的随机数
*/
double ran();

/**
产生一个随机数种子，该种子用于生成一个新随机数
*/
unsigned generate_seed();

/**
生成一个位于区间{ [low, high], (low, high), (low, high], [low, high)}之间的随机数
mode = 0, 开区间
mode = 1, 闭区间
mode = 2, 左开右闭区间
mode = 3, 左闭右开区间
默认返回（0，1）之间的随机数
*/
double random_in_interval(double low, double high, int mode);
/** 返回{ [low, high], (low, high), (low, high], [low, high)}之间的随机整数 */
int integer_random_in_interval(int low, int high, int mode);

/* Function declarations for the random number generator */
void randomize();
void warmup_random(long double seed);
void advance_random();
long double randomperc();
int rnd(int low, int high);
long double rndreal(long double low, long double high);


/** 生成一个随机数序列服从正太分布N(mean_value, variance)的随机数 */
double gaussrand(double mean_value, double variance);
/** 生成一个随机数序列服从柯西分布Cauchy(alpha, beta)的随机数 */
double cauchyrnd(double alpha, double beta);
#endif
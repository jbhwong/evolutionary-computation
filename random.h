#include <time.h>

#ifndef RANDOM_H
#define RANDOM_H

#define OPEN_INTERVAL 0
#define CLOSED_INTERVAL 1
#define LEFT_OPEN_RIGHT_CLOSED_INTERVAL 2
#define LEFT_CLOSED_RIGHT_OPEN_INTERVAL 3

/** ��������������� */
void start_up_random();

/**
���ɣ�0, 1��֮��������
*/
double ran();

/**
����һ����������ӣ���������������һ���������
*/
unsigned generate_seed();

/**
����һ��λ������{ [low, high], (low, high), (low, high], [low, high)}֮��������
mode = 0, ������
mode = 1, ������
mode = 2, ���ұ�����
mode = 3, ����ҿ�����
Ĭ�Ϸ��أ�0��1��֮��������
*/
double random_in_interval(double low, double high, int mode);
/** ����{ [low, high], (low, high), (low, high], [low, high)}֮���������� */
int integer_random_in_interval(int low, int high, int mode);

/* Function declarations for the random number generator */
void randomize();
void warmup_random(long double seed);
void advance_random();
long double randomperc();
int rnd(int low, int high);
long double rndreal(long double low, long double high);


/** ����һ����������з�����̫�ֲ�N(mean_value, variance)������� */
double gaussrand(double mean_value, double variance);
/** ����һ����������з��ӿ����ֲ�Cauchy(alpha, beta)������� */
double cauchyrnd(double alpha, double beta);
#endif
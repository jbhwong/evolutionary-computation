#include <stdlib.h>
#include <math.h>
#include "comm_operation.h"
#include "random.h"

void start_up_random()
{
	srand(time(NULL));
	randomize();
}

/**
	ԭ�������������ֻ������0��1֮����������
	��������������ɵ����������ܶȽϵͣ�������������Ϊ10^4

	������������ᵼ��������ֲ�������
	��1��ran_num = rand() * 1.0 / 0xffffffff;
	��2��ran_num = rand() * 1.0 / 0x80;
	��1���ᵼ��������ֲ���С����2���ᵼ��������ֲ�����
*/
double ran()
{
	double ran_num;
	do
	{
		ran_num = rand() / (RAND_MAX + 1.0);
	} while (ran_num == 0 || ran_num == 1);
	return ran_num;
}

/**
	��int���ֽ���ѭ����λ
	mode = 0, ѭ������
	mode = 1, ѭ������
	С��C���Ե���λ���ԣ�����һ����0������һ��������λ
*/
#define LEFT_MOVE 0
#define RIGHT_MOVE 1
unsigned move_bit_loop(unsigned number, int move_bit, int mode)
{	
	switch (mode)
	{
		case LEFT_MOVE:
			for (int i = 1; i <= move_bit; i++)
			{
				if (number & 0x80000000)  //���λΪ1
				{
					number <<= 1;
					number += 1;
				}
				else  //���λΪ0
				{
					number <<= 1;
				}
			}
			break;
		case RIGHT_MOVE:
			for (int i = 1; i <= move_bit; i++)
			{
				if (number & 0x00000001)  //���λΪ1
				{
					number >>= 1;
					number |= 0x80000000;
				}
				else  //���λΪ0
				{
					number >>= 1;
					number &= 0x7fffffff;
				}
			}
			break;
		default:break;
	}
	
	return number;
}

/**
	����һ�����ӣ������Ǹ������
	���ӵĳ�ʼֵΪ�������еĳ�ʼʱ�䣨��ȷ��΢�룩
	����ÿһ�ε�����ֵΪ��һ�ε�����ֵ�������±仯��
	��1�������������λ��Ϊ1
	��2�������������λ��Ϊ0
	��3��ѭ���ƶ�kλ��0 <= k < 32��
*/
unsigned turn_one[32] = { 0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 },
turn_zero[32] = { 0xfffffffe, 0xfffffffd, 0xfffffffb, 0xfffffff7, 0xffffffef, 0xffffffdf, 0xffffffbf, 0xffffff7f,
0xfffffeff, 0xfffffdff, 0xfffffbff, 0xfffff7ff, 0xffffefff, 0xffffdfff, 0xffffbfff, 0xffff7fff,
0xfffeffff, 0xfffdffff, 0xfffbffff, 0xfff7ffff, 0xffefffff, 0xffdfffff, 0xffbfffff, 0xff7fffff,
0xfeffffff, 0xfdffffff, 0xfbffffff, 0xf7ffffff, 0xefffffff, 0xdfffffff, 0xbfffffff, 0x7fffffff };
unsigned generate_seed()
{
	static unsigned seed = time(NULL);  //�þ�ֵ̬�ǵ�seedֻ�ᱻ��ʼ��һ��
	int turn_index[4];
	for (int i = 0; i < 4; i++)
	{
		turn_index[i] = rand() % 32;
	}
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{
			seed |= turn_one[turn_index[i]];
		}
		else
		{
			seed &= turn_zero[turn_index[i]];
		}
	}
	int move_bit = rand() % 32;
	seed = move_bit_loop(seed, move_bit, LEFT_MOVE);

	return seed;
}

/**
����һ��λ������[-radius, radius]֮��������
���ܼ�����洢�ִ�С���޵�Ӱ�죬���ܲ�����
������ֵ��ܸ����Ǻ㶨����ģ�ֻ���������������
������ֵķֲ��ܶ��½��˶��ѣ�
*/
#define MAX_RAND 0xffffffff
double random(double radius)
{
	int flag = (rand() % 2 == 0) ? 1 : -1;  //���ѡȡ�������ֵ�������
	unsigned seed = generate_seed();  //����������
	int move_bit = rand() % 32;  //����ƶ���λ��

	seed = move_bit_loop(seed, move_bit, RIGHT_MOVE);
	double random = seed * 1.0 / MAX_RAND;   //���һ�����С������ͨ���뾶������ֲ���Χ

	return flag * random * radius;
}

/**
	�°����������
	��������������ɵ����������ܶȽϸߣ�������������Ϊ10^9��
	��������ĳ�����䷶Χ�ڵ������
*/
double random_in_interval(double low, double high, int mode)
{
	double radius = (high - low) / 2.0, deviation = high - radius;
	double random_number;
	switch (mode)
	{
	case OPEN_INTERVAL:
		do
		{
			random_number = random(radius);
		} while (isEqual(random_number, -1 * radius, 0) || isEqual(random_number, radius, 0));
		random_number += deviation;
		break;
	case CLOSED_INTERVAL:
		random_number = random(radius);
		random_number += deviation;
		break;
	case LEFT_OPEN_RIGHT_CLOSED_INTERVAL:
		do
		{
			random_number = random(radius);
		} while (isEqual(random_number, -1 * radius, 0));
		random_number += deviation;
		break;
	case LEFT_CLOSED_RIGHT_OPEN_INTERVAL:
		do
		{
			random_number = random(radius);
		} while (isEqual(random_number, radius, 0));
		random_number += deviation;
		break;
	default:
		random_number = random(1.0);
		break;
	}

	return random_number;
}

int integer_random_in_interval(int low, int high, int mode)
{
	return (int)(random_in_interval(low, high, mode) + 0.5);
}

/* Variable declarations for the random number generator */
long double seed;
long double oldrand[55];
int jrand;
int rndcalcflag;
long double rndx1, rndx2;

/* Get seed number for random and start it up */
void randomize()
{
	int j1;
	for (j1 = 0; j1 <= 54; j1++)
	{
		oldrand[j1] = 0.0;
	}
	jrand = 0;
	warmup_random(seed);
	return;
}

/* Get randomize off and running */
void warmup_random(long double seed)
{
	int j1, ii;
	long double new_random, prev_random;
	oldrand[54] = seed;
	new_random = 0.000000001;
	prev_random = seed;
	for (j1 = 1; j1 <= 54; j1++)
	{
		ii = (21 * j1) % 54;
		oldrand[ii] = new_random;
		new_random = prev_random - new_random;
		if (new_random<0.0)
		{
			new_random += 1.0;
		}
		prev_random = oldrand[ii];
	}
	advance_random();
	advance_random();
	advance_random();
	jrand = 0;
	return;
}

/* Create next batch of 55 random numbers */
void advance_random()
{
	int j1;
	long double new_random;
	for (j1 = 0; j1<24; j1++)
	{
		new_random = oldrand[j1] - oldrand[j1 + 31];
		if (new_random<0.0)
		{
			new_random = new_random + 1.0;
		}
		oldrand[j1] = new_random;
	}
	for (j1 = 24; j1<55; j1++)
	{
		new_random = oldrand[j1] - oldrand[j1 - 24];
		if (new_random<0.0)
		{
			new_random = new_random + 1.0;
		}
		oldrand[j1] = new_random;
	}
}

/* Fetch a single random number between 0.0 and 1.0 */
long double randomperc()
{
	jrand++;
	if (jrand >= 55)
	{
		jrand = 1;
		advance_random();
	}
	return((long double)oldrand[jrand]);
}

/* Fetch a single random integer between low and high including the bounds */
int rnd(int low, int high)
{
	int res;
	if (low >= high)
	{
		res = low;
	}
	else
	{
		res = low + (randomperc()*(high - low + 1));
		if (res > high)
		{
			res = high;
		}
	}
	return (res);
}

/* Fetch a single random real number between low and high including the bounds */
long double rndreal(long double low, long double high)
{
	return (low + (high - low)*randomperc());
}



/**
	���ɷ��Ӹ�˹�ֲ�����̫�ֲ��������������
	mean_valueΪ��ֵ��varianceΪ����
	��ҪΪ[-3, 3]֮��������
*/
double gaussrand(double mean_value, double variance)
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0)
	{
		do{
			double U1 = rand() * 1.0 / RAND_MAX;
			double U2 = rand() * 1.0 / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;

			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);
		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	//X = X * sqrt(variance) + mean_value;  //�˴��˱�׼���������׼����ǲ����뾶
	X = X * variance + mean_value;

	return X;
}

/**
	���ɷ��ӿ����ֲ������������
	alpha-λ�ò�����beta-�߶Ȳ���
*/
double cauchyrnd(double alpha, double beta)
{
	double u, y;

	u = random_in_interval(0, 1, CLOSED_INTERVAL);
	y = alpha + beta * tan(acos(-1.0) * (u - 0.5));

	return(y);
}
#include "comm_operation.h"
#include <math.h>

using namespace std;

bool isEqual(double n1, double n2, double error)  //判断两个浮点数是否相等，最好使用误差法判断，否则因为计算机自身计算误差等原因导致相等条件要求很高
{
	if (error <= 0)
		return n1 == n2;
	else
	{
		return fabs(n1 - n2) < error;
	}
}

bool is_equal_vector(vector<double> *a, vector<double> *b)
{
	bool is_equal = true;
	if (a->size() != b->size())
	{
		is_equal = false;
	}
	else
	{
		for (int i = 0; i < a->size(); i++)
		{
			if (!isEqual((*a)[i], (*b)[i], 1.0e-9))
			{
				is_equal = false;
			}
		}
	}
	return is_equal;
}

void copy_vector(vector<double> *a, vector<double> *b)
{
	int size = a->size() < b->size() ? a->size() : b->size();
	int devation = a->size() - b->size(), index = 0;
	devation = abs(devation);

	while(index < size)
	{
		(*a)[index] = (*b)[index];
		index++;
	}
	if(a->size() < b->size())
	{
		for(int i=1; i <= devation; i++)
		{
			a->push_back((*b)[index++]);
		}
	}
	else if(a->size() > b->size())
	{
		for(int i=1; i <= devation; i++)
		{
			a->pop_back();
		}
	}
}

bool is_equal_array(double* a, double* b, int a_size, int b_size, double error)
{
	if (a_size != b_size)
		return false;
	else
	{
		bool is_equal = true;
		for (int i = 0; i < a_size; i++)
		{
			if (!isEqual(a[i], b[i], error))
			{
				is_equal = false;
				break;
			}
		}
		return is_equal;
	}
}

void copy_array(double* a, double* b, int a_size, int b_size)
{
	int size = a_size < b_size ? a_size : b_size;
	for (int i = 0; i < size; i++)
	{
		a[i] = b[i];
	}
}

void exchange_array(double* a, double* b, int size)
{
	for (int i = 0; i < size; i++)
	{
		double t = a[i];
		a[i] = b[i];
		b[i] = t;
	}
}

int min_index_among_three_value(double v1, double v2, double v3)
{
	if (v1 < v2)
	{
		if (v1 < v3)
		{
			return 1;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		if (v2 < v3)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
}

double sgn(double value)
{
	if (value < 0)
		return -1;
	else if (value == 0)
		return 0;
	else
		return 1;
}
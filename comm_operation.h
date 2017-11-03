#include <vector>

using namespace std;

#ifndef COMM_OPERATION_H
#define COMM_OPERATION_H

/** �ж����������Ƿ���� */
bool is_equal_vector(vector<double> *a, vector<double> *b);

/** ������b�����ݿ���������a����������Ժ�a����b�ȼ� */
void copy_vector(vector<double> *a, vector<double> *b);

/** ��������������Сֵ���±� */
int min_index_among_three_value(double v1, double v2, double v3);

/** ���ݾ�������ж����������Ƿ���� */
bool isEqual(double n1, double n2, double error);

/** �ж����������������ʾ���Ƿ���� */
bool is_equal_array(double* a, double* b, int a_size, int b_size, double error);

/** ������b�����ݿ���������a�������ʾ����a�Ĵ洢�ռ䲻�㣬��ֻ�洢b��һ���֣� */
void copy_array(double* a, double* b, int a_size, int b_size);

/** �����������ݣ������ʾ�� */
void exchange_array(double* a, double* b, int size);

/** �����źź��� */
double sgn(double value);
#endif
#include <vector>

using namespace std;

#ifndef COMM_OPERATION_H
#define COMM_OPERATION_H

/** 判断两个向量是否相等 */
bool is_equal_vector(vector<double> *a, vector<double> *b);

/** 将向量b的内容拷贝至向量a，操作完成以后a将与b等价 */
void copy_vector(vector<double> *a, vector<double> *b);

/** 返回三个数中最小值的下标 */
int min_index_among_three_value(double v1, double v2, double v3);

/** 根据精度误差判断两个数字是否相等 */
bool isEqual(double n1, double n2, double error);

/** 判断两个向量（数组表示）是否相等 */
bool is_equal_array(double* a, double* b, int a_size, int b_size, double error);

/** 将向量b的内容拷贝至向量a（数组表示，若a的存储空间不足，则只存储b的一部分） */
void copy_array(double* a, double* b, int a_size, int b_size);

/** 交换数组内容（数组表示） */
void exchange_array(double* a, double* b, int size);

/** 计算信号函数 */
double sgn(double value);
#endif
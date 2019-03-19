#pragma OPENCL EXTENSION all : enable

__attribute__ ((reqd_work_group_size(1, 1, 1)))
__kernel void simple_add(global const int* A, global const int* B, global int* C)
{
  int id0 = get_global_id(0);
  int id1 = get_global_id(1);
  int id2 = get_global_id(2);

  C[id0 + id1*6 + id2*36] = A[id0 + id1*6 + id2*36] + B[id0 + id1*6 + id2*36];
}

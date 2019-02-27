#pragma OPENCL EXTENSION all : enable

__attribute__ ((reqd_work_group_size(12, 1, 1)))
__kernel void simple_add(global const int* A, global const int* B, global int* C)
{
  int id         = get_global_id(0);
  int group_size = get_local_size(0);

  int i;
  for (i = group_size * id; i < group_size * (id + 1); i++)
    C[i] = A[i] + B[i];
}

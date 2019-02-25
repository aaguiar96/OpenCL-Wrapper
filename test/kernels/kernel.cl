#pragma OPENCL EXTENSION all : enable

void kernel simple_add(global const int* A, global const int* B, global int* C)
{
  int id         = get_global_id(0);
  int group_size = get_local_size(0);

  for (int i = group_size * id; i < group_size * (id + 1); i++)
    C[i]     = A[i] + B[i];
}

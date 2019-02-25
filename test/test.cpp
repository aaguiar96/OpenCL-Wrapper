#include "../include/cl_wrapper.hpp"

using namespace cl_wrapper;

int main()
{
  ClContainer cl_container;

  std::vector<std::string> paths;
  paths.push_back("/home/pi/tools/cl/test/kernels/kernel.cl");
  cl_container.init(paths);
  cl_container.program(paths[0], cl_container.prog_sources[0]);
  
  int N[1] = {100};
  int n    = N[0];

  Buffer buffer_a = Buffer(cl_container, CL_MEM_READ_WRITE, n * sizeof(int), 0);
  Buffer buffer_b = Buffer(cl_container, CL_MEM_READ_WRITE, n * sizeof(int), 0);
  Buffer buffer_c = Buffer(cl_container, CL_MEM_READ_WRITE, n * sizeof(int), 0);
  

  std::vector<cl::Buffer> buffers;
  buffers.push_back(buffer_a.buffer);
  buffers.push_back(buffer_b.buffer);
  buffers.push_back(buffer_c.buffer);

  std::vector<int> A, B;
  for (int i = 0; i < n; i++) {
    A.push_back(i);	  
    B.push_back(i);	  
  }

  buffer_a.write(&A[0]);
  buffer_b.write(&B[0]);

  Kernel kernel = Kernel(cl_container, cl_container.progs[paths[0]], "simple_add", 0, 12,
                         12, buffers, 0);
  kernel.exec();
  

  std::vector<int> C(n);
  buffer_c.read(&C[0]);
  
  std::cout << "result: {";
  for (int i = 0; i < C.size(); i++) {
    std::cout << C[i] << " ";
  }
  std::cout << "}" << std::endl;
  
}

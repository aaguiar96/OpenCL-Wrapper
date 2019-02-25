#pragma once

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include <fstream>
#include <iostream>
#include <map>

namespace cl_wrapper {

class ClContainer;
class Buffer;
class Kerner;

class ClContainer
{
private:
  cl::Platform m_platform;
  cl::Device   m_device;
  int          nProgs;

public:
  cl::Context              context;
  std::vector<std::string> prog_sources;
  std::vector<cl::CommandQueue> queues;

  /* Program association with its path. */
  std::map<std::string, cl::Program> progs;
  /* Program association with its id */
  std::map<std::string, unsigned int> progs_id;

  /* Chooses the platform, device and initializes the context. */
  /* The argument is a vector containing the path for all the kernel .cl files
   */
  void init(std::vector<std::string> paths);

  /* Creates and builds a program. */
  void program(std::string path, std::string prog_source);

  /* Cleans the allocated memory using OpenCL. */
  void clean_mem();

  /* Checks for errors and specifies its occurrence. */
  void error_handler(cl_int error);

  /* Reads the kernel source text from a .cl file. */
  void read_source(std::string path, std::string* source_code);
};

class Buffer
{
private:
  ClContainer  m_container;
  size_t       m_size;

public:
  cl::Buffer buffer;
  unsigned int queue_id;

  Buffer(ClContainer container, cl_mem_flags flags, unsigned int size,
         unsigned int id)
      : m_container(container), m_size(size),
        queue_id(id)
  {
    cl_int error;
    buffer = cl::Buffer(m_container.context, flags, m_size, NULL, &error);
    m_container.error_handler(error);
  }

  /* Passes an argument to the kernel. */
  /* data is a pointer to the memory address where the data is written */
  /* e.g. of usage write(&vec[0]) */
  cl::Event write(const void* data, const std::vector<cl::Event> ev_list);

  /* Reads the output data from a kernel. */
  /* data is a pointer to the memory address where the data is read */
  /* e.g. of usage read(&vec[0]) */
  cl::Event read(void* data, const std::vector<cl::Event> ev_list);

  /* Clear buffer  memory address. */
  void clean();
};

class Kernel
{
private:
  ClContainer  m_container;
  size_t       m_offset;
  size_t       m_globalsize;
  size_t       m_localsize;
  unsigned int m_queue_id;
  cl::Kernel   m_kernel;

public:
  Kernel(ClContainer container, cl::Program program, std::string name,
         size_t offset, size_t global_size, size_t local_size,
         std::vector<cl::Buffer> buffers, unsigned int id)
      : m_container(container), m_offset(offset), m_globalsize(global_size),
        m_localsize(local_size), m_queue_id(id)
  {
    cl_int error;
    m_kernel = cl::Kernel(program, name.c_str(), &error);
    m_container.error_handler(error);

    for (int i = 0; i < buffers.size(); i++) m_kernel.setArg(i, buffers[i]);
  }

  /* Executes the kernel program. */
  cl::Event exec(const std::vector<cl::Event> ev_list);

  /* Clear kernel memory address. */
  void clean();
};
};

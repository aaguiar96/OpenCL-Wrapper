#include "../include/cl_wrapper.hpp"

namespace cl_wrapper {
void ClContainer::init(std::vector<std::string> paths)
{
  cl_int                    error;
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  nProgs = 0;

  if (platforms.size() <= 0) {
    std::cerr << "No platforms found." << std::endl;
    exit(1);
  }
  m_platform = platforms[0];
  std::cout << "Using platform " << m_platform.getInfo<CL_PLATFORM_NAME>()
            << std::endl;

  std::vector<cl::Device> devices;
  m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
  if (devices.size() <= 0) {
    std::cerr << "No devices found." << std::endl;
    exit(1);
  }
  m_device = devices[0];
  std::cout << "Using device " << m_device.getInfo<CL_DEVICE_NAME>()
            << std::endl;

  context = cl::Context({m_device}, NULL, NULL, NULL, &error);
  error_handler(error);

  std::string source_code;
  for (auto& p : paths) {
    read_source(p, &source_code);
    prog_sources.push_back(source_code);
  }
}

void ClContainer::program(std::string path, std::string prog_source)
{
  cl::Program::Sources source;
  source.push_back({prog_source.c_str(), prog_source.size()}); 

  cl::Program prog  = cl::Program(context, source);
  cl_int      error = prog.build({m_device});
  error_handler(error);
  progs[path] = prog;

  cl::CommandQueue queue = cl::CommandQueue(context, m_device, 0, &error);
  error_handler(error);
  queues.push_back(queue);
  progs_id[path] = (nProgs+1);
  nProgs++;
}

void ClContainer::error_handler(cl_int error)
{
  if (error == CL_SUCCESS) return;

  std::string error_message;
  switch (error) {
  case CL_DEVICE_NOT_FOUND:
    error_message = "CL_DEVICE_NOT_FOUND";
    break;
  case CL_DEVICE_NOT_AVAILABLE:
    error_message = "CL_DEVICE_NOT_AVAILABLE";
    break;
  case CL_COMPILER_NOT_AVAILABLE:
    error_message = "CL_COMPILER_NOT_AVAILABLE";
    break;
  case CL_MEM_OBJECT_ALLOCATION_FAILURE:
    error_message = "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    break;
  case CL_OUT_OF_RESOURCES:
    error_message = "CL_OUT_OF_RESOURCES";
    break;
  case CL_OUT_OF_HOST_MEMORY:
    error_message = "CL_OUT_OF_HOST_MEMORY";
    break;
  case CL_PROFILING_INFO_NOT_AVAILABLE:
    error_message = "CL_PROFILING_INFO_NOT_AVAILABLE";
    break;
  case CL_MEM_COPY_OVERLAP:
    error_message = "CL_MEM_COPY_OVERLAP";
    break;
  case CL_IMAGE_FORMAT_MISMATCH:
    error_message = "CL_IMAGE_FORMAT_MISMATCH";
    break;
  case CL_IMAGE_FORMAT_NOT_SUPPORTED:
    error_message = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    break;
  case CL_BUILD_PROGRAM_FAILURE:
    error_message = "CL_BUILD_PROGRAM_FAILURE";
    break;
  case CL_MAP_FAILURE:
    error_message = "CL_MAP_FAILURE";
    break;
  case CL_MISALIGNED_SUB_BUFFER_OFFSET:
    error_message = "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    break;
  case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
    error_message = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    break;
  case CL_COMPILE_PROGRAM_FAILURE:
    error_message = "CL_COMPILE_PROGRAM_FAILURE";
    break;
  case CL_LINKER_NOT_AVAILABLE:
    error_message = "CL_LINKER_NOT_AVAILABLE";
    break;
  case CL_LINK_PROGRAM_FAILURE:
    error_message = "CL_LINK_PROGRAM_FAILURE";
    break;
  case CL_DEVICE_PARTITION_FAILED:
    error_message = "CL_DEVICE_PARTITION_FAILED";
    break;
  case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
    error_message = "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
    break;
  case CL_INVALID_VALUE:
    error_message = "CL_INVALID_VALUE";
    break;
  case CL_INVALID_DEVICE_TYPE:
    error_message = "CL_INVALID_DEVICE_TYPE";
    break;
  case CL_INVALID_PLATFORM:
    error_message = "CL_INVALID_PLATFORM";
    break;
  case CL_INVALID_DEVICE:
    error_message = "CL_INVALID_DEVICE";
    break;
  case CL_INVALID_CONTEXT:
    error_message = "CL_INVALID_CONTEXT";
    break;
  case CL_INVALID_QUEUE_PROPERTIES:
    error_message = "CL_INVALID_QUEUE_PROPERTIES";
    break;
  case CL_INVALID_COMMAND_QUEUE:
    error_message = "CL_INVALID_COMMAND_QUEUE";
    break;
  case CL_INVALID_HOST_PTR:
    error_message = "CL_INVALID_HOST_PTR";
    break;
  case CL_INVALID_MEM_OBJECT:
    error_message = "CL_INVALID_MEM_OBJECT";
    break;
  case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
    error_message = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    break;
  case CL_INVALID_BUILD_OPTIONS:
    error_message = "CL_INVALID_BUILD_OPTIONS";
    break;
  case CL_INVALID_PROGRAM:
    error_message = "CL_INVALID_PROGRAM";
    break;
  case CL_INVALID_BINARY:
    error_message = "CL_INVALID_BINARY";
    break;
  case CL_INVALID_PROGRAM_EXECUTABLE:
    error_message = "CL_INVALID_PROGRAM_EXECUTABLE";
    break;
  case CL_INVALID_KERNEL_NAME:
    error_message = "CL_INVALID_KERNEL_NAME";
    break;
  case CL_INVALID_KERNEL_DEFINITION:
    error_message = "CL_INVALID_KERNEL_DEFINITION";
    break;
  case CL_INVALID_KERNEL:
    error_message = "CL_INVALID_KERNEL";
    break;
  case CL_INVALID_ARG_INDEX:
    error_message = "CL_INVALID_ARG_INDEX";
    break;
  case CL_INVALID_ARG_VALUE:
    error_message = "CL_INVALID_ARG_VALUE";
    break;
  case CL_INVALID_ARG_SIZE:
    error_message = "CL_INVALID_ARG_SIZE";
    break;
  case CL_INVALID_KERNEL_ARGS:
    error_message = "CL_INVALID_KERNEL_ARGS";
    break;
  case CL_INVALID_WORK_DIMENSION:
    error_message = "CL_INVALID_WORK_DIMENSION";
    break;
  case CL_INVALID_WORK_GROUP_SIZE:
    error_message = "CL_INVALID_WORK_GROUP_SIZE";
    break;
  case CL_INVALID_WORK_ITEM_SIZE:
    error_message = "CL_INVALID_WORK_ITEM_SIZE";
    break;
  case CL_INVALID_GLOBAL_OFFSET:
    error_message = "CL_INVALID_GLOBAL_OFFSET";
    break;
  case CL_INVALID_EVENT_WAIT_LIST:
    error_message = "CL_INVALID_EVENT_WAIT_LIST";
    break;
  case CL_INVALID_EVENT:
    error_message = "CL_INVALID_EVENT";
    break;
  case CL_INVALID_OPERATION:
    error_message = "CL_INVALID_OPERATION";
    break;
  case CL_INVALID_GL_OBJECT:
    error_message = "CL_INVALID_GL_OBJECT";
    break;
  case CL_INVALID_BUFFER_SIZE:
    error_message = "CL_INVALID_BUFFER_SIZE";
    break;
  case CL_INVALID_MIP_LEVEL:
    error_message = "CL_INVALID_MIP_LEVEL";
    break;
  case CL_INVALID_GLOBAL_WORK_SIZE:
    error_message = "CL_INVALID_GLOBAL_WORK_SIZE";
    break;
  case CL_INVALID_PROPERTY:
    error_message = "CL_INVALID_PROPERTY";
    break;
  case CL_INVALID_IMAGE_DESCRIPTOR:
    error_message = "CL_INVALID_IMAGE_DESCRIPTOR";
    break;
  case CL_INVALID_COMPILER_OPTIONS:
    error_message = "CL_INVALID_COMPILER_OPTIONS";
    break;
  case CL_INVALID_LINKER_OPTIONS:
    error_message = "CL_INVALID_LINKER_OPTIONS";
    break;
  case CL_INVALID_DEVICE_PARTITION_COUNT:
    error_message = "CL_INVALID_DEVICE_PARTITION_COUNT";
    break;
  }

  std::cerr << "OpenCL fails. The error was " << error_message << "(" << error << ")" << std::endl;
  exit(1);
}

void ClContainer::read_source(std::string path, std::string* source_code)
{
  std::ifstream source_file(path.c_str());
  *source_code = std::string(std::istreambuf_iterator<char>(source_file),
                            (std::istreambuf_iterator<char>()));
}

void ClContainer::clean_mem()
{
}

cl::Event Buffer::write(const void* data, const std::vector<cl::Event> ev_list)
{
  cl::Event ev;
  cl_int error = m_container.queues[queue_id].enqueueWriteBuffer(
      buffer, CL_FALSE, 0, m_size, data, &ev_list, &ev);
  m_container.error_handler(error);
  return ev;
}

cl::Event Buffer::read(void* data, const std::vector<cl::Event> ev_list)
{
  cl::Event ev;
  cl_int error = m_container.queues[queue_id].enqueueReadBuffer(
      buffer, CL_FALSE, 0, m_size, data, &ev_list, &ev);
  m_container.error_handler(error);
  return ev;
}

void Buffer::clean()
{
}

cl::Event Kernel::exec(const std::vector<cl::Event> ev_list)
{
  cl::Event ev;
  cl_int error = m_container.queues[m_queue_id].enqueueNDRangeKernel(
      m_kernel, m_offset, cl::NDRange(m_globalsize), cl::NDRange(m_localsize), &ev_list, &ev);
  m_container.error_handler(error);
  return ev;
}

void Kernel::clean() {}
};

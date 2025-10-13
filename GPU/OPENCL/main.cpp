#define CL_TARGET_OPENCL_VERSION 120

#define __CL_ENABLE_EXCEPTIONS
#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#else
    #include "cl.hpp"
#endif
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string> 
#include <memory>


int main(int argc, char *argv[])
{
    int platform_id=0, device_id=0;
    try{
        // Query for platforms
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if(platforms.empty()){
            std::cout << "[°] No se encontraron plataformas" << "\n";
            return -1;
        }

        // Get a list of devices on this platform
        std::vector<cl::Device> devices;
        // Select the platform.
        platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU|CL_DEVICE_TYPE_CPU, &devices);
        cl::Device default_device = devices[0];
        //std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
        
        // Create a context
        cl::Context context(devices);

        // Create a command queue
        // Select the device.
        cl::CommandQueue queue = cl::CommandQueue( context, devices[device_id] );

        if (argc < 6) {
            std::cerr << "[°] Faltó algún parámetro" << std::endl;   
            return 1;
        }

        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "[°] Error al abrir el archivo: " << argv[1] << std::endl;
            return 1;
        }
                       
        int n = 10;
        size_t size = n * sizeof(int);
        int* A = new int[n];
        int* B = new int[n];
        int* d_c = new int[n];

        // Verify that allocations succeeded
        if (A == NULL || B == NULL || C == NULL )
        {
            fprintf(stderr, "Failed to allocate host vectors!\n");
            exit(EXIT_FAILURE);
        }
        
        // Create the memory buffers
        cl::Buffer bufferA=cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
        cl::Buffer bufferB=cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
        cl::Buffer bufferC=cl::Buffer(context, CL_MEM_WRITE_ONLY, n * sizeof(int));

        queue.enqueueWriteBuffer( bufferA, CL_FALSE, 0, x*y * sizeof(bool), data );


        std::string namefile = "checkPointThread2";
        if(kernelOption){
            namefile = "checkPointThread";
        }

        // Read the program source
        std::ifstream sourceFile("OPENCL/"+namefile+".cl");
        std::string sourceCode( std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
        cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

        // Make program from the source code
        cl::Program program=cl::Program(context, source);

        // Build the program for the devices
        program.build(devices);

        // Make kernel
        cl::Kernel checkPoint_kernel(program, namefile.c_str());



        size_t max_workgroup_size = default_device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        auto max_wi_sizes = default_device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

        // 2. Calcula un tamaño local adecuado
        size_t local_size = std::min((size_t)sizeBlock, max_workgroup_size);
        local_size = std::min(local_size, (size_t)max_wi_sizes[0]);

        // 3. Ajusta el tamaño global para que sea divisible
        size_t global_size = ((x*y) + local_size - 1) / local_size * local_size;


        // Execute the kernel
        cl::NDRange global( global_size );
        cl::NDRange local( local_size );

        for (int i = 0; i < nSteps; i++)
        {
            checkPoint_kernel.setArg( 0, bufferA );
            checkPoint_kernel.setArg( 1, bufferC );
            checkPoint_kernel.setArg( 2, static_cast<int>(y) );
            checkPoint_kernel.setArg( 3, static_cast<int>(x) );
            checkPoint_kernel.setArg( 4, static_cast<int>(y*x) );
            queue.enqueueNDRangeKernel( checkPoint_kernel, cl::NullRange, global, local );
            std::swap(bufferA, bufferC);
        }
    



        // Copy the output data back to the host
        queue.enqueueReadBuffer( bufferA, CL_TRUE, 0, x*y * sizeof(bool), data );
        if (flag){
            printTable(data,x,y);
        }
        
    }catch (cl::Error err) {
        std::cout << "Error: " << err.what() << "(" << err.err() << ")" << std::endl;
              return( EXIT_FAILURE );
    }
    std::cout << "[°] Ejecución terminada con éxito" <<  std::endl; 
    return 0;
}
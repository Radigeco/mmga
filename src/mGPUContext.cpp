#include "mGPUContext.h"
#include "mGenome.h"
#include "mLog.h"

#define MMKERNELSRC(...) #__VA_ARGS__

namespace mmga {
    const char * mGPUContext::bilinear_alg_name = "cl_bilinear_alg";
    const char * mGPUContext::bilinear_alg_src =
#include "cl_bilinear_alg_naive"    

            mGPUContext::mGPUContext(){
        mm_log;

        int num_entries = 1; // private, const
        int properties = 0;
        int num_devices = 1;
        cl_context_properties ctx_properties = 0;

        // Connect to a compute device
        err = clGetPlatformIDs(num_entries, &platform, NULL);
        if (err != CL_SUCCESS) {
            cerr << "Error: Failed to find a platform!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }

        // Get a device of the appropriate type
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_entries, &device, NULL);
        if (err != CL_SUCCESS) {
            cerr << "Error: Failed to create a device group!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }

        // Create a compute context
        context = clCreateContext(&ctx_properties, num_devices, &device, NULL, NULL, &err);
        if (!context) {
            cerr << "Error: Failed to create a compute context!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }

        // Create a command commands
        command_queue = clCreateCommandQueue(context, device, properties, &err);
        if (!command_queue) {
            cerr << "Error: Failed to create a command commands!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }

        setupKernel(bilinear_alg_name, bilinear_alg_src, bilinear_kernel);
    }

    void mGPUContext::setupKernel(const char * kernel_name, const char * kernel_src, cl_kernel & kernel_obj) {
        // Create the compute program from the source buffer
        mm_log << "setting up kernel " << kernel_name;

        cl_program program = clCreateProgramWithSource(context, 1,
                (const char **) &kernel_src, NULL, &err);

        if (!program) {
            cerr << "Error: Failed to create compute program!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }

        // Build the program executable
        err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        if (err != CL_SUCCESS) {
            size_t len;
            char buffer[4096];
            cerr << "Error: Failed to build program executable!" << endl << err << endl;
            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof (buffer), buffer, &len);
            cerr << buffer << endl;
            exit(EXIT_FAILURE);
        }

        // Create the compute kernel in the program
        kernel_obj = clCreateKernel(program, kernel_name, &err);
        if (!kernel_obj || err != CL_SUCCESS) {
            cerr << "Error: Failed to create compute kernel!" << endl << err << endl;
            exit(EXIT_FAILURE);
        }
    }

    mGPUContext::~mGPUContext() {
        mm_log;
        //OpenCL specific memory cleanup
        clReleaseContext(this->context);
        clReleaseCommandQueue(this->command_queue);
        clReleaseKernel(this->bilinear_kernel);
    }

    mGPUContext * mGPUContext::getContext() {
        static mGPUContext __ctx;
        return & __ctx;
    }

    void mGPUContext::evaluate(GAPopulation & pop) {
        //(a) convert pop into float arrays using the mPArrayAdapter
        mm_log;

        cl_mem cl_A; // device memory for A coefficient
        cl_mem cl_B; // device memory for B coefficient
        cl_mem cl_C; //device memory for C coefficient
        cl_mem cl_sigma; //device memory for sigma size
        cl_mem cl_delta; //device memory for delta size
        cl_mem cl_R; // device memory for the result array
        float * A; //host memory for A array of matrix
        float * B; //host memory for B array of matrix
        float * C; //host memory for C array of matrix
        float * R; // host memory for the Result array
        int sigma; // host memory for sigma size
        int delta; // host memory for delta size        

        float * kron; //host memory for precalculated kronecker products
        cl_mem cl_kron;
        //all operations must be performed using ctx
        mGPUContext * ctx = mGPUContext::getContext();
        int A_size = pop.size() * ((mGenome&) pop.individual(0)).length() / 3;
        cout << "popsize:" << pop.size() << endl;
        A = new float[A_size];
        B = new float[A_size];
        C = new float[A_size];
        ctx->global_size = pop.size();
        ctx->local_size = 1; //comment
        R = new float[ctx->global_size];
        delta = ((mGenome&) pop.individual(0)).matrixSize();
        sigma = ((mGenome&) pop.individual(0)).numberOfMultiplications();

        int delta2 = delta * delta;
        int delta6 = delta2 * delta2 * delta2;
        kron = new float[delta6];

        for (int i = 0, p = 0; i < delta; ++i)
            for (int j = 0; j < delta; ++j)
                for (int k = 0; k < delta; ++k)
                    for (int l = 0; l < delta; ++l)
                        for (int m = 0; m < delta; ++m)
                            for (int n = 0; n < delta; ++n)
                                kron[p++] = (i == n) * (j == k) * (l == m);


        // cout << "ctx->global_size:" << ctx->global_size << endl;
        //cout << "nrmul:" << sigma << endl;
        //cout << "msize:" << delta << endl;

        mPArrayAdapter()(pop, A, B, C);

        //todo: reuse (if possible) the float array between evaluations	(later)

        // Allocation of device memory
        cl_A = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (float) * A_size, NULL, NULL);
        cl_B = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (float) * A_size, NULL, NULL);
        cl_C = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (float) * A_size, NULL, NULL);

        //Allocation of delta, sigma parameters in CL
        cl_delta = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (int), NULL, NULL);
        cl_sigma = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (int), NULL, NULL);

        cl_kron = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, sizeof (float) * delta6, NULL, NULL);

        //Allocation of the Result multi dimensional arrays in CL
        cl_R = clCreateBuffer(ctx->context, CL_MEM_READ_WRITE, sizeof (float) * ctx->global_size, NULL, NULL);

        if (!cl_A || !cl_B || !cl_C || !cl_sigma || !cl_delta || !cl_R || !cl_kron) {
            mm_err << "Error: Failed to allocate device memory!";
            exit(EXIT_FAILURE);
        }

        //(b) (1) copy the array to the GPU, (2) make the evaluation, (3) copy back the fitness values
        // Transfer the host vector into device memory
        ctx->err = clEnqueueWriteBuffer(ctx->command_queue, cl_A, CL_TRUE, 0, sizeof (float) * A_size, A, 0, NULL, NULL);
        ctx->err |= clEnqueueWriteBuffer(ctx->command_queue, cl_B, CL_TRUE, 0, sizeof (float) * A_size, B, 0, NULL, NULL);
        ctx->err |= clEnqueueWriteBuffer(ctx->command_queue, cl_C, CL_TRUE, 0, sizeof (float) * A_size, C, 0, NULL, NULL);
        ctx->err |= clEnqueueWriteBuffer(ctx->command_queue, cl_sigma, CL_TRUE, 0, sizeof (int), &sigma, 0, NULL, NULL);
        ctx->err |= clEnqueueWriteBuffer(ctx->command_queue, cl_delta, CL_TRUE, 0, sizeof (int), &delta, 0, NULL, NULL);
        ctx->err |= clEnqueueWriteBuffer(ctx->command_queue, cl_kron, CL_TRUE, 0, sizeof (float) * delta6, kron, 0, NULL, NULL);

        if (ctx->err != CL_SUCCESS) {
            cerr << "Error: Failed to write to source array!" << endl << ctx->err << endl;
            exit(EXIT_FAILURE);
        }

        //Execution of kernels
        cl_mem bilinear_args [] = {cl_A, cl_B, cl_C, cl_sigma, cl_delta, cl_kron, cl_R};
        ctx->runKernel(ctx->bilinear_kernel, 7, bilinear_args);

        //Read back the results from the device
        ctx->err = clEnqueueReadBuffer(ctx->command_queue, cl_R, CL_TRUE, 0, sizeof (float) * ctx->global_size, R, 0, NULL, NULL);
        if (ctx->err != CL_SUCCESS) {
            cerr << "Error: Failed to read output array! " << endl << ctx->err << endl;
            exit(1);
        }

        //(c) assign the fitness values to each element of pop
        for (int i = 0; i < pop.size(); ++i)
            pop.individual(i).score(R[i]);

        clReleaseMemObject(cl_A);
        clReleaseMemObject(cl_B);
        clReleaseMemObject(cl_C);
        clReleaseMemObject(cl_R);
        clReleaseMemObject(cl_delta);
        clReleaseMemObject(cl_sigma);
        clReleaseMemObject(cl_kron);
        delete [] A;
        delete [] B;
        delete [] C;
        delete [] R;
        delete [] kron;

    }

    void mGPUContext::runKernel(cl_kernel & kernel, int nrparam, cl_mem * mem) {
        mm_log;
        mGPUContext * ctx = this->getContext();
        ctx->err = 0;

        for (int i = 0; i < nrparam; ++i) {
            ctx->err = clSetKernelArg(kernel, i, sizeof (cl_mem), &mem[i]);
            if (ctx->err != CL_SUCCESS) {
                mm_err << "Error: Failed to set kernel argument " << i << endl << ctx->err << endl;
                exit(EXIT_FAILURE);
            }
        }

        //Execute the kernel on our data
        ctx->err = clEnqueueNDRangeKernel(ctx->command_queue, kernel, 1, NULL, &ctx->global_size, &ctx->local_size, 0, NULL, NULL);

        if (ctx->err) {
            cerr << "Error: Failed to execute kernel!" << endl;
            exit(EXIT_FAILURE);
        }

        // Wait for all commands to complete
        clFinish(ctx->command_queue);
    }
}






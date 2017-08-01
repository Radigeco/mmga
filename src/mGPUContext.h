/** 
 * File:   mGPUContext.h
 * Author: andrasjoo
 *
 * Created on May 2, 2012, 8:42 AM
 */

#ifndef MGPUCONTEXT_H
#define	MGPUCONTEXT_H

#include "ga/GAPopulation.h"
#include <iostream>
#include <CL/cl.h>
#include "mPArrayAdapter.h"

using namespace std;

namespace mmga {

    /**
     * Singleton class representing the GPU context. 
     * --NOT-- to be called from a multi-threading section.
     */

    class mGPUContext {
        cl_platform_id platform; //to get the platform provided by the vendor
        cl_context context; //to setup the context
        cl_device_id device; //to get the device, which has a platform
        cl_command_queue command_queue; //to setup the command queue
        cl_kernel bilinear_kernel; //to get the kernel(function) that will be run on the device
        size_t global_size; //global worksize
        size_t local_size; //local worksize

        cl_int err; // for error checking 

        static const char * bilinear_alg_src;
        static const char * bilinear_alg_name;

        /**
         * Default constructor. Builds the OpenCL context.
         */

        mGPUContext();

        /**
         * Destructor. Cleans up allocated GPU resources.
         */

        ~mGPUContext();

        /**
         * Get pointer to the unique context.
         * The context consists of the following: command queue, device, compute context, command queue.
         * The user needs to setup the kernel and compute program based on this context
         * @return GPU context.
         */

        static mGPUContext * getContext();
        /**
         * Setup for a kernel
         */
        void setupKernel(const char * name, const char * source, cl_kernel & obj);

        /**
         * Execute a kernel
         */
        void runKernel(cl_kernel&, int, cl_mem*);

    public:

        /**
         * GPU evaluation routine.
         * @param pop Population to evaluate.
         */

        static void evaluate(GAPopulation & pop);

    };

}

#endif


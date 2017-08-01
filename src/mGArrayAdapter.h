/* 
 * File:   mGArrayAdapter.h
 * Author: andrasjoo
 *
 * Created on April 12, 2012, 12:43 PM
 */

#ifndef MGARRAYADAPTER_H
#define	MGARRAYADAPTER_H

#include "mGenome.h"




namespace mmga {

    /**
     * Converts a mGenome object into a flattened float array.  
     */
    struct mGArrayAdapter {
        /**
         * Conversion operator.
         * @param in_g mGenome object to convert.
         * @param out_a Output array that contains the A 3D arrays
         * @param out_b Output array that contains the B 3D arrays
         * @param out_c Output array that contains the C 3D arrays 
         * NB. The memory is managed by the caller.
         */

        void operator()(const mGenome & in_g, float * out_a, float * out_b, float * out_c);

    };
};

#endif	/* MGARRAYADAPTER_H */


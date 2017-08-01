/* 
 * File:   mPArrayAdapter.h
 * Author: andrasjoo
 *
 * Created on April 12, 2012, 12:43 PM
 */

#ifndef MPARRAYADAPTER_H
#define	MPARRAYADAPTER_H

#include "ga/GAPopulation.h"
#include "mGenome.h"
#include "mGArrayAdapter.h"

namespace mmga {

    /**
     * Converts a GAPopulation object into a flattened float array. ///!!!  
     */
    struct mPArrayAdapter {
        /**
         * Conversion operator.
         * divides and flattens the population in 3*population.size() number of 3D arrays
         * @param in_p population object to convert
         * @param out_a contains population.size() number of 3D arrays
         * @param out_b contains population.size() number of 3D arrays
         * @param out_c contains population.size() number of 3D arrays
         * NB. The memory is managed by the caller.
         */

        void operator()(const GAPopulation & in_p, float * out_a, float * out_b, float * out_c);
    };

}
#endif	/* MPARRAYADAPTER_H */


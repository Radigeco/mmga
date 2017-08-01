/* 
 * File:   mJohnsonGE.h
 * Author: andrasjoo
 *
 * Created on May 28, 2012, 3:43 PM
 */
#ifndef MJOHNSON_H
#define MJOHNSON_H

#include "ga/GARealGenome.h"

namespace mmga
{

/**
 * Based on Johnson's formula described in Noncommutative bilinear algorithms 
 * for 3x3 matrix multiplication (Johnson et al, 1986).
 *
 * Fitness ranges from 0.0f to 1.0f, inclusive. 1.0f corresponds to a perfect
 * genome.
 * 
 * The evaluator runs on the CPU and is based on Armadillo.
 *
 * @param gen Genome to evaluate. 
 * @return The fitness corresponding to gen.
 */ 

float mJohnsonGE(GAGenome & gen);

}

#endif
/* 
 * File:   mJohnsonGE.cpp
 * Author: andrasjoo
 *
 * Created on May 28, 2012, 3:43 PM
 */

#include "mJohnsonGE.h"
#include "mGenome.h"
#include <armadillo/armadillo>
#include "mGCubeAdapter.h"

namespace mmga {

    float mJohnsonGE(GAGenome & gen) {
        int sigma = ((mGenome&) gen).numberOfMultiplications();
        int delta = ((mGenome&) gen).matrixSize();
        float s1, s2;
        float err = 0;

        arma::cube a, b, c;
        mGCubeAdapter()((mGenome&) gen, a, b, c);

        for (int i = 0; i < delta; ++i) {
            for (int j = 0; j < delta; ++j) {
                for (int k = 0; k < delta; ++k) {
                    for (int l = 0; l < delta; ++l) {
                        for (int m = 0; m < delta; ++m) {
                            for (int n = 0; n < delta; ++n) {
                                s1 = (n == i) * (j == k) * (l == m);
                                s2 = 0;

                                for (int r = 0; r < sigma; ++r) {
                                    s2 += a(i, j, r) * b(k, l, r) * c(m, n, r);
                                }

                                err += (s2 - s1) * (s2 - s1);
                            }
                        }
                    }
                }
            }
        }

        return 1.0f / (1.0f + err);
    }

}

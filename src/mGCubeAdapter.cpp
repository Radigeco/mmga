#include "mGCubeAdapter.h"


namespace mmga {

    void mGCubeAdapter::operator ()(const mGenome& in_g, arma::cube& out_a, arma::cube& out_b, arma::cube& out_c) {
        unsigned int msize = in_g.matrixSize();
        unsigned nrmul = in_g.numberOfMultiplications();

        out_a.set_size(msize, msize, nrmul); 
        out_b.set_size(msize, msize, nrmul);
        out_c.set_size(msize, msize, nrmul);

        unsigned offs1 = nrmul * msize * msize;
        unsigned offs2 = 2 * offs1;

        int m = 0;

		for (unsigned k = 0; k < nrmul; ++k){
	        for (unsigned i = 0; i < msize; ++i) {
    	        for (unsigned j = 0; j < msize; ++j, ++m) {
                    out_a(i, j, k) = in_g.gene(m);
                    out_b(i, j, k) = in_g.gene(m + offs1);
                    out_c(i, j, k) = in_g.gene(m + offs2);
                }
            }
        }


    }

}

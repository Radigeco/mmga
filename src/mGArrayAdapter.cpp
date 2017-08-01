#include "mGArrayAdapter.h"

namespace mmga {

    void mGArrayAdapter::operator ()(const mGenome& in_g, float* out_a, float* out_b, float* out_c) {
        int offs1 = in_g.numberOfMultiplications() * in_g.matrixSize() * in_g.matrixSize();
        int offs2 = 2 * offs1;

        for (int i = 0; i < offs1; ++i) {
            out_a[i] = in_g.gene(i);
            out_b[i] = in_g.gene(i + offs1);
            out_c[i] = in_g.gene(i + offs2);
        }

    }
};

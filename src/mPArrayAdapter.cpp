#include "mPArrayAdapter.h"

namespace mmga {

    void mPArrayAdapter::operator ()(const GAPopulation& in_p, float* out_a, float* out_b, float* out_c) {

        unsigned offs = ((mGenome&) in_p.individual(0)).length() / 3;

        for (int k = 0; k < in_p.size(); ++k)
            mGArrayAdapter()((mGenome&) in_p.individual(k),
                out_a + k * offs,
                out_b + k * offs,
                out_c + k * offs);

    }
}

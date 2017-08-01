#include "mPopEvalGPU.h"
#include "mGPUContext.h"

namespace mmga {

    void mPopEvalGPU(GAPopulation & pop) {
        mGPUContext::evaluate(pop);
    }

}

/** 
 * File:   mGenome.cpp
 * Author: andrasjoo
 *
 * Created on April 12, 2012, 8:42 AM
 */


#include "mGenome.h"
#include "mGArrayAdapter.h"
#include "ga/GARealGenome.C"
#include "mGA.h"

// From GALIB:
// "...To use the real genome in your code, you must include the real genome header file in each 
// of your files that uses the real genome. You must also include the real genome source file
// (it contains template specialization code) in one (and only one) of your source files. 
// Including the real genome source file will force the compiler to use the real specializations..."


namespace mmga {

    const float mGenome::MIN_ALLELE = -1.0f;

    const float mGenome::MAX_ALLELE = +1.0f;

    const GARealAlleleSet mGenome::RAS(mGenome::MIN_ALLELE, mGenome::MAX_ALLELE);

    mGenome::mGenome(unsigned msize, unsigned nrmul) : GARealGenome(RAS), msize(msize), nrmul(nrmul) {

        this->length(msize * msize * nrmul * 3);
    }

    mGenome::mGenome(unsigned msize, unsigned nrmul, std::string str) : GARealGenome(RAS), msize(msize), nrmul(nrmul) {
        assert(msize >= 2 && msize <= 4);
        assert(nrmul >= 7);
        this->length(msize * msize * nrmul * 3);

        std::stringstream ss(str);
        float g;
        for (int k = 0; ss >> g; ++k)
            this->gene(k, g);

    }

    mGenome::mGenome(const mGenome& orig) : GARealGenome(orig) {
        this->msize = orig.msize;
        this->nrmul = orig.nrmul;
    }

    unsigned mGenome::matrixSize() const {

        return this->msize;
    }

    unsigned mGenome::numberOfMultiplications() const {
        return this->nrmul;
    }

    mGenome& mGenome::operator =(const GAGenome& orig) {
        if (&orig != this) copy(orig);
        return *this;
    }

    void mGenome::copy(const GAGenome& orig) {
        GARealGenome::copy(orig);
        const mGenome & _orig = (const mGenome &) orig;
        this->msize = _orig.msize;
        this->nrmul = _orig.nrmul;
    }

    GAGenome* mGenome::clone(CloneMethod cm) const {
        return new mGenome(*this);
    }

    mGenome::~mGenome() {
    }

    int mGenome::RealGaussianMutator(GAGenome& g, float pmut) {
        GA1DArrayAlleleGenome<float> &child =
                DYN_CAST(GA1DArrayAlleleGenome<float> &, g);
        float amp = mGA::mutationAmp();
        register int n, i;
        if (pmut <= 0.0) return (0);

        float nMut = pmut * (float) (child.length());
        int length = child.length() - 1;
        if (nMut < 1.0) { // we have to do a flip test on each element
            nMut = 0;
            for (i = length; i >= 0; i--) {
                float value = child.gene(i);
                if (GAFlipCoin(pmut)) {
                    if (child.alleleset(i).type() == GAAllele::ENUMERATED ||
                            child.alleleset(i).type() == GAAllele::DISCRETIZED)
                        value = child.alleleset(i).allele();
                    else if (child.alleleset(i).type() == GAAllele::BOUNDED) {
                        value += amp * GAUnitGaussian();
                        value = GAMax(child.alleleset(i).lower(), value);
                        value = GAMin(child.alleleset(i).upper(), value);
                    }
                    child.gene(i, value);
                    nMut++;
                }
            }
        } else { // only mutate the ones we need to
            for (n = 0; n < nMut; n++) {
                int idx = GARandomInt(0, length);
                float value = child.gene(idx);
                if (child.alleleset(idx).type() == GAAllele::ENUMERATED ||
                        child.alleleset(idx).type() == GAAllele::DISCRETIZED)
                    value = child.alleleset(idx).allele();
                else if (child.alleleset(idx).type() == GAAllele::BOUNDED) {
                    value += amp * GAUnitGaussian();
                    value = GAMax(child.alleleset(idx).lower(), value);
                    value = GAMin(child.alleleset(idx).upper(), value);
                }
                child.gene(idx, value);
            }
        }
        return ((int) nMut);
    }


}




//test functions for mGenome

#include "mGenome.h"
#include "mJohnsonGE.h"
#include <cassert>
#include "mLog.h"
#include "ga/GAPopulation.h"
#include "mPopEvalCPU.h"
#include <iostream>
#include <iterator>

using namespace std;
using namespace mmga;

void tevalstrassen() {
    mm_log << "testing";

    std::string strassen = "1 0 0 1 0 0 1 1 1 0 0 0 0 0 0 1 1 1 0 0 -1 0 1 0 0 1 0 -1\
                          1 0 0 1 1 0 0 0 0 1 0 -1 -1 0 1 0 0 0 0 1 1 1 0 0 0 0 1 1\
		    			  1 0 0 1 0 1 0 -1 0 0 1 1 1 1 0 0 -1 0 1 0 0 0 0 1 1 0 0 0";

    mGenome g(2, 7, strassen);
    g.evaluator(mJohnsonGE);
    g.evaluate();
    assert(g.score() == 1.0f);
    mm_log << "passed";
}

void tevalzeros() {
    mm_log << "testing";

    std::stringstream zeros;
    fill_n(ostream_iterator<string > (zeros), 84, "0 ");

    mGenome g(2, 7, zeros.str());
    g.evaluator(mJohnsonGE);
    g.evaluate();
    assert(1.0f != g.score());
    mm_log << "passed";
}

void tevalapprox() {
    mm_log << "testing";

    std::string gapprox = "1.457746 0.463267 -1.809741 1.190045 0.768351 -0.524340 -0.190790 0.127919 -0.141505 \
                          -1.869923 0.051039 0.444630 0.088598 1.321331 0.061380 0.932895 0.349327 -0.314009 \
                           0.248115 -0.221140 1.225120 -0.860113 -1.605163 1.394299 -0.963090 0.659562 1.276389 -0.861715\
  					       1.387956 0.326725 1.949332 0.657979 1.143703 0.597192 -0.078319 -0.038726 1.140377 0.553803 \
						   1.623138 0.917227 -0.492609 1.064451 -0.541860 1.192156 -0.527731 1.146459 0.036414 -0.075748 \
						   1.378089 0.524578 2.081269 0.684772 1.455776 0.570135 1.640031 0.536097 \
						   0.680131 -0.577106 -1.098057 1.726545 1.080466 0.814420 0.492695 0.376996 0.312772 -0.496841 \
						   -1.028739 1.427900 -0.046490 -0.189886 0.133092 0.543258 -0.657797 -0.469123 1.231836 0.940614\
						   -0.346203 1.943728 1.252628 -1.102773 0.576352 2.143629 0.183907 1.095282";

    mGenome g(2, 7, gapprox);
    g.evaluator(mJohnsonGE);
    g.evaluate();
    assert(g.score() > 0.9f);
    assert(g.score() < 1.0f);
    mm_log << "passed";
}


void tevalladerman() {
    mm_log << "testing";

    GAPopulation pop;

    std::string laderman = "1 1 1 -1 -1 0 0 -1 -1 \
                                                1 0 0 -1 0 0 0 0 0 \
                                                0 0 0 0 1 0 0 0 0\
  					       -1 0 0 1 1 0 0 0 0 \
						0 0 0 1 1 0 0 0 0 \
						1 0 0 0 0 0 0 0 0 \
						-1 0 0 0 0 0 1 1 0 \
						-1 0 0 0 0 0 1 0 0\
						0 0 0 0 0 0 1 1 0\
                                                1 1 1 0 -1 -1 -1 -1 0\
                                                0 0 0 0 0 0 0 1 0\
                                                0 0 -1 0 0 0 0 1 1\
                                                0 0 1 0 0 0 0 0 -1\
                                                0 0 1 0 0 0 0 0 0\
                                                0 0 0 0 0 0 0 1 1\
                                                0 0 -1 0 1 1 0 0 0\
                                                0 0 1 0 0 -1 0 0 0\
                                                0 0 0 0 1 1 0 0 0\
                                                0 1 0 0 0 0 0 0 0\
                                                0 0 0 0 0 1 0 0 0\
                                                0 0 0 1 0 0 0 0 0\
                                                0 0 0 0 0 0 1 0 0\
                                                0 0 0 0 0 0 0 0 1\
                                                0 0 0 0 1 0 0 0 0\
                                                0 -1 0 0 1 0 0 0 0\
                                                -1 1 0 1 -1 -1 -1 0 1\
                                                1 -1 0 0 1 0 0 0 0\
                                                -1 1 0 0 0 0 0 0 0\
                                                1 0 0 0 0 0 0 0 0\
                                                1 0 -1 0 0 1 0 0 0\
                                                0 0 1 0 0 -1 0 0 0\
                                                -1 0 1 0 0 0 0 0 0\
                                                0 0 0 0 0 1 0 0 0\
                                                -1 0 1 1 -1 -1 -1 1 0\
                                                0 0 0 0 1 0 1 -1 0\
                                                0 0 0 0 1 0 0 -1 0\
                                                0 0 0 0 0 0 1 0 0\
                                                0 0 0 0 0 0 -1 1 0\
                                                0 0 0 0 0 1 1 0 -1\
                                                0 0 0 0 0 1 0 0 -1\
                                                0 0 0 0 0 0 -1 0 1\
                                                0 0 0 1 0 0 0 0 0\
                                                0 0 0 0 0 0 0 1 0\
                                                0 0 1 0 0 0 0 0 0\
                                                0 1 0 0 0 0 0 0 0\
                                                0 0 0 0 0 0 0 0 1\
                                                0 0 0 1 0 0 0 0 0\
                                                0 1 0 0 1 0 0 0 0\
                                                0 1 0 0 0 0 0 0 0\
                                                0 1 0 1 1 0 0 0 0\
                                                0 0 0 1 1 0 0 0 0\
                                                1 1 1 1 1 0 1 0 1\
                                                0 0 1 0 0 0 1 0 1\
                                                0 0 1 0 0 0 0 0 1\
                                                0 0 0 0 0 0 1 0 1\
                                                0 0 0 0 0 0 1 0 0\
                                                0 0 1 0 0 0 0 0 0\
                                                0 0 1 1 0 1 0 0 0\
                                                0 0 1 0 0 1 0 0 0\
                                                1 1 1 1 0 1 1 1 0\
                                                0 0 0 1 0 1 0 0 0\
                                                0 1 0 0 0 0 1 1 0\
                                                0 1 0 0 0 0 0 1 0\
                                                0 0 0 0 0 0 1 1 0\
                                                1 0 0 0 0 0 0 0 0\
                                                0 0 0 0 1 0 0 0 0\
                                                0 0 0 0 0 0 0 1 0\
                                                0 0 0 0 0 1 0 0 0\
                                                0 0 0 0 0 0 0 0 1\
";

    mGenome genome(3, 23, laderman);
    genome.evaluator(mJohnsonGE);
    pop.add(genome);
    pop.evaluator(mPopEvalCPU);
	pop.evaluate();
    assert(pop.individual(0).score() == 1.0f);
    mm_warn << "laderman genome scored: " << pop.individual(0).score();

}

int main() {
    tevalstrassen();
    tevalzeros();
    tevalapprox();
    tevalladerman();
    return 0;
}


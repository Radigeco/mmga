/* 
 * File:   mGA.cpp
 * Author: radi
 * 
 * Created on June 4, 2012, 9:28 PM
 */

#include "mGA.h"
#include "mColor.h"
#include "ga/garandom.h"

namespace mmga {

    float mGA::getGenoEps() {
        return this->eps_geno;
    }

    float mGA::getPhenoEps() {
        return this->eps_pheno;
    }

    void mGA::setPhenoEps(float eps) {
        this->eps_pheno = eps;
    }

    void mGA::setGenoEps(float eps) {
        this->eps_geno = eps;
    }

    void mGA::step() {

        static unsigned long long c = 0;
        mGenome mom, dad, child;
        //!!!!
        int nrchild = pop->size() / 2;
        GAPopulation tmpPop;

        for (int i = 0; i < nrchild;) {
            mom = this->pop->select();
            dad = this->pop->select();

            if (GAFlipCoin(pCrossover())) //Crossover if it procs
                stats.numcro += (*scross)(mom, dad, &child, 0);
            else
                child.copy(mom);
            stats.nummut += child.mutate(pMutation()); //Mutation if it procs
            child.evaluate();

            //!!!!
            if (child.score() > dad.score() || child.score() > mom.score() || GAFlipCoin(0.01)) {
                tmpPop.add(child);
                ++i;
            }

        }

        for (int i = 0; i < nrchild; i++)
            pop->add(tmpPop.individual(i));

        pop->evaluate(); // get info about current pop for next time
        pop->scale(); // remind the population to do its scaling
         cout << ++c << " " << pop->size() << "  " <<  pop->best().score() << endl;

        mGenome best((mGenome&) pop->best()); //the best always survives
        mGA::mutationAmp(1.0 - best.score());
        
        while (nrchild) //delete extra individuals
        {
            int idx = GARandomInt(0, pop->size() - 1);
            mGenome & x = (mGenome&) pop->individual(idx);
            if (!genoDistance(x, *pop) || !phenoDistance(x, *pop)) {
                pop->destroy(idx);
                --nrchild;
            }//!!!!
            else if (GAFlipCoin(0.1)) {
                pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);
                --nrchild;
            }
        }

        if (pop->best().equal(best)) {
            pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);
            pop->add(best);
        }


        stats.update(*pop); // update the statistics by one generation

    }

    float mGA::genomeScoreDistance(mGenome& gen1, mGenome& gen2) {
        return fabs(gen1.score() - gen2.score());
    }

    //!!!!

    float mGA::genomeGeneDistance(mGenome& gen1, mGenome& gen2) {
        float sum = 0;
        for (int i = 0; i < gen1.length(); ++i) {
            float d = gen1.gene(i) - gen2.gene(i);
            sum += d*d;
        }
        return sqrt(sum);

    }
    //!!!!

    bool mGA::genoDistance(mGenome& gen, GAPopulation& pop) {


        for (int i = 0; i < pop.size(); ++i) {
            if (&gen == &pop.individual(i)) return true;
            if (genomeScoreDistance(gen, (mGenome&) pop.individual(i))<this->eps_geno)
                return false;
        }
        return true;

    }

    bool mGA::phenoDistance(mGenome& gen, GAPopulation& pop) {
        for (int i = 0; i < pop.size(); ++i) {
            if (&gen == &pop.individual(i)) return true;

            if (genomeGeneDistance(gen, (mGenome&) pop.individual(i))<this->eps_pheno)
                return false;
        }
        return true;
    }

    float mGA::mutationAmp(float x) {
        static float amp = 1.0f;
        if (x != 0.0f) {
            amp = x;
        }
        return amp;
    }
}

/* 
 * File:   mGA.h
 * Author: radi
 *
 * Created on June 4, 2012, 9:28 PM
 */

#ifndef MGA_H
#define	MGA_H
#include <ga/GASStateGA.h>
#include <iostream>
#include <cstring>
#include <mGenome.h>
#include <ga/GAGenome.h>
#include <mJohnsonGE.h>
#include <cmath>

using namespace std;

namespace mmga {

    class mGA : public GASteadyStateGA {
    public:

        mGA(const GAGenome& g) : GASteadyStateGA(g) {
        }

        mGA(const GAPopulation& pop) : GASteadyStateGA(pop) {

        }

        mGA(const GASteadyStateGA ga) : GASteadyStateGA(ga) {

        }

        GADefineIdentity("mGA", 388);

        virtual void step();

        /**
         * Sets the epsilon for phenotype distance
         * @param eps 
         */
        void setPhenoEps(float eps);
        float getPhenoEps();
        /**
         * Sets the epsilon for genotype distance
         * @param eps 
         */
        void setGenoEps(float eps);
        float getGenoEps();
        /**
         * Returns the Score distance between 2 mGenomes.
         * @param gen1  
         * @param gen2
         */
        float genomeScoreDistance(mGenome &gen1, mGenome &gen2);
        /**
         * Returns the Gene distance between 2 mGenomes.
         * @param gen1  
         * @param gen2
         */
        float genomeGeneDistance(mGenome &gen1, mGenome &gen2);
        /**
         * Returns true if the distance between the genome and the population is greater then the genotype epsilon.
         * @param gen  
         * @param pop
         */
        bool genoDistance(mGenome &gen, GAPopulation &pop);
        /**
         * Returns true if the distance between the genome and the population is greater then the phenotype epsilon.
         * @param gen  
         * @param pop
         */
        bool phenoDistance(mGenome &gen, GAPopulation &pop);
        static float mutationAmp(float x = 0.0f);
    private:
        float eps_pheno;
        float eps_geno;
    };

#endif	/* MGA_H */

}
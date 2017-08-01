#include <ga/GASStateGA.h>	// include the steady-state GA header
#include "mPopEvalGPU.h"
#include "mPopEvalCPU.h"
#include "mGenome.h"
#include "mLog.h"
#include <cassert>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace mmga;

void tsteadystatega(GAParameterList& list) {
    
    int msize, nrmul, popsize, ngen;
    float pmut, pcross, prepl;
    string sfile;
    list.get("msize", &msize);
    list.get("nrmul", &nrmul);
    list.get("popsize", &popsize);
    list.get("ngen", &ngen);
    list.get("pmut", &pmut);
    list.get("prepl", &prepl);
    list.get("pcross", &pcross);
    
    mm_log << "Running SteadyGA with | msize:" << msize << " | nrmul: " << nrmul << " | popsize: " << popsize << " | mutratio: " << pmut << " | crossratio: " << pcross << " | replratio: " << prepl << " | nrgen: " << ngen << " |" << endl;

    mGenome genome(msize, nrmul);
    GAPopulation pop(genome, popsize);
    pop.evaluator(mPopEvalGPU);
    GASteadyStateGA ga(pop);
    ga.parameters(list);
    ga.selector(GATournamentSelector());
    ga.scoreFilename("bog.dat"); // name of file for scores
    ga.scoreFrequency(ngen / 10); // keep the scores of every nrgen/10th generation
    ga.flushFrequency(ngen / 10); // specify how often to write the score to disk
    ga.selectScores(GAStatistics::AllScores);
    loglevel(logoff);
    ga.evolve();
    loglevel(loginfo);

    stringstream ss;
    ss << ga.statistics().bestIndividual();
    mm_log << "best individual (snippet): " << ss.str().substr(0, 50) << "...";
    mm_log << "best score: " << ga.statistics().bestIndividual().score();
    ofstream afs;
    afs.open("bestresults.txt", fstream::out | fstream::app);
    afs << endl << "Best score: " << ga.statistics().bestIndividual().score() << endl << ga.statistics().bestIndividual();
    afs.close();
    //We create a new file which contains the parameters in the beginning of it and delete the old one
    ofstream ofs;
    ofs.open("bog1.dat");
    ofs << msize << " " << nrmul << " " << popsize << " " << pmut << " " << pcross << " " << prepl << " " << ngen << " " << ga.statistics().bestIndividual().score() << endl;
    fstream ifs("bog.dat");
    ifs << noskipws;
    istream_iterator<unsigned char> begin(ifs);
    istream_iterator<unsigned char> end;
    ostream_iterator<unsigned char> begin2(ofs);
    copy(begin, end, begin2);
    ofs.close();
    ifs.close();
    remove("bog.dat");
    rename("bog1.dat", "bog.dat");
}

int main(int argc, char **argv) {
    //Declaration of variables with default values
    GAParameterList params;
    GASteadyStateGA::registerDefaultParameters(params);
    params.set("ngen", 250);
    params.set("popsize", 30);
    params.set("pmut", 0.01);
    params.set("pcross", 0.5);
    params.set("prepl", 0.8);
    params.set("sfreq", 10);
    params.set("ffreq", 10);
    string sfile = "bog.dat";
    int msize = 2;
    int nrmul = 7;
    params.add("msize", "msize", GAParameter::INT, &msize);
    params.add("nrmu", "nrmul", GAParameter::INT, &nrmul);
    params.add("sfile", "sfile", GAParameter::STRING, &sfile);
    params.parse(argc, argv, gaTrue);
    tsteadystatega(params);


    return 0;
}

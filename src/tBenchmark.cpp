//Benchmark file
#include "mPopEvalGPU.h"
#include "mGenome.h"
#include "mLog.h"
#include "mPopEvalCPU.h"
#include <cassert>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <time.h>


using namespace std;
using namespace mmga;

double tdiff(timespec & end, timespec & start) {
    double ten9 = 1000000000.0;
    double n1 = start.tv_sec * ten9 + start.tv_nsec;
    double n2 = end.tv_sec * ten9 + end.tv_nsec;
    return (n2 - n1) / ten9;
}

double tbenchmark(long N, void(*eval)(GAPopulation &), string desc, int msize, int nrmul) {

    mm_log << "testing";

    //first we do a phony run, just with one task, to warm up the GPU evaluator
    {
        mGenome genome(msize, nrmul);
        GAPopulation pop(genome, 1);
        pop.evaluator(eval);
        pop.evaluate();
    }

    mGenome genome(msize, nrmul);
    GAPopulation pop(genome, N);
    pop.evaluator(eval);

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    pop.evaluate();
    clock_gettime(CLOCK_REALTIME, &end);
    mm_warn << "benchmarking " << desc << " with " << pop.size() << " tasks on " << msize << "x" << msize << "x" << nrmul;
    mm_warn << "execution time:" << tdiff(end, start) << "s";
    return tdiff(end, start);
}

int main() {
    ofstream ofs("Bench_results_naive.txt");
    long N = 25600;
    //    tbenchmark(N, mmga::mPopEvalCPU, "mPopEvalCPU", 2, 7);
    // tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7);
    //    tbenchmark(N, mmga::mPopEvalCPU, "mPopEvalCPU", 3, 23);
    //    tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 3, 23);
    ofs << N << " : " << tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7) << endl;
    N *= 2;
    ofs << N << " : " << tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7) << endl;
    N *= 2;
    ofs << N << " : " << tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7) << endl;
    N *= 2;
    ofs << N << " : " << tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7) << endl;
    N *= 2;
    ofs << N << " : " << tbenchmark(N, mmga::mPopEvalGPU, "mPopEvalGPU", 2, 7) << endl;

    return 0;
}
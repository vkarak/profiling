#include "profiling.hpp"
#include "profilers/chrono_profiler.hpp"
#ifdef HAVE_VTUNE
#   include "profilers/vtune_profiler.hpp"
#endif

#ifdef HAVE_LIKWID
#   include "profilers/likwid_profiler.hpp"
#endif

#include <cstdlib>
#include <iostream>

#ifdef _OPENMP
#   define PARALLEL_REGION _Pragma("omp parallel")
#else
#   define PARALLEL_REGION
#endif

int main()
{
    ProfilingInit();

    ChronoProfiler cprof;
    ProfilingRegisterBackend(cprof, "chrono");

#ifdef HAVE_VTUNE
    VTuneProfiler vtprof;
    ProfilingRegisterBackend(vtprof, "vtune");
#endif

#ifdef HAVE_LIKWID
    VTuneProfiler lwprof;
    ProfilingRegisterBackend(lwprof, "likwid");
#endif

    PARALLEL_REGION
    {
        ProfilingInitParallel();
    }

    PARALLEL_REGION
    {
        ProfilingStart("print");
        std::cout << "Hello dudes, I'm gonna take a nap for a sec.\n";
        ProfilingStop("print");
        ProfilingStart("sleep");
        system("sleep 1");
        ProfilingStop("sleep");
        ProfilingStart("overhead");
        ProfilingStop("overhead");
    }

    ProfilingGetBackend("chrono").Report();
    return 0;
}

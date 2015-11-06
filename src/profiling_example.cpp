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
    profiling_init();

    chrono_profiler cprof;
    profiling_register_backend(cprof, "chrono");

#ifdef HAVE_VTUNE
    vtune_profiler vtprof;
    profiling_register_backend(vtprof, "vtune");
#endif

#ifdef HAVE_LIKWID
    likwid_profiler lwprof;
    profiling_register_backend(lwprof, "likwid");
#endif

    PARALLEL_REGION
    {
        profiling_init_parallel();
    }

    PARALLEL_REGION
    {
        profiling_start("print");
        std::cout << "Hello dudes, I'm gonna take a nap for a sec.\n";
        profiling_stop("print");
        profiling_start("sleep");
        system("sleep 1");
        profiling_stop("sleep");
        profiling_start("overhead");
        profiling_stop("overhead");
    }

    profiling_backend("chrono").report();
    return 0;
}

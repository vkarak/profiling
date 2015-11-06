#pragma once

#include "profilers/profiler.hpp"
#include <likwid.h>

class likwid_profiler : public profiler
{
public:
    likwid_profiler()
    {
        likwid_markerInit();
    }

    ~likwid_profiler()
    {
        likwid_markerClose();
    }

    void init_parallel()
    {
        likwid_markerThreadInit();
    }

    void start(const std::string &region)
    {
        likwid_markerStartRegion(region.c_str());
    }

    void stop(const std::string &region)
    {
        likwid_markerStopRegion(region.c_str());
    }

    double time(const std::string &region)
    {
        return 0.;
    }

    double time()
    {
        return 0.;
    }

    void report() const
    {
        // nothing to report from here
    }
};

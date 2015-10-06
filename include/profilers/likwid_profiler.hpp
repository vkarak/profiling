#pragma once

#include "profilers/profiler.hpp"
#include <likwid.h>

class LikwidProfiler : public Profiler
{
public:
    LikwidProfiler()
    {
        likwid_markerInit();
    }

    ~LikwidProfiler()
    {
        likwid_markerClose();
    }

    void InitParallel()
    {
        likwid_markerThreadInit();
    }

    void Start(const std::string &region)
    {
        likwid_markerStartRegion(region.c_str());
    }

    void Stop(const std::string &region)
    {
        likwid_markerStopRegion(region.c_str());
    }

    double Time(const std::string &region)
    {
        return 0.;
    }

    double Time()
    {
        return 0.;
    }

    void Report() const
    {
        // nothing to report from here
    }
};

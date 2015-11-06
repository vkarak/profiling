#pragma once

#include "profilers/profiler.hpp"

#include <map>
#include <ittnotify.h>


class vtune_profiler : public profiler
{
public:
    vtune_profiler()
        : domain_(__itt_domain_create("main app"))
    { }

    vtune_profiler(const std::string &domain_name)
        : domain_(__itt_domain_create(domain_name.c_str()))
    { }

    void init_parallel()
    {
        // do nothing, VTune API takes care of these
    }

    void start(const std::string &region)
    {
        if (taskmap_.find(region) == taskmap_.end()) {
            __itt_string_handle *task =
                __itt_string_handle_create(region.c_str());
            taskmap_.insert(std::make_pair(region, task));
        }

        __itt_task_begin(domain_, __itt_null, __itt_null, taskmap_[region]);
    }

    void stop(const std::string &region)
    {
        __itt_task_end(domain_);
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

private:
    __itt_domain *domain_;
    std::map<const std::string, __itt_string_handle *> taskmap_;
};

#pragma once

#include "profilers/profiler.hpp"
#include "utility/platform.hpp"

#include <chrono>
#include <unordered_map>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <iostream>

class ChronoProfiler : public Profiler
{
    using clock_type = std::chrono::high_resolution_clock;
    using duration_type = std::chrono::duration<double>;

public:
    ChronoProfiler()
    {
        // Just initialise for the calling thread
        InitParallel();
    }

    void InitParallel();

    void Start(const std::string &region)
    {
        // Non initialised threads will be ignored
        auto task_id = GetLocalTaskID();
        if (task_id < 0)
            return;

        if (timestamps_[task_id].find(region) != timestamps_[task_id].end()) {
            timestamps_[task_id][region].first = clock_type::now();
        } else {
            timestamps_[task_id].insert(
                std::make_pair(region, std::make_pair(clock_type::now(), 0.)));
        }
    }

    void Stop(const std::string &region)
    {
        // Not initialised thread will be ignored
        auto task_id = GetLocalTaskID();
        if (task_id < 0)
            return;

        if (timestamps_[task_id].find(region) != timestamps_[task_id].end()) {
            auto t_start = timestamps_[task_id][region].first;
            timestamps_[task_id][region].second +=
                duration_type(clock_type::now() - t_start).count();
        }
    }

    double Time();
    double Time(const std::string &region);
    void Report() const;

private:
    std::size_t GetLocalTaskID()
    {
        auto tid = GetSystemThreadID();
        std::size_t ret = -1;
        if (tid_map_.find(tid) != tid_map_.end())
            ret = tid_map_[tid];

        return ret;
    }

    std::mutex guard_;
    std::unordered_map<std::size_t, std::size_t> tid_map_;
    std::vector<std::map<const std::string,
                         std::pair<clock_type::time_point, double> > >
    timestamps_;
};

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

class chrono_profiler : public profiler
{
    using clock_type = std::chrono::high_resolution_clock;
    using duration_type = std::chrono::duration<double>;

public:
    chrono_profiler()
    {
        // Just initialise for the calling thread
        init_parallel();
    }

    void init_parallel();

    void start(const std::string &region)
    {
        // Non initialised threads will be ignored
        auto task_id = local_task_id();
        if (task_id < 0)
            return;

        if (timestamps_[task_id].find(region) != timestamps_[task_id].end()) {
            timestamps_[task_id][region].first = clock_type::now();
        } else {
            timestamps_[task_id].insert(
                std::make_pair(region, std::make_pair(clock_type::now(), 0.)));
        }
    }

    void stop(const std::string &region)
    {
        // Not initialised thread will be ignored
        auto task_id = local_task_id();
        if (task_id < 0)
            return;

        if (timestamps_[task_id].find(region) != timestamps_[task_id].end()) {
            auto t_start = timestamps_[task_id][region].first;
            timestamps_[task_id][region].second +=
                duration_type(clock_type::now() - t_start).count();
        }
    }

    double time();
    double time(const std::string &region);
    void report() const;

private:
    std::size_t local_task_id()
    {
        auto tid = system_thread_id();
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

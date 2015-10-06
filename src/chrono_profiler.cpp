#include "profilers/chrono_profiler.hpp"
#include "utility/platform.hpp"

void ChronoProfiler::InitParallel()
{
    auto tid = GetSystemThreadID();
    guard_.lock();
    if (tid_map_.find(tid) == tid_map_.end()) {
        tid_map_.insert(std::make_pair(tid, tid_map_.size()));
        timestamps_.push_back(std::map<const std::string,
                              std::pair<clock_type::time_point, double> >());
    }
    guard_.unlock();
}

double ChronoProfiler::Time()
{
    // Accumulate times for all regions of the current thread
    auto task_id = GetLocalTaskID();
    double time = 0.;
    for (auto t : timestamps_[task_id]) {
        time += t.second.second;
    }

    return time;
}

double ChronoProfiler::Time(const std::string &region)
{
    double time = 0.;
    auto task_id = GetLocalTaskID();
    if (task_id >= 0 &&
        timestamps_[task_id].find(region) != timestamps_[task_id].cend())
        time = timestamps_[task_id][region].second;

    return time;
}

void ChronoProfiler::Report() const
{
    for (std::size_t i = 0; i < timestamps_.size(); ++i) {
        double time_per_thread = 0.;
        std::cout << "Thread #" << i << "\n";
        for (auto r : timestamps_[i]) {
            std::cout << "\tRegion \"" << r.first << "\": "
                      << r.second.second << " (s)\n";
            time_per_thread += r.second.second;
        }

        std::cout << "\tTOTAL: " << time_per_thread << " (s)\n";
    }
}

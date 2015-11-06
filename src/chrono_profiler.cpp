#include "profilers/chrono_profiler.hpp"
#include "utility/platform.hpp"

void chrono_profiler::init_parallel()
{
    auto tid = system_thread_id();
    guard_.lock();
    if (tid_map_.find(tid) == tid_map_.end()) {
        tid_map_.insert(std::make_pair(tid, tid_map_.size()));
        timestamps_.push_back(std::map<const std::string,
                              std::pair<clock_type::time_point, double> >());
    }
    guard_.unlock();
}

double chrono_profiler::time()
{
    // Accumulate times for all regions of the current thread
    auto task_id = local_task_id();
    double time = 0.;
    for (auto t : timestamps_[task_id]) {
        time += t.second.second;
    }

    return time;
}

double chrono_profiler::time(const std::string &region)
{
    double time = 0.;
    auto task_id = local_task_id();
    if (task_id >= 0 &&
        timestamps_[task_id].find(region) != timestamps_[task_id].cend())
        time = timestamps_[task_id][region].second;

    return time;
}

void chrono_profiler::report() const
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

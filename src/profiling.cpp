#include "profiling.hpp"
#include "profiler.hpp"

#include <map>

class null_profiler : public profiler
{
public:
    void init_parallel() { }
    void start(const std::string &region) { }
    void stop(const std::string &region) { }
    double time(const std::string &region) { return 0.; }
    double time() { return 0.; }
    void report() const { }
};

static null_profiler no_profiler;

class profiling_framework
{
public:
    static profiling_framework &instance()
    {
        static profiling_framework instance;
        return instance;
    }

    void register_backend(profiler &prof, const std::string &name)
    {
        profilers_.insert(std::make_pair(name, &prof));
    }

    void init_parallel()
    {
        for (auto p : profilers_)
            p.second->init_parallel();
    }

    void start_profiling(const std::string &region)
    {
        for (auto p : profilers_)
            p.second->start(region);
    }

    void stop_profiling(const std::string &region)
    {
        auto p_end = profilers_.rend();
        for (auto pi = profilers_.rbegin(); pi != p_end; ++pi)
            (*pi).second->stop(region);
    }

    profiler &backend(const std::string &name)
    {
        if (profilers_.find(name) == profilers_.end()) {
            return no_profiler;
        }

        return *profilers_[name];
    }

private:
    profiling_framework() { }
    profiling_framework(const profiling_framework &) = delete;
    void operator=(const profiling_framework &) = delete;
    std::map<const std::string, profiler *> profilers_;
};

void profiling_init()
{
    // Just initialize the singleton
    profiling_framework::instance();
}

void profiling_init_parallel()
{
    profiling_framework::instance().init_parallel();
}

void profiling_register_backend(profiler &prof, const std::string &name)
{
    profiling_framework::instance().register_backend(prof, name);
}

profiler &profiling_backend(const std::string &name)
{
    return profiling_framework::instance().backend(name);
}

void profiling_start(const std::string &region)
{
    profiling_framework::instance().start_profiling(region);
}

void profiling_stop(const std::string &region)
{
    profiling_framework::instance().stop_profiling(region);
}

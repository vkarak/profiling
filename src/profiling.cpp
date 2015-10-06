#include "profiling.hpp"
#include "profiler.hpp"

#include <map>

class NullProfiler : public Profiler
{
public:
    void InitParallel() { }
    void Start(const std::string &region) { }
    void Stop(const std::string &region) { }
    double Time(const std::string &region) { return 0.; }
    double Time() { return 0.; }
    void Report() const { }
};

static NullProfiler NoProfiler;

class ProfilingFramework
{
public:
    static ProfilingFramework &GetInstance()
    {
        static ProfilingFramework instance;
        return instance;
    }

    void RegisterProfiler(Profiler &prof, const std::string &name)
    {
        profilers_.insert(std::make_pair(name, &prof));
    }

    void InitParallel()
    {
        for (auto p : profilers_)
            p.second->InitParallel();
    }

    void StartProfiling(const std::string &region)
    {
        for (auto p : profilers_)
            p.second->Start(region);
    }

    void StopProfiling(const std::string &region)
    {
        auto p_end = profilers_.rend();
        for (auto pi = profilers_.rbegin(); pi != p_end; ++pi)
            (*pi).second->Stop(region);
    }

    Profiler &GetProfiler(const std::string &name)
    {
        if (profilers_.find(name) == profilers_.end()) {
            return NoProfiler;
        }

        return *profilers_[name];
    }

private:
    ProfilingFramework() { }
    ProfilingFramework(const ProfilingFramework &) = delete;
    void operator=(const ProfilingFramework &) = delete;
    std::map<const std::string, Profiler *> profilers_;
};

void ProfilingInit()
{
    // Just initialize the singleton
    ProfilingFramework::GetInstance();
}

void ProfilingInitParallel()
{
    ProfilingFramework::GetInstance().InitParallel();
}

void ProfilingRegisterBackend(Profiler &prof, const std::string &name)
{
    ProfilingFramework::GetInstance().RegisterProfiler(prof, name);
}

Profiler &ProfilingGetBackend(const std::string &name)
{
    return ProfilingFramework::GetInstance().GetProfiler(name);
}

void ProfilingStart(const std::string &region)
{
    ProfilingFramework::GetInstance().StartProfiling(region);
}

void ProfilingStop(const std::string &region)
{
    ProfilingFramework::GetInstance().StopProfiling(region);
}

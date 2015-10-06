#ifndef PROFILING_HPP
#define PROFILING_HPP

#include <string>

class Profiler;

// The two functions must be called from a serial context
void ProfilingInit();
void ProfilingRegisterBackend(Profiler &prof, const std::string &name);
Profiler &ProfilingGetBackend(const std::string &name);
void ProfilingInitParallel();
void ProfilingStart(const std::string &region);
void ProfilingStop(const std::string &region);

#endif  // PROFILING_HPP

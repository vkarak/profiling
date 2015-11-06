#ifndef PROFILING_HPP
#define PROFILING_HPP

#include <string>

class profiler;

// The two functions must be called from a serial context
void profiling_init();
void profiling_init_parallel();
void profiling_register_backend(profiler &prof, const std::string &name);
profiler &profiling_backend(const std::string &name);
void profiling_start(const std::string &region);
void profiling_stop(const std::string &region);

#endif  // PROFILING_HPP

#pragma once

#include <string>
#include <iostream>

class Profiler
{
public:
    virtual void InitParallel() = 0;
    virtual void Start(const std::string &region) = 0;
    virtual void Stop(const std::string &region) = 0;
    virtual double Time(const std::string &region) = 0;
    virtual double Time() = 0;
    virtual void Report() const = 0;
    virtual ~Profiler() {};
};

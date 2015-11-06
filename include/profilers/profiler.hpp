#pragma once

#include <string>
#include <iostream>

class profiler
{
public:
    virtual void init_parallel() = 0;
    virtual void start(const std::string &region) = 0;
    virtual void stop(const std::string &region) = 0;
    virtual double time(const std::string &region) = 0;
    virtual double time() = 0;
    virtual void report() const = 0;
    virtual ~profiler() {};
};

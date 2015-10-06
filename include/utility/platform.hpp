#pragma once

#include <cstddef>
#include <unistd.h>
#include <sys/syscall.h>

static std::size_t GetSystemThreadID()
{
    return syscall(SYS_gettid);
}

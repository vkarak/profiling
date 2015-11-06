#pragma once

#include <cstddef>
#include <unistd.h>
#include <sys/syscall.h>

static std::size_t system_thread_id()
{
    return syscall(SYS_gettid);
}

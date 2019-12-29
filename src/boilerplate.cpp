#include "boilerplate.hpp"

std::mutex emit_mutex;

thread_local std::string underliner_base::buffer;

#include <iostream>
#include "dead_code_analysis.h"

analysis_tool_t *
dead_code_analysis_create()
{
    return new dead_code_analysis_t();
}

dead_code_analysis_t::dead_code_analysis_t()
{
}

dead_code_analysis_t::~dead_code_analysis_t()
{
}

bool
dead_code_analysis_t::process_memref(const memref_t &memref)
{
    if (memref.data.type == TRACE_TYPE_READ) {
        num_read_refs_++;
    }

    if (memref.data.type == TRACE_TYPE_WRITE) {
        num_write_refs_++;
    }

    return true;
}

bool
dead_code_analysis_t::print_results()
{
    std::cerr << "DEAD CODE ANALYSIS results:\n";
    std::cerr << "  Number of memory read references: " << num_read_refs_ << "\n";
    std::cerr << "  Number of memory write references: " << num_write_refs_ << "\n";
    return true;
}
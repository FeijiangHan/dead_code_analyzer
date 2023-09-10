#include <stdio.h>
#include <iostream>
#include "analyzer_runner.h"

#define FATAL_ERROR(msg, ...)                               \
    do {                                                    \
        fprintf(stderr, "ERROR: " msg "\n", ##__VA_ARGS__); \
        fflush(stderr);                                     \
        exit(1);                                            \
    } while (0)

#define VERBOSE 0
static analyzer_t *analyzer;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <trace directory> " << std::endl;
        return 1;
    }

    analyzer = new analyzer_runner_t(argv[1], VERBOSE);
    if (!*analyzer) {
        std::string error_string_ = analyzer->get_error_string();
        FATAL_ERROR("failed to initialize analyzer%s%s",
                    error_string_.empty() ? "" : ": ", error_string_.c_str());
    }

    if (!analyzer->run()) {
        std::string error_string_ = analyzer->get_error_string();
        FATAL_ERROR("failed to run analyzer%s%s", error_string_.empty() ? "" : ": ",
                    error_string_.c_str());
    }

    if (!analyzer->print_stats()) {
        std::string error_string_ = analyzer->get_error_string();
        FATAL_ERROR("failed to print results%s%s", error_string_.empty() ? "" : ": ",
                    error_string_.c_str());
    }
    // release analyzer's space
    delete analyzer;

    return 0;
}


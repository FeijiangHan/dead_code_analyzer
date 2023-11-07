#include "../headers/analyzer.h"
#include "../headers/analyzer_runner.h"
#include "../headers/utils.h"
#include "../headers/directory_iterator.h"
#include "../headers/file_reader.h"
#include "../headers/dead_code_analysis.h"

analyzer_runner_t::analyzer_runner_t(std::string tracedir, bool verbose)
{
    worker_count_ = 0;
    skip_instrs_ = false;
    parallel_ = false;

    if (!directory_iterator_t::is_directory(tracedir)) {
        success_ = false;
        error_string_ = "Directory does not exist: " + tracedir;
        return;
    }

    if (!create_analysis_tool()) {
        success_ = false;
        error_string_ = "Failed to create analysis tool";
        return;
    }

    if (!init_file_reader(tracedir, verbose)) {
        success_ = false;
        error_string_ = "Failed to initialize file reader";
        return;
    }

    if (!init_analysis_tool()) {
        success_ = false;
        error_string_ = "Failed to initialize analysis tool";
        return;
    }
}

analyzer_runner_t::~analyzer_runner_t()
{
    destroy_analysis_tool();
}

bool
analyzer_runner_t::create_analysis_tool()
{
    tools_ = new analysis_tool_t *[1];
    tools_[0] = dead_code_analysis_create();
    if (tools_[0] == NULL)
        return false;
    if (!*tools_[0]) {
        std::string tool_error = tools_[0]->get_error_string();
        if (tool_error.empty())
            tool_error = "no error message provided.";
        error_string_ = "Tool failed to initialize: " + tool_error;
        delete tools_[0];
        tools_[0] = NULL;
        return false;
    }
    num_tools_ = 1;
    return true;
}

bool
analyzer_runner_t::init_analysis_tool()
{
    std::string tool_error = tools_[0]->initialize_stream(serial_trace_iter_.get());
    if (!tool_error.empty()) {
        error_string_ = "Tool failed to initialize: " + tool_error;
        delete tools_[0];
        tools_[0] = NULL;
        return false;
    }
    return true;
}

void
analyzer_runner_t::destroy_analysis_tool()
{
    if (!success_)
        return;
    for (int i = 0; i < num_tools_; i++)
        delete tools_[i];
    delete[] tools_;
}


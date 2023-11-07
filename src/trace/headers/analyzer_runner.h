/* analyzer_runner: represent a memory trace analysis tool that can process
 * a trace from multiple inputs: a file, from a raw file, or over a pipe online.
 */

#ifndef _ANALYZER_RUNNER_H_
#define _ANALYZER_RUNNER_H_ 1

#include "analyzer.h"

class analyzer_runner_t : public analyzer_t {
public:
    analyzer_runner_t(std::string tracedir, bool verbose);
    virtual ~analyzer_runner_t();

protected:
    bool
    create_analysis_tool();
    bool
    init_analysis_tool();
    void
    destroy_analysis_tool();
};

#endif /* _ANALYZER_RUNNER_H_ */

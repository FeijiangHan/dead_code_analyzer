#ifndef _DEAD_CODE_ANALYSIS_H_
#define _DEAD_CODE_ANALYSIS_H_ 1

#include "analysis_tool.h"
#include "memref.h"

class dead_code_analysis_t : public analysis_tool_t {
public:
    dead_code_analysis_t();
    virtual ~dead_code_analysis_t();
    bool
    process_memref(const memref_t &memref) override;
    bool
    print_results() override;

protected:
    uint64_t num_read_refs_;
    uint64_t num_write_refs_;
};

/** Creates an instance of a TLB simulator. */
analysis_tool_t *
dead_code_analysis_create(void);

#endif

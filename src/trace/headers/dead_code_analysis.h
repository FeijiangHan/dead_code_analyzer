#ifndef _DEAD_CODE_ANALYSIS_H_
#define _DEAD_CODE_ANALYSIS_H_ 1

#include "analysis_tool.h"
#include "memref.h"
#include<unordered_map>
#include<map>
#include<string>
#include <vector>
#include <algorithm>
#include <utility>

class read_write {
public:
    bool read;
    bool write;
    // the previous instruction
    memref_t obj;

    int index_write;
    read_write() {
        read = false;
        write = false;
        index_write = 0;
    }
};

class deadCode {
public:
    memref_t obj; // key
    int index = -1; // index
    deadCode(){}
    deadCode(memref_t obj, int index){
        obj = obj;
        index = index;
    }
    bool operator < (deadCode & dc) {
        return this->index < dc.index;
    }
};

struct CodeNumber {
    int allCodeNumber = -1;
    int deadCodeNumber_read = 0;
    int deadCodeNumber_write = 0;
};


// Hash function for pair objects
struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
  }
};

class dead_code_analysis_t : public analysis_tool_t {
public:
    // Constructor
    dead_code_analysis_t();
    // Destructor
    virtual ~dead_code_analysis_t();
    // Processes a memory reference
    bool process_memref(const memref_t& memref) override;
    // Prints analysis results 
    bool print_results() override;
protected:
    // Number of read references
    uint64_t num_read_refs_;
    // Number of write references
    uint64_t num_write_refs_;
private:
    // Counts for analysis stats
    CodeNumber number;
    // Map of address reads/writes per thread
    std::unordered_map<std::pair<memref_tid_t,addr_t>, read_write*, pair_hash> addr_read_write;
    // Sorted dead code instructions
    std::vector<deadCode> sorted_deadCode;
    // Instruction corresponding to a memref
    memref_t ins_memref;
    // Adds dead code 
    void addDeadCode(int index, const memref_t obj);
    // Initializes address mapping for thread
    void initAddrReadWrite_thread(memref_t memref);
    // Detects dead code
    void detectDeadCode_thread(memref_t memref);
};

/** Creates an instance of a TLB simulator. */
analysis_tool_t *
dead_code_analysis_create(void);

#endif
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


// 定义pair_hash
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

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

private:
    CodeNumber number;
    std::unordered_map<std::pair<memref_tid_t,addr_t>,read_write*,pair_hash> addr_read_write;
    std::vector<deadCode> sorted_deadCode;
    memref_t ins_memref; // 某读写memref对应的指令
    void addDeadCode(int index,const memref_t obj ); // 添加死指令到数组中

    void initAddrReadWrite_thread(memref_t memref); // 初始化map

    void detectDeadCode_thread(memref_t memref); // 检测死指令逻辑
};
/** Creates an instance of a TLB simulator. */
analysis_tool_t *
dead_code_analysis_create(void);

#endif
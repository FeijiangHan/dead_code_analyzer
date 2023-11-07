#include <iostream>
#include "../headers/dead_code_analysis.h"
#include<unordered_map>
#include<string>
#include <vector>
#include<utility>
#include <algorithm>
#include<bitset>
extern "C" {
#include "xed-interface.h"
}
using namespace std;


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

void dead_code_analysis_t::addDeadCode(int index,const memref_t obj ) {
    deadCode dead_code = deadCode(); // 死指令初始化
    dead_code.index = index;
    dead_code.obj = obj;
    sorted_deadCode.push_back(dead_code);
}

void dead_code_analysis_t::initAddrReadWrite_thread(memref_t memref){
    // 1.构造tid-addr的pair
    std::pair<memref_tid_t,addr_t> pair_tid_addr(memref.data.tid, memref.data.addr);
    // 2.初始化构建map的key
    addr_read_write[pair_tid_addr] = new read_write();
    if (memref.data.type == TRACE_TYPE_READ) {
        addr_read_write[pair_tid_addr]->read = true;
    } else if (memref.data.type == TRACE_TYPE_WRITE) {
        addr_read_write[pair_tid_addr]->write = true;
        addr_read_write[pair_tid_addr]->index_write = number.allCodeNumber;
    }
    // 3.记录本次操作的类型
    addr_read_write[pair_tid_addr]->obj = memref; // read
}

string get_register(xed_decoded_inst_t* xedd) {

    unsigned int i, noperands;
    char tbuf[90];
    const xed_inst_t* xi = xed_decoded_inst_inst(xedd);
    xed_operand_action_enum_t rw;
    xed_uint_t bits;
    noperands = xed_inst_noperands(xi);
    tbuf[0]=0;

    for( i=0; i < noperands ; i++) {
        const xed_operand_t *op = xed_inst_operand(xi, i);
        xed_operand_enum_t op_name = xed_operand_name(op);
        switch (op_name) {
        case XED_OPERAND_MEM0:
        case XED_OPERAND_MEM1:
            return "";
        case XED_OPERAND_REG:
        case XED_OPERAND_REG0:
        case XED_OPERAND_REG1:
        case XED_OPERAND_REG2:
        case XED_OPERAND_REG3:
        case XED_OPERAND_REG4:
        case XED_OPERAND_REG5:
        case XED_OPERAND_REG6:
        case XED_OPERAND_REG7:
        case XED_OPERAND_REG8:
        case XED_OPERAND_REG9:{
            return xed_operand_enum_t2str(op_name);
        }
        default: return "";
        }
    }
}

string get_reg(memref_t memref){
//    // 1. state initialization
//    xed_state_t dstate;
//    // 2. init this state, pointing cpu_mode and address_width
//    xed_state_init(&dstate, XED_MACHINE_MODE_LONG_64,XED_ADDRESS_WIDTH_64b,XED_ADDRESS_WIDTH_64b);
//    // 3. the main container for storing messages after instruction decoding
//    xed_decoded_inst_t xedd;
//    // 4. using dstate to initialize xedd. If not, xed_error=64!
//    xed_decoded_inst_zero_set_mode(&xedd, &dstate);
    // 5. give a default init value
    //xed_error_enum_t xed_error = xed_decode(&xedd,reinterpret_cast<const xed_uint8_t*>(memref.instr.encoding),memref.instr.size);
    // 6. detecting error
    xed_state_t dstate;
    xed_decoded_inst_t xedd;
    //xed_chip_enum_t chip = XED_CHIP_INVALID;
    xed_tables_init();
    xed_state_zero(&dstate);
    dstate.mmode=XED_MACHINE_MODE_LONG_64;
    dstate.stack_addr_width=XED_ADDRESS_WIDTH_64b;
    xed_decoded_inst_zero_set_mode(&xedd, &dstate);
    //xed_decoded_inst_set_input_chip(&xedd, chip);

    printf("Attempting to decode: ");
    for(int i=0; i<memref.instr.size;i++) {
        cout << hex << reinterpret_cast<xed_uint8_t>(memref.instr.encoding[i]) << " ";
    }
    cout << endl;

    xed_error_enum_t xed_error = xed_decode(&xedd,reinterpret_cast<const xed_uint8_t*>(memref.instr.encoding),memref.instr.size);

    if (xed_error != XED_ERROR_NONE) {
        cout << "\033[31m" << "error: " << " code size: " << memref.instr.size << "\033[0m" << endl;
        cout << "\033[31m" << "error code => [" << "\033[0m";
        for (int i = 0; i < memref.instr.size; i++) {
            bitset<8> b(memref.instr.encoding[i]);
            cout << "\033[31m" << b << "\033[0m" << " ";
        }
        cout << "\033[31m" << "]" << "\033[0m" << endl;
        switch(xed_error) {
        case XED_ERROR_BUFFER_TOO_SHORT:
            printf("XED_ERROR_BUFFER_TOO_SHORT: There were not enough bytes in the given buffer\n");
            break;
        case XED_ERROR_GENERAL_ERROR:
            printf("\033[31mXED_ERROR_GENERAL_ERROR: XED could not decode the given instruction\033[0m \n");
            break;
        case XED_ERROR_INVALID_FOR_CHIP:
            printf("XED_ERROR_INVALID_FOR_CHIP: The instruction is not valid for the specified chip\n");
            break;
        case XED_ERROR_BAD_REGISTER:
            printf("XED_ERROR_BAD_REGISTER: XED could not decode the given instruction because an invalid register encoding was used\n");
            break;
        case XED_ERROR_BAD_LOCK_PREFIX:
            printf("XED_ERROR_BAD_LOCK_PREFIX: A lock prefix was found where none is allowed\n");
            break;
        case XED_ERROR_BAD_REP_PREFIX:
            printf("XED_ERROR_BAD_REP_PREFIX: An F2 or F3 prefix was found where none is allowed\n");
            break;
        case XED_ERROR_BAD_LEGACY_PREFIX:
            printf("XED_ERROR_BAD_LEGACY_PREFIX: A 66, F2 or F3 prefix was found where none is allowed\n");
            break;
        case XED_ERROR_BAD_REX_PREFIX:
            printf("XED_ERROR_BAD_REX_PREFIX: A REX prefix was found where none is allowed\n");
            break;
        case XED_ERROR_BAD_EVEX_UBIT:
            printf("XED_ERROR_BAD_EVEX_UBIT: An illegal value for the EVEX.U bit was present in the instruction\n");
            break;
        case XED_ERROR_BAD_MAP:
            printf("XED_ERROR_BAD_MAP: An illegal value for the MAP field was detected in the instruction\n");
            break;
        case XED_ERROR_BAD_EVEX_V_PRIME:
            printf("XED_ERROR_BAD_EVEX_V_PRIME: EVEX.V'=0 was detected in a non-64b mode instruction\n");
            break;
        case XED_ERROR_BAD_EVEX_Z_NO_MASKING:
            printf("XED_ERROR_BAD_EVEX_Z_NO_MASKING: EVEX.Z!=0 when EVEX.aaa==0\n");
            break;
        case XED_ERROR_NO_OUTPUT_POINTER:
            printf("XED_ERROR_NO_OUTPUT_POINTER: The output pointer for xed_agen was zero\n");
            break;
        case XED_ERROR_NO_AGEN_CALL_BACK_REGISTERED:
            printf("XED_ERROR_NO_AGEN_CALL_BACK_REGISTERED: One or both of the callbacks for xed_agen were missing.\n");
            break;
        case XED_ERROR_BAD_MEMOP_INDEX:
            printf("XED_ERROR_BAD_MEMOP_INDEX: Memop indices must be 0 or 1\n");
            break;
        case XED_ERROR_CALLBACK_PROBLEM:
            printf("XED_ERROR_CALLBACK_PROBLEM: The register or segment callback for xed_agen experienced a problem\n");
            break;
        case XED_ERROR_GATHER_REGS:
            printf("XED_ERROR_GATHER_REGS: The index, dest and mask regs for AVX2 gathers must be different\n");
            break;
        case XED_ERROR_INSTR_TOO_LONG:
            printf("XED_ERROR_INSTR_TOO_LONG: Full decode of instruction would exceed 15B\n");
            break;
        case XED_ERROR_INVALID_MODE:
            printf("XED_ERROR_INVALID_MODE: The instruction was not valid for the specified mode\n");
            break;
        case XED_ERROR_BAD_EVEX_LL:
            printf("XED_ERROR_BAD_EVEX_LL: EVEX.LL must not ==3 unless using embedded rounding\n");
            break;
        case XED_ERROR_BAD_REG_MATCH:
            printf("XED_ERROR_BAD_REG_MATCH: Source registers must not match the destination register for this instruction\n");
            break;
        default:
            printf("XED_ERROR_UNKNOWN\n");
            break;
        }
    }
#define BUFLEN  1000
    char buffer[BUFLEN];
    xed_decoded_inst_dump(&xedd,buffer, BUFLEN);
    printf("%s\n",buffer);
    // 6. withdrew messages from xedd
    return get_register(&xedd);
}

int get_memref_type(const memref_t& memref){
    switch (memref.data.type) {
    case TRACE_TYPE_READ:
        printf("\033[32m[type]: TRACE_TYPE_READ,data manipulate\033[0m\n");
        return 3;
    case TRACE_TYPE_WRITE: printf("\033[32m[type]: TRACE_TYPE_WRITE,data manipulate\033[0m\n"); return 3;
    case TRACE_TYPE_PREFETCH: printf("\033[32m[type]: TRACE_TYPE_PREFETCH,data manipulate\033[0m\n"); return 1;
    case TRACE_TYPE_PREFETCHT0: printf("\033[32m[type]: TRACE_TYPE_PREFETCHT0,data manipulate\033[0m\n"); return 1;
    case TRACE_TYPE_PREFETCHT1: printf("\033[32m[type]: TRACE_TYPE_PREFETCHT1,data manipulate\033[0m\n"); return 1;
    case TRACE_TYPE_PREFETCHT2: printf("\033[32m[type]: TRACE_TYPE_PREFETCHT2,data manipulate\033[0m\n"); return 1;
    case TRACE_TYPE_PREFETCHNTA:
        printf("\033[32m[type]: TRACE_TYPE_PREFETCHNTA,data manipulate\033[0m\n");
        return 1;
    case TRACE_TYPE_PREFETCH_READ:
        printf("\033[32m[type]: TRACE_TYPE_PREFETCH_READ,data manipulate\033[0m\n");
        return 1;
    case TRACE_TYPE_PREFETCH_WRITE:
        printf("\033[32m[type]: TRACE_TYPE_PREFETCH_WRITE,data manipulate\033[0m\n");
        return 1;
    case TRACE_TYPE_PREFETCH_INSTR:
        printf("\033[32m[type]: TRACE_TYPE_PREFETCH_INSTR,data manipulate\033[0m\n");
        return 1;
    case TRACE_TYPE_INSTR: printf("\033[34m[type]: TRACE_TYPE_INSTR,instruction access\033[0m\n"); return 0;
    case TRACE_TYPE_INSTR_DIRECT_JUMP:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_DIRECT_JUMP,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_INDIRECT_JUMP:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_INDIRECT_JUMP,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_CONDITIONAL_JUMP:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_CONDITIONAL_JUMP,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_DIRECT_CALL:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_DIRECT_CALL,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_INDIRECT_CALL:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_INDIRECT_CALL,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_RETURN:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_RETURN,instruction access\033[0m\n");
        return 0;
    case TRACE_TYPE_INSTR_BUNDLE:
        printf("\033[34m[type]: TRACE_TYPE_INSTR_BUNDLE，instruction access\033[0m\n");
        return 0;
    default: cout << "\033[34mother type\033[0m" << endl; return 2;
    }
}


void dead_code_analysis_t::detectDeadCode_thread(memref_t memref)
{
    //int type = get_memref_type(memref);

    //if (type == 2) return;

    //cout << endl;
    number.allCodeNumber++; // counts
    if (false) // instruction
    {
//        ins_memref = memref;
//        cout << "\033[34m" << "addr: " << ins_memref.instr.addr << endl
//             << "code: " << "\033[0m" ;
//        for (int i = 0; i < ins_memref.instr.size; i++) {
//            bitset<8> bits(ins_memref.instr.encoding[i]);
//            cout << "\033[34m" << bits << "\033[0m" << " ";
//        }
//        cout << endl;

//        cout << "\033[33m=======BEGIN DECODING=======\033[0m" << endl;
//        string reg = get_reg(ins_memref);
//        cout << "\033[33m[REG]: " << reg << "\033[0m" << endl;
//        cout << "\033[33m=======END DECODING=======\033[0m" << endl << endl;

    }
    if ((memref.data.type >= TRACE_TYPE_INSTR && memref.data.type <= TRACE_TYPE_INSTR_RETURN) ||
        memref.data.type == TRACE_TYPE_INSTR_SYSENTER) {
        cout << "{type: }" << memref.data.type << endl;
        cout << "{encoding_is_new: }" << memref.instr.encoding_is_new << endl;
        cout << "\033[34m" << "addr: " << memref.instr.addr << endl
             << "code: " << "\033[0m" ;
        for (int i = 0; i < memref.instr.size; i++) {
            bitset<8> bits(memref.instr.encoding[i]);
            cout << hex <<"\033[34m" << bits.to_ulong() << "\033[0m" << " ";
        }
        cout << endl;
        cout << "\033[33m=======BEGIN DECODING=======\033[0m" << endl;
        string reg = get_reg(memref);
        cout << "\033[33m[REG]: " << reg << "\033[0m" << endl;
        cout << "\033[33m=======END DECODING=======\033[0m" << endl << endl;
    }
    else if (memref.data.type == TRACE_TYPE_READ || memref.data.type == TRACE_TYPE_WRITE){ // 数据操纵-读or写

//        cout << "\033[33m=======BEGIN DECODING=======\033[0m" << endl;
//        string reg = get_reg(ins_memref);
//        cout << "\033[33m[REG]: " << reg << "\033[0m" << endl;
//        cout << "\033[33m=======END DECODING=======\033[0m" << endl << endl;

        // 1.construct tid-addr的pair
        std::pair<memref_tid_t,addr_t> pair_tid_addr(memref.data.tid, memref.data.addr);
        if (addr_read_write.find(pair_tid_addr) == addr_read_write.end()) {
            initAddrReadWrite_thread(memref); // Initialize the operation to build the key-value in the map
        } else {
            // read
            if (memref.data.type == TRACE_TYPE_READ) {

                if (addr_read_write[pair_tid_addr]->read) {
                    if (addr_read_write[pair_tid_addr]->obj.data.type == TRACE_TYPE_READ) {
                        addDeadCode(number.allCodeNumber,memref);
                        number.deadCodeNumber_read++;
                    }
                } else {
                    // never read
                    addr_read_write[pair_tid_addr]->read = true;
                }
                addr_read_write[pair_tid_addr]->obj = memref; // read
            } else if (memref.data.type == TRACE_TYPE_WRITE) { // write

                if (addr_read_write[pair_tid_addr]->write) {
                    if (addr_read_write[pair_tid_addr]->obj.data.type == TRACE_TYPE_WRITE) {

                        addDeadCode(addr_read_write[pair_tid_addr]->index_write,
                                    addr_read_write[pair_tid_addr]->obj);
                        number.deadCodeNumber_write++;
                    }
                } else {
                    addr_read_write[pair_tid_addr]->write = true;
                }
                addr_read_write[pair_tid_addr]->index_write = number.allCodeNumber;
                addr_read_write[pair_tid_addr]->obj = memref; // write
            }
        }
    }
}



std::ostream& operator<<(std::ostream& out, const deadCode code) {
    if (code.obj.data.type == TRACE_TYPE_READ) {
        out << "[dead read] "
            //<< " pid: " << code.obj.data.pid
           // << " tid: " << code.obj.data.tid
            << " addr: " << code.obj.data.addr
            << " exec index: " << code.index
            << " tid: " << code.obj.data.tid << endl;
    } else if (code.obj.data.type == TRACE_TYPE_WRITE) {
        out << "[dead write] "
            //<< " pid: " << code.obj.data.pid
           // << " tid: " << code.obj.data.tid
            << " addr: " << code.obj.data.addr
            << " exec index: " << code.index
            << " tid: " << code.obj.data.tid << endl;
    }
    return out;
}

int count_ = 0;
bool
dead_code_analysis_t::process_memref(const memref_t &memref)
{
    if (count_++ > 100) return true;
    bool isRead = memref.data.type == TRACE_TYPE_READ;
    bool isWrite = memref.data.type == TRACE_TYPE_WRITE;
    if (isRead) {
        num_read_refs_++;
    }

    if (isWrite) {
        num_write_refs_++;
    }

    // detect dead code
    detectDeadCode_thread(memref);
    return true;
}


bool
dead_code_analysis_t::print_results()
{
    std::cerr << "DEAD CODE ANALYSIS results:\n";
    std::cerr << "  Number of memory read references: " << num_read_refs_ << "\n";
    std::cerr << "  Number of memory write references: " << num_write_refs_ << "\n";
    std::cerr << "  Number of deadCode: " << number.deadCodeNumber_read + number.deadCodeNumber_write<< "\n";
    std::cerr << "  Number of deadCode_read: " << number.deadCodeNumber_read << "\n";
    std::cerr << "  Number of deadCode_write: " << number.deadCodeNumber_write << "\n";

    sort(sorted_deadCode.begin(),sorted_deadCode.end());
//    int threshold = 0;
//    for (deadCode dc : sorted_deadCode) {
//        threshold++;
//        //if (threshold > 1000) break;
//        if (dc.obj.data.tid != 1719843)
//            cout << dc;
//    }
    return true;
}
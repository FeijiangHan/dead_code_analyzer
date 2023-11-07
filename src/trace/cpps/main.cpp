#include <stdio.h>
#include <iostream>
#include "../headers/analyzer_runner.h"

/*
 * 检查命令行参数是否正确，如果不正确则输出使用说明并退出程序
创建一个analyzer_runner_t类型的对象，用来运行分析器
调用analyzer_runner_t的run()方法运行分析器，如果返回值为false则表示运行失败，输出错误信息并退出程序
调用analyzer_runner_t的print_stats()方法输出分析结果，如果返回值为false则表示输出结果失败，输出错误信息并退出程序。
释放analyzer_runner_t对象所占用的空间并结束程序.
这段代码还包含了一些宏定义，如FATAL_ERROR，用于在程序发生错误时输出错误信息并终止程序。
 * */

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


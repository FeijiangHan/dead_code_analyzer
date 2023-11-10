# DeadCode Detection Tool

**Using DynamoRIO and XED to find redundant executions (Dead code)**

![poster-Feijiang Han](https://gitee.com/han-feijiang/img-store/raw/master/2023/poster-Feijiang%20Han.png)

> [!IMPORTANT]
> This tool aims to detect and analyze dead code (redundant executions) in binary files using DynamoRIO and XED.
> Here, I implement a basic logic to detect dead writes and reads

**The process can be summarized as follows:**

1. Obtain the binary file (trace) that needs to be parsed.
2. Extract the necessary code from DynamoRIO for parsing the target binary. This step gathers detailed information about instructions, such as read/write operations and parameter details.
3. Utilize [Intel XED](https://intelxed.github.io/) to decode register details and analyze fine-grained parameters.
4. Modify the dead code detection logic in [dead_code_analysis.cpp](https://github.com/FeijiangHan/DeadCodeDetection/blob/main/src/trace/cpps/dead_code_analysis.cpp). The implementation includes detecting redundant reads and redundant writes.
5. The results of the analysis will be output to the 'out' file, which can be further analyzed.

**TODO:**

- Optimize the output format for better readability.
- Implement more sophisticated dead code detection logic.

For more information and access to the source code, please refer to the project repository.

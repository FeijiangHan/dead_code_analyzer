<div class="column" align="middle">
  <p align="center">
  </p>
  </a>
  <img src="https://img.shields.io/badge/License-Apache%202.0-red.svg" alt="license"/>
  <a href="https://golang.org/">
    <img src="https://img.shields.io/badge/Language-C++-blue.svg" alt="language"/>
  </a>
  <img src="https://img.shields.io/badge/platform-Linux-9cf.svg" alt="linux"/>
  <img src="https://img.shields.io/badge/Release-v0.1.0-green.svg" alt="release"/>

<h5 align="center">If you are interested in This project, please kindly give Me a triple `Star`, `Fork` and `Watch`, Thanks!</h5>
</div>

# DeadCode Detection Framework

**Using DynamoRIO and XED to find redundant executions (Dead code)**
* **This project can be used as a framework for dead code analysis.**
* **This project can also be seen as introductory training to help students or beginners explore some instruction-level programming work**

> [!IMPORTANT]
> 1. This tool aims to detect and analyze dead code (redundant executions) in binary files using DynamoRIO and XED.
> 2. Here, I only implement a basic logic to detect dead writes and reads
> 3. You have the freedom to modify the detection logic in [dead_code_analysis.cpp](https://github.com/FeijiangHan/DeadCodeDetection/blob/main/src/trace/cpps/dead_code_analysis.cpp) to achieve better dead code detection than what is currently implemented.
> 4. You can also modify the code output logic to use CSV or any other file you want
> 5. This project does not read files using C++, so you need to configure the compiler to specify the binary path in order to get the binary into the program. I feel that Clion is more convenient, VScode is slightly more complicated


**Here is a poster of me showcasing my work during NCSU GEARE training:**
![poster-Feijiang Han](https://github.com/FeijiangHan/dead_code_analyzer/blob/main/poster-Feijiang%20Han.png)


**The process can be summarized as follows:**

1. Obtain the binary file (trace) that needs to be parsed.
2. Extract the necessary code from DynamoRIO for parsing the target binary. This step gathers detailed information about instructions, such as read/write operations and parameter details.
3. Utilize [Intel XED](https://intelxed.github.io/) to decode register details and analyze fine-grained parameters.
4. Modify the dead code detection logic in [dead_code_analysis.cpp](https://github.com/FeijiangHan/DeadCodeDetection/blob/main/src/trace/cpps/dead_code_analysis.cpp). The implementation includes detecting redundant reads and redundant writes.
5. The analysis results will be output to the 'out' file, which can be further analyzed.


**TODO:**

- Optimize the output format for better readability.
- Implement more sophisticated dead code detection logic.

Please refer to the project repository for more information and access to the source code.

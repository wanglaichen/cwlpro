#include <iostream>
#include <cstdlib> // 用于调用系统命令
#include <string>  // 用于处理字符串

int main() {
    std::string target = "baidu.com";
    std::string pingCommand;

    // 根据不同操作系统构造ping命令
    #ifdef _WIN32
        // Windows系统，默认发送4个数据包
        pingCommand = "ping -n 4 " + target;
    #else
        // Linux/Unix系统，默认发送4个数据包
        pingCommand = "ping -c 4 " + target;
    #endif

    std::cout << "正在执行ping命令: " << pingCommand << std::endl;
    std::cout << "====================================" << std::endl;

    // 调用系统命令执行ping
    int result = system(pingCommand.c_str());

    std::cout << "====================================" << std::endl;
    if (result == 0) {
        std::cout << "ping命令执行成功" << std::endl;
    } else {
        std::cout << "ping命令执行失败，错误代码: " << result << std::endl;
    }

    return result;
}
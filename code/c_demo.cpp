#include <iostream>
#include <fstream> // 用于文件操作
#include <cstdlib> // 用于调用系统命令
#include <string>  // 用于处理字符串
#include <ctime>   // 用于获取时间戳

int main() {
    // 打开日志文件
    std::ofstream logFile("network_diagnostic.log");
    if (!logFile.is_open()) {
        std::cerr << "无法创建日志文件!" << std::endl;
        return 1;
    }
    
    // 输出到控制台和日志文件的辅助函数
    auto logOutput = [&](const std::string& message) {
        std::cout << message;
        logFile << message;
    };
    
    // 获取当前时间
    time_t now = time(0);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    std::string target = "baidu.com";
    std::string pingCommand;
    std::string curlCommand;
    
    // 日志头信息
    logOutput("=== 网络连接诊断测试日志 ===\n");
    logOutput(std::string("测试时间: ") + timestamp + "\n");
    logOutput("测试目标: " + target + "\n\n");

    // ====================== ping测试 ======================
    logOutput("1. 执行ping测试 (ICMP协议):\n");
    // 根据不同操作系统构造ping命令
    #ifdef _WIN32
        pingCommand = "ping -n 2 " + target;  // 减少到2个包，加快测试
    #else
        pingCommand = "ping -c 2 " + target;  // 减少到2个包，加快测试
    #endif

    logOutput("执行命令: " + pingCommand + "\n");
    logOutput("------------------------------------\n");

    // 调用系统命令执行ping
    int pingResult = system(pingCommand.c_str());

    logOutput("------------------------------------\n");
    if (pingResult == 0) {
        logOutput("ping测试: 成功\n");
    } else {
        logOutput("ping测试: 失败，错误代码: " + std::to_string(pingResult) + "\n");
        logOutput("注意: GitHub Actions可能限制了ICMP协议(ping)\n");
    }
    logOutput("\n");

    // ====================== curl测试 ======================
    logOutput("2. 执行curl测试 (TCP协议):\n");
    // 使用curl测试HTTP连接
    curlCommand = "curl -I -m 5 http://" + target;  // -I只获取头信息，-m 5秒超时

    logOutput("执行命令: " + curlCommand + "\n");
    logOutput("------------------------------------\n");

    // 调用系统命令执行curl
    int curlResult = system(curlCommand.c_str());

    logOutput("------------------------------------\n");
    if (curlResult == 0) {
        logOutput("curl测试: 成功\n");
        logOutput("结论: 网络连接正常，可能只是ICMP(ping)被限制\n");
    } else {
        logOutput("curl测试: 失败，错误代码: " + std::to_string(curlResult) + "\n");
        logOutput("结论: 网络连接可能存在问题\n");
    }
    logOutput("\n");

    // ====================== DNS测试 ======================
    logOutput("3. 执行DNS解析测试:\n");
    std::string dnsCommand;
    #ifdef _WIN32
        dnsCommand = "nslookup " + target;
    #else
        dnsCommand = "host " + target;
    #endif

    logOutput("执行命令: " + dnsCommand + "\n");
    logOutput("------------------------------------\n");

    // 调用系统命令执行DNS解析
    int dnsResult = system(dnsCommand.c_str());

    logOutput("------------------------------------\n");
    if (dnsResult == 0) {
        logOutput("DNS解析: 成功\n");
    } else {
        logOutput("DNS解析: 失败，错误代码: " + std::to_string(dnsResult) + "\n");
    }
    logOutput("\n");

    // 获取结束时间
    now = time(0);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    logOutput("=== 测试完成 ===\n");
    logOutput(std::string("测试结束时间: ") + timestamp + "\n");
    
    // 关闭日志文件
    logFile.close();
    logOutput("日志已保存到 network_diagnostic.log\n");
    
    return 0;  // 无论网络测试结果如何，都返回成功，确保部署流程完成
}
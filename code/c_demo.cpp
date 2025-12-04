#include <iostream>
#include <cstdlib> // 用于调用系统命令
#include <string>  // 用于处理字符串

int main() {
    std::string target = "baidu.com";
    std::string pingCommand;
    std::string curlCommand;

    std::cout << "=== 网络连接诊断测试 ===" << std::endl;
    std::cout << "测试目标: " << target << std::endl;
    std::cout << std::endl;

    // ====================== ping测试 ======================
    std::cout << "1. 执行ping测试 (ICMP协议):" << std::endl;
    // 根据不同操作系统构造ping命令
    #ifdef _WIN32
        pingCommand = "ping -n 2 " + target;  // 减少到2个包，加快测试
    #else
        pingCommand = "ping -c 2 " + target;  // 减少到2个包，加快测试
    #endif

    std::cout << "执行命令: " << pingCommand << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // 调用系统命令执行ping
    int pingResult = system(pingCommand.c_str());

    std::cout << "------------------------------------" << std::endl;
    if (pingResult == 0) {
        std::cout << "ping测试: 成功" << std::endl;
    } else {
        std::cout << "ping测试: 失败，错误代码: " << pingResult << std::endl;
        std::cout << "注意: GitHub Actions可能限制了ICMP协议(ping)" << std::endl;
    }
    std::cout << std::endl;

    // ====================== curl测试 ======================
    std::cout << "2. 执行curl测试 (TCP协议):" << std::endl;
    // 使用curl测试HTTP连接
    curlCommand = "curl -I -m 5 http://" + target;  // -I只获取头信息，-m 5秒超时

    std::cout << "执行命令: " << curlCommand << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // 调用系统命令执行curl
    int curlResult = system(curlCommand.c_str());

    std::cout << "------------------------------------" << std::endl;
    if (curlResult == 0) {
        std::cout << "curl测试: 成功" << std::endl;
        std::cout << "结论: 网络连接正常，可能只是ICMP(ping)被限制" << std::endl;
    } else {
        std::cout << "curl测试: 失败，错误代码: " << curlResult << std::endl;
        std::cout << "结论: 网络连接可能存在问题" << std::endl;
    }
    std::cout << std::endl;

    // ====================== DNS测试 ======================
    std::cout << "3. 执行DNS解析测试:" << std::endl;
    std::string dnsCommand;
    #ifdef _WIN32
        dnsCommand = "nslookup " + target;
    #else
        dnsCommand = "host " + target;
    #endif

    std::cout << "执行命令: " << dnsCommand << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // 调用系统命令执行DNS解析
    int dnsResult = system(dnsCommand.c_str());

    std::cout << "------------------------------------" << std::endl;
    if (dnsResult == 0) {
        std::cout << "DNS解析: 成功" << std::endl;
    } else {
        std::cout << "DNS解析: 失败，错误代码: " << dnsResult << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== 测试完成 ===" << std::endl;
    return 0;  // 无论网络测试结果如何，都返回成功，确保部署流程完成
}
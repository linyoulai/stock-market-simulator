// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
// main.cpp
// 项目的唯一入口点

#include <iostream>
#include "./include/Market.h"
#include "./include/CommandLine.h"

int main(int argc, char** argv) {
    // 1. 创建一个对象来存储命令行设置
    CommandLineArgs args;
    // 2. 解析命令行参数，填充args对象
    parse_command_line(argc, argv, args);
    
    // 输出args 看看解析是否正确
    /*
    std::cout << "verbose: " << args.verbose << std::endl;
    std::cout << "median: " << args.median << std::endl;
    std::cout << "trader_info: " << args.trader_info << std::endl;
    std::cout << "time_traveler: " << args.time_traveler << std::endl;
    */

    // 3. 创建市场总控对象
    Market market(args);
    // 4. 启动市场模拟
    market.run();
    std::cout << "Market simulation completed." << std::endl;

    return 0;
}
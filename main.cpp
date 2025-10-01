// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
// main.cpp
// 项目的唯一入口点

#include <iostream>
#include "Market.h"
#include "CommandLine.h"

// ./market -vmit < /home/lyl/projects/market_sim_yl/spec-input-PR.txt
// ./market_debug -vmit < /home/lyl/projects/market_sim_yl/spec-input-PR.txt

int main(int argc, char** argv) {
    // 设置I/O加速
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
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

    return 0;
}
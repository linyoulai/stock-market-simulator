// main.cpp
// 项目的唯一入口点

#include <iostream>
#include "Market.h"
#include "CommandLine.h"

int main(int argc, char** argv) {
    // 1. 创建一个对象来存储命令行设置
    CommandLineArgs args;
    // 2. 解析命令行参数，填充args对象
    parse_command_line(argc, argv, args);
    // 3. 创建市场总控对象
    Market market(args);
    // 4. 启动市场模拟
    market.run();

    return 0;
}
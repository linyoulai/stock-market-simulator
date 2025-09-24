#ifndef MARKET_H
#define MARKET_H

#include "CommandLine.h"
#include "Stock.h"
#include "Trader.h"
#include <vector>
#include <string>

class Market {
public:
    Market(const CommandLineArgs& args);
    void run();

private:
    // 处理文件头，包括TL模式的前4行和PR模式的前7行
    void process_file_header();
    // 开始处理订单，这里TL模式和PR模式是一样的
    void process_orders(std::istream &inputStream);

    // 闭市，打印今日报告
    void print_final_reports();
    void print_summary_output();// 打印摘要报告
    void print_trader_info_output();// 打印交易员信息报告
    void print_time_traveler_output();// 打印时间旅行报告

    
    CommandLineArgs args; // 命令行参数
    std::string mode;
    std::vector<Trader> traders;
    std::vector<Stock> stocks;
    int current_timestamp = 0;
    long long trades_completed = 0;
    long long order_counter = 0; // 用于生成唯一的订单ID
};

#endif // MARKET_H
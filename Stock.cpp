#include "Stock.h"
#include <iostream>

void Stock::process_order(const Order& new_order, std::vector<Trader>& traders, bool verbose, long long& trades_completed) {
    // 简单的处理逻辑：将订单添加到订单队列
    (void)new_order;
    (void)traders;
    (void)verbose;
    (void)trades_completed;
}

void Stock::print_median_report(int timestamp) const {
    // 打印当前股票的中位数报告
    std::cout << "Median report for stock " << timestamp << std::endl;
}

void Stock::print_time_traveler_report() const {
    // 打印当前股票的时间旅行者报告
    time_traveler.print_report(stock_id);
    
}

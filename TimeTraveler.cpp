#include "TimeTraveler.h"
#include <iostream>


void TimeTraveler::process_order(const Order& new_order) {
    // 简单的处理逻辑：将订单添加到订单队列
    (void)new_order;
}

void TimeTraveler::print_report(const int stock_id) const {
    // 打印当前股票的时间旅行者报告
    std::cout << "Time traveler report for stock " << stock_id << std::endl;
}

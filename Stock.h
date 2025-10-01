// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#ifndef STOCK_H
#define STOCK_H

#include "Order.h"
#include "Trader.h"
#include "TimeTraveler.h"
#include "CommandLine.h"
#include <queue>
#include <vector>
#include <functional>//

class Stock {
public:

    void process_order(Order* new_order, std::vector<Trader>& traders, const CommandLineArgs& args, long long& trades_completed);
    void print_median_report(int timestamp) const;
    void print_time_traveler_report() const;

    bool check_match(Order* new_order, std::vector<Trader>& traders, const CommandLineArgs& args, long long& trades_completed);
    int stock_id;

private:

    // 两个优先队列，存储订单
    std::priority_queue<Order*, std::vector<Order*>, BuyOrderComparator> buy_orders_queue;
    std::priority_queue<Order*, std::vector<Order*>, SellOrderComparator> sell_orders_queue;
    
    // TODO: 添加用于计算中位数的数据结构，两个优先队列，根据成交价格分为高价和低价，低价队列的大小等于高价队列的大小或加1
    // BUG2:Comparator用错
    std::priority_queue<int, std::vector<int>, std::less<int>> lower_prices_queue; // 低价队列
    std::priority_queue<int, std::vector<int>, std::greater<int>> higher_prices_queue; // 高价队列
    
    // TODO: 添加用于时间旅行者的数据结构
    TimeTraveler time_traveler;

};

#endif // STOCK_H
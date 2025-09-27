// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#ifndef TIME_TRAVELER_H
#define TIME_TRAVELER_H

#include "Order.h"

// 每种股票都有一个timetraveler来记录这种股票的最大收益
class TimeTraveler {
public:
    void process_order(const Order& new_order);
    void print_report(const int stock_id) const;

private:
    // 记录最大利润和最大利润的时间戳
    int max_profit = 0;
    int max_profit_buy_timestamp = -1;
    int max_profit_sell_timestamp = -1;
    int max_profit_buy_price = -1;
    int max_profit_sell_price = -1;

    // 买单来时，就比较，如果利润更高，就更新最大利润
    // 卖单来时，就比较，如果价格更低，就更新最低价格
    int lowest_sell_price_so_far = -1;
    int lowest_sell_price_timestamp = -1;
};


#endif
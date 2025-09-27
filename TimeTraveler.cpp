// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#include "./include/TimeTraveler.h"
#include <iostream>


void TimeTraveler::process_order(const Order& new_order) {
    if (new_order.is_buy) {
        if (lowest_sell_price_so_far != -1 && new_order.price > lowest_sell_price_so_far) {
            // 利润更高，更新最大利润
            int profit = new_order.price - lowest_sell_price_so_far;
            if (profit > max_profit) {
                max_profit = profit;
                max_profit_sell_timestamp = lowest_sell_price_timestamp;
                max_profit_sell_price = lowest_sell_price_so_far;
                max_profit_buy_timestamp = new_order.timestamp;
                max_profit_buy_price = new_order.price;
            }
        }

    } else {
        // 卖单来时，就比较，如果价格更低，就更新最低价格
        if (lowest_sell_price_so_far == -1 || new_order.price < lowest_sell_price_so_far) {
            lowest_sell_price_so_far = new_order.price;
            lowest_sell_price_timestamp = new_order.timestamp;
        }
    }
}

/*
A time traveler would buy Stock 0 at time 1 for $56 and sell it at time 2 for $73
A time traveler could not make a profit on Stock 1
*/
void TimeTraveler::print_report(const int stock_id) const {
    if (max_profit > 0) {
        std::cout << "A time traveler would buy Stock " << stock_id 
        << " at time " << max_profit_sell_timestamp << " for $" 
        << max_profit_sell_price << " and sell it at time " 
        << max_profit_buy_timestamp << " for $" << max_profit_buy_price << std::endl;
    } else {
        std::cout << "A time traveler could not make a profit on Stock " << stock_id << std::endl;
    }
}

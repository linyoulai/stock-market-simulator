// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#include "./include/Stock.h"
#include "./include/CommandLine.h"
#include "./include/debug.h"
#include "./include/Trader.h"
#include <iostream>

void Stock::process_order(const Order& new_order_const, std::vector<Trader>& traders, const CommandLineArgs& args, long long& trades_completed) {
    // 时间旅行者处理订单
    if (args.time_traveler) {
        time_traveler.process_order(new_order_const);
    }

    Order new_order = new_order_const;

    // 判断订单是否匹配
    bool is_match = check_match(new_order, traders, args);
    if (is_match) {
        trades_completed++;
    }
}

bool Stock::check_match(Order& new_order, std::vector<Trader>& traders, const CommandLineArgs& args) {
    bool trade_occurred = false;
    // 检查订单是否匹配
    if (new_order.is_buy) {
        // 买单还有数量，并且卖单队列不为空，且卖单价格低于买单价格
        while (new_order.quantity > 0 && !sell_orders_queue.empty() && new_order.price >= sell_orders_queue.top().price) {
            trade_occurred = true;

            // 复制一份，并出队
            Order matched_order = sell_orders_queue.top();
            sell_orders_queue.pop();

            int trade_price = matched_order.price;
            int trade_quantity = std::min(new_order.quantity, matched_order.quantity);

            traders[new_order.trader_id].record_buy(trade_quantity, trade_price);
            traders[matched_order.trader_id].record_sell(trade_quantity, trade_price);

            // Trader 1 purchased 19 shares of Stock 1 from Trader 2 for $49/share
            if (args.verbose) {
                std::cout << "Trader " << new_order.trader_id 
                << " purchased " << trade_quantity 
                << " shares of Stock " << this->stock_id 
                << " from Trader " << matched_order.trader_id 
                << " for $" << trade_price << "/share" << std::endl;
            }

            if (args.median) {
                if (lower_prices_queue.empty()) {
                    lower_prices_queue.push(trade_price);
                } else {
                    if (trade_price < lower_prices_queue.top()) {
                        lower_prices_queue.push(trade_price);
                    } else {
                        higher_prices_queue.push(trade_price);
                    }
                }
                // 保持低价队列元素个数大于等于高价队列，最多大1
                if (lower_prices_queue.size() > higher_prices_queue.size() + 1) {
                    higher_prices_queue.push(lower_prices_queue.top());
                    lower_prices_queue.pop();
                } else if (higher_prices_queue.size() > lower_prices_queue.size()) {
                    lower_prices_queue.push(higher_prices_queue.top());
                    higher_prices_queue.pop();
                }
            }

            new_order.quantity -= trade_quantity;
            matched_order.quantity -= trade_quantity;

            // 如果卖单还有数量，加入卖单队列
            if (matched_order.quantity > 0) {
                sell_orders_queue.push(matched_order);
            }
        }
        // 买单还有数量，但是卖单队列空了，加入买单队列
        if (new_order.quantity > 0) {
            buy_orders_queue.push(new_order);
        }
    } else {
        // 卖单还有数量，并且买单队列不为空，且买单价格高于卖单价格
        while (new_order.quantity > 0 && !buy_orders_queue.empty() && new_order.price <= buy_orders_queue.top().price) {
           trade_occurred = true;

            // 复制一份，并出队
            Order matched_order = buy_orders_queue.top();
            buy_orders_queue.pop();

            int trade_price = matched_order.price;
            int trade_quantity = std::min(new_order.quantity, matched_order.quantity);

            traders[new_order.trader_id].record_sell(trade_quantity, trade_price);
            traders[matched_order.trader_id].record_buy(trade_quantity, trade_price);

            // Trader 1 sold 19 shares of Stock 1 to Trader 2 for $49/share
            if (args.verbose) {
                std::cout << "Trader " << matched_order.trader_id 
                            << " purchased " << trade_quantity 
                          << " shares of Stock " << new_order.stock_id << " from Trader " 
                          << new_order.trader_id << " for $" << trade_price << "/share\n";
            }

            if (args.median) {
                if (lower_prices_queue.empty()) {
                    lower_prices_queue.push(trade_price);
                } else {
                    if (trade_price < lower_prices_queue.top()) {
                        lower_prices_queue.push(trade_price);
                    } else {
                        higher_prices_queue.push(trade_price);
                    }
                }
                // 保持低价队列元素个数大于等于高价队列，最多大1
                if (lower_prices_queue.size() > higher_prices_queue.size() + 1) {
                    higher_prices_queue.push(lower_prices_queue.top());
                    lower_prices_queue.pop();
                } else if (higher_prices_queue.size() > lower_prices_queue.size()) {
                    lower_prices_queue.push(higher_prices_queue.top());
                    higher_prices_queue.pop();
                }
            }

            new_order.quantity -= trade_quantity;
            matched_order.quantity -= trade_quantity;

            // 如果买单还有数量，加入买单队列
            if (matched_order.quantity > 0) {
                buy_orders_queue.push(matched_order);
            }
        }
        // 卖单还有数量，但是买单队列空了，加入卖单队列
        if (new_order.quantity > 0) {
            sell_orders_queue.push(new_order);
        }
    }

    return trade_occurred;
}
    

// Median match price of Stock 0 at time 2 is $45
void Stock::print_median_report(int timestamp) const {
    if (lower_prices_queue.empty()) {
        return;
    }
    DEBUGOUT("DEBUGOUT--------------median report---------------" << std::endl);
    // 输出当前股票的队列大小
    DEBUGOUT("lower_prices_queue.size() = " << lower_prices_queue.size() << std::endl);
    DEBUGOUT("higher_prices_queue.size() = " << higher_prices_queue.size() << std::endl);
    // 打印当前股票的中位数报告
    int median_price = 0;
    if (lower_prices_queue.size() == higher_prices_queue.size()) {
        median_price = (lower_prices_queue.top() + higher_prices_queue.top()) / 2;
    } else {
        median_price = lower_prices_queue.top();
    }
    std::cout << "Median match price of Stock " << this->stock_id 
    << " at time " << timestamp << " is $" << median_price << std::endl;
}

void Stock::print_time_traveler_report() const {
    // 打印当前股票的时间旅行者报告
    time_traveler.print_report(stock_id);
}

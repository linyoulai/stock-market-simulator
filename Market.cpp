// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
// Market.cpp
#include "./include/Market.h"
#include "./include/P2random.h"
#include "./include/debug.h"
#include "./include/Trader.h"
#include <iostream>
#include <string>
#include <sstream>


Market::Market(const CommandLineArgs& args) : args(args) {

}

void Market::run() {
    std::cout << "Processing orders..." << std::endl;
    // 启动市场模拟
    process_file_header();
    print_final_reports();
}

/*
COMMENT: Spec example, TL mode generating 12 orders.
MODE: TL
NUM_TRADERS: 3
NUM_STOCKS: 2
*/
void Market::process_file_header() {
    // comment -> junk
    std::string junk, mode;
    getline(std::cin, junk);

    // First read Input File Header (mode, num_traders, num_stocks)
    int num_traders, num_stocks;
    std::cin >> junk >> mode;
    std::cin >> junk >> num_traders;
    std::cin >> junk >> num_stocks;

    // 输出看看是否接收成功：
    DEBUGOUT("DEBUGOUT--------------file header---------------" << std::endl);
    DEBUGOUT("mode: " << mode << std::endl);
    DEBUGOUT("num_traders: " << num_traders << std::endl);
    DEBUGOUT("num_stocks: " << num_stocks << std::endl);

    /*
    std::cout << "mode: " << mode << std::endl;
    std::cout << "num_traders: " << num_traders << std::endl;
    std::cout << "num_stocks: " << num_stocks << std::endl;
    */

    // init traders and stocks
    this->mode = mode;
    traders.resize(num_traders);
    stocks.resize(num_stocks);
    
    // Create a stringstream object in case the PROG is used
    std::stringstream ss;
    
    if (mode == "PR") {
        /*
        RANDOM_SEED: <RANDOM_SEED>
        NUMBER_OF_ORDERS: <NUM_ORDERS>
        ARRIVAL_RATE: <ARRIVAL_RATE>
        */
        // TODO: Read PR mode values from cin (seed, num_orders, arrival_rate)
        unsigned int seed, num_orders, arrival_rate;
        std::cin >> junk >> seed;
        std::cin >> junk >> num_orders;
        std::cin >> junk >> arrival_rate;

        // 输出看看是否接收成功：
        DEBUGOUT("DEBUGOUT--------------IF PR mode---------------" << std::endl);
        DEBUGOUT("seed: " << seed << std::endl);
        DEBUGOUT("num_orders: " << num_orders << std::endl);
        DEBUGOUT("arrival_rate: " << arrival_rate << std::endl);
        
        // Initialize the PROG and populate ss with orders
        P2random::PR_init(ss, seed, num_traders, num_stocks, num_orders, arrival_rate);
    }  // if ..mode

    // Call the function with either the stringstream produced by PR_init()
    // or cin
    if (mode == "PR") {
        this->process_orders(ss);  // This is a separate function you must write
    }
    else if (mode == "TL") {
        this->process_orders(std::cin); 
    }
    else {
        std::cerr << "Error: Invalid mode. Please use 'PR' or 'TL'." << std::endl;
        exit(1);
    }
}



/*
<TIMESTAMP> <BUY/SELL> T<TRADER_ID> S<STOCK_ID> $<PRICE> #<QUANTITY>
0 BUY T1 S2 $100 #50
At timestamp 0, trader 1 is willing to buy 50 shares of stock 2 for up to $100/share.
*/
void Market::process_orders(std::istream &inputStream) {
    // 处理订单，包括TL模式和PR模式
    int timestamp;
    std::string buy_or_sell;
    char prefix_T, prefix_S, prefix_dollar, prefix_sharp;
    int trader_id, stock_id, price, quantity;
    
    while (inputStream >> timestamp >> buy_or_sell 
        >> prefix_T >> trader_id >> prefix_S >> stock_id 
        >> prefix_dollar >> price >> prefix_sharp >> quantity) {
        // 输出看看是否接收成功：
        DEBUGOUT("DEBUGOUT--------------check input---------------" << std::endl);
        DEBUGOUT("timestamp: " << timestamp << std::endl);
        DEBUGOUT("buy_or_sell: " << buy_or_sell << std::endl);
        DEBUGOUT("trader_id: " << trader_id << std::endl);
        DEBUGOUT("stock_id: " << stock_id << std::endl);
        DEBUGOUT("price: " << price << std::endl);
        DEBUGOUT("quantity: " << quantity << std::endl);

        // --- 1. 输入验证 (直接根据项目规格) ---
        // 只有 TL 模式需要检查，PR 模式保证是合法的
        if (this->mode == "TL") {
            if (timestamp < 0) {
                std::cerr << "Error: Negative timestamp" << std::endl;
                exit(1);
            }
            if (timestamp < this->current_timestamp) {
                std::cerr << "Error: Decreasing timestamp" << std::endl;
                exit(1);
            }
            if (trader_id < 0 || static_cast<size_t>(trader_id) >= traders.size()) {
                std::cerr << "Error: Invalid trader ID" << std::endl;
                exit(1);
            }
            if (stock_id < 0 || static_cast<size_t>(stock_id) >= stocks.size()) {
                std::cerr << "Error: Invalid stock ID" << std::endl;
                exit(1);
            }
            if (price <= 0) {
                std::cerr << "Error: Invalid price" << std::endl;
                exit(1);
            }
            if (quantity <= 0) {
                std::cerr << "Error: Invalid quantity" << std::endl;
                exit(1);
            }
        }
        // --- 2. 时间戳检查与中位数报告 ---
        if (timestamp > this->current_timestamp) {
            // 打印当前时间点的中位数报告
            if (args.median) {
                for (size_t i = 0; i < stocks.size(); i++) {
                    stocks[i].print_median_report(this->current_timestamp);
                }
            }
            // 更新当前时间点
            this->current_timestamp = timestamp;
        }
        // --- 3. 创建并分派订单 ---
        Order new_order;
        new_order.timestamp = timestamp;
        new_order.is_buy = (buy_or_sell == "BUY");
        new_order.trader_id = trader_id;
        new_order.stock_id = stock_id;
        new_order.price = price;
        new_order.quantity = quantity;
        new_order.order_id = this->order_counter++;
        // 输出订单看看
        DEBUGOUT("DEBUGOUT--------------one order info--------" << std::endl);
        DEBUGOUT("order_id: " << new_order.order_id << std::endl);
        DEBUGOUT("timestamp: " << new_order.timestamp << std::endl);
        DEBUGOUT("buy_or_sell: " << (new_order.is_buy ? "BUY" : "SELL") << std::endl);
        DEBUGOUT("trader_id: " << new_order.trader_id << std::endl);
        DEBUGOUT("stock_id: " << new_order.stock_id << std::endl);
        DEBUGOUT("price: " << new_order.price << std::endl);
        DEBUGOUT("quantity: " << new_order.quantity << std::endl);
    
        stocks[stock_id].stock_id = stock_id;
        stocks[stock_id].process_order(new_order, traders, args, this->trades_completed);
    }
    // --- 循环结束后 ---
    // 为最后一个交易时间点打印一次中位数报告
    if (args.median) {
        for (size_t i = 0; i < stocks.size(); i++) {
            stocks[i].print_median_report(this->current_timestamp);
        }
    }
}   

void Market::print_final_reports() {
    // 打印最终报告，包括股票价格、交易记录等
    DEBUGOUT("DEBUGOUT--------------final report---------------" << std::endl);
    print_summary_output();
    if (args.trader_info) {
        print_trader_info_output();
    }
    if (args.time_traveler) {
        print_time_traveler_output();
    }
}


/*
---End of Day---
Trades Completed: 5
*/
void Market::print_summary_output() {
    // 打印摘要报告
    DEBUGOUT("DEBUGOUT--------------summary report---------------" << std::endl);
    std::cout << "---End of Day---" << std::endl;
    std::cout << "Trades Completed: " << this->trades_completed << std::endl;
}

/*
---Trader Info---
Trader 0 bought 0 and sold 44 for a net transfer of $1571
Trader 1 bought 43 and sold 11 for a net transfer of $-1866
Trader 2 bought 50 and sold 38 for a net transfer of $295
*/
void Market::print_trader_info_output() {
    // 打印交易员信息报告
    DEBUGOUT("DEBUGOUT--------------trader info report---------------" << std::endl);
    std::cout << "---Trader Info---" << std::endl;
    for (size_t i = 0; i < traders.size(); i++) {
        traders[i].print_report((int)i);
    }
}

/*
---Time Travelers---
A time traveler would buy Stock 0 at time 1 for $56 and sell it at time 2 for $73
A time traveler could not make a profit on Stock 1
*/
void Market::print_time_traveler_output() {
    // 打印时间旅行报告
    DEBUGOUT("DEBUGOUT--------------time traveler report---------------" << std::endl);
    std::cout << "---Time Travelers---" << std::endl;
    for (const auto &stock : stocks) {
        stock.print_time_traveler_report();
    }
}

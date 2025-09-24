// Market.cpp
#include "Market.h"
#include "P2random.h"
#include "debug.h"
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
    DEBUGOUT("mode: " << mode << std::endl);
    DEBUGOUT("num_traders: " << num_traders << std::endl);
    DEBUGOUT("num_stocks: " << num_stocks << std::endl);
    
    // init traders and stocks
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
        DEBUGOUT("seed: " << seed << std::endl);
        DEBUGOUT("num_orders: " << num_orders << std::endl);
        DEBUGOUT("arrival_rate: " << arrival_rate << std::endl);
        
        // Initialize the PROG and populate ss with orders
        P2random::PR_init(ss, seed, num_traders, num_stocks, num_orders, arrival_rate);
    }  // if ..mode

    // Call the function with either the stringstream produced by PR_init()
    // or cin
    if (mode == "PR")
        this->process_orders(ss);  // This is a separate function you must write
    else if (mode == "TL")
        this->process_orders(std::cin); 
    else {
        std::cerr << "Error: Invalid mode. Please use 'PR' or 'TL'." << std::endl;
        exit(1);
    }
}

// 到这行为止看看对不对！

void Market::process_orders(std::istream &inputStream) {
    // 处理订单，包括TL模式和PR模式
    inputStream >> current_timestamp;
    
}   

void Market::print_final_reports() {
    // 打印最终报告，包括股票价格、交易记录等
}

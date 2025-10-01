// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#include "Trader.h"
#include <iostream>


void Trader::record_buy(const int quantity, const int price) {
    buy_quantity += quantity;
    net_transfer -= quantity * price;
}

void Trader::record_sell(const int quantity, const int price) {
    sell_quantity += quantity;
    net_transfer += quantity * price;
}

void Trader::print_report(const int trader_id) const {
    std::cout << "Trader " << trader_id << " bought " 
    << buy_quantity << " and sold " << sell_quantity 
    << " for a net transfer of $" << net_transfer << std::endl;
}

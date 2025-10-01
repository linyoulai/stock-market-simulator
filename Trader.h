// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#ifndef TRADER_H
#define TRADER_H

class Trader {
public:
    void record_buy(const int quantity, const int price);
    void record_sell(const int quantity, const int price);
    void print_report(const int trader_id) const;

private:
    long long buy_quantity = 0;
    long long sell_quantity = 0;
    long long net_transfer = 0;
};

#endif // TRADER_H
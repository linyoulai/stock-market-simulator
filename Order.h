// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#ifndef ORDER_H
#define ORDER_H

#include <string>

// 订单结构体
struct Order {
    int timestamp;
    bool is_buy; // "BUY" or "SELL"
    int trader_id;
    int stock_id;
    int price;
    int quantity;
    int order_id; // 唯一的ID，用于时间平局

    Order(int timestamp, bool is_buy, int trader_id, int stock_id, int price, int quantity, int order_id) :
        timestamp(timestamp), is_buy(is_buy), trader_id(trader_id), stock_id(stock_id), price(price), quantity(quantity), order_id(order_id) {}
};

// 买家优先队列的比较器 (价格高的优先)
struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price < b.price; // 价格低的优先级低
        }
        // 价格相同时，订单ID小的（来得早的）优先级高
        return a.order_id > b.order_id;
    }
};

// 卖家优先队列的比较器 (价格低的优先)
struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price > b.price; // 价格高的优先级低
        }
        // 价格相同时，订单ID小的（来得早的）优先级高
        return a.order_id > b.order_id;
    }
};


#endif // ORDER_H
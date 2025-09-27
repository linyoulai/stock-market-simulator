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
    long long order_id; // 唯一的ID，用于时间平局
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
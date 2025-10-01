// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6

#pragma once

#include <cinttypes>
#include <sstream>
#include <string>
#include <vector>

class P2random {
public:
    static inline void PR_init(std::stringstream &ss, unsigned int seed,
        unsigned int num_traders, unsigned int num_stocks,
        unsigned int num_orders,
        unsigned int arrival_rate) {
        static Prng rng(seed);
        const unsigned int max_price = 100;
        const unsigned int max_quantity = 50;

        long double timestamp = 0;
        for (unsigned int i = 0; i < num_orders; ++i) {
            unsigned int time_increase = rng() % arrival_rate + 1;
            timestamp += 1.0l / time_increase;
            const char *buy_or_sell = rng() % 2 == 0 ? "BUY" : "SELL";
            unsigned int trader_num = rng() % num_traders;
            unsigned int stock_num = rng() % num_stocks;
            unsigned int price = rng() % max_price + 1;
            unsigned int quantity = rng() % max_quantity + 1;

            ss << int(timestamp) << " " << buy_or_sell << " T" << trader_num << " S"
                << stock_num << " $" << price << " #" << quantity << "\n";
        } // for
    } // PR_init()

private:
    // Adapted from http://www.pcg-random.org
    struct Prng {
        using result_type = uint32_t;
        explicit Prng() noexcept = default;
        explicit Prng(uint64_t seq) noexcept { reseed(seq); }
        explicit Prng(uint64_t state, uint64_t seq) noexcept { reseed(state, seq); }
        auto reseed(uint64_t seq) noexcept -> void { reseed(init_state, seq); }

        auto reseed(uint64_t state, uint64_t seq) noexcept -> void {
            state_ = 0U;
            inc_ = (seq << 1u) | 1u;
            operator()();
            state_ += state;
            operator()();
        } // reseed()

        auto operator()() noexcept -> result_type {
            auto const oldstate = state_;
            state_ = oldstate * 6364136223846793005ULL + (inc_ | 1);
            auto const xorshifted =
                static_cast<result_type>(((oldstate >> 18u) ^ oldstate) >> 27u);
            auto const rot = static_cast<result_type>(oldstate >> 59u);
            return (xorshifted >> rot) | (xorshifted << ((~rot + 1) & 31));
        } // operator()()

    private:
        constexpr static auto init_state = 0x853c49e6748fea9bULL;
        constexpr static auto init_seq = 0xda3e39cb94b95bdbULL;

        uint64_t state_ = init_state;
        uint64_t inc_ = init_seq;
    };
}; // P2random

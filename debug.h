// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
// debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

// 这是一个智能的调试输出宏
// 它的工作原理是：
// 1. Makefile 在 "make debug" 时会定义一个叫 DEBUG 的宏 (-DDEBUG)
// 2. #ifdef DEBUG 检查这个宏是否存在
// 3. 如果存在 (debug模式)，DEBUGOUT(x) 就被替换为 std::cerr << x
// 4. 如果不存在 (release模式)，DEBUGOUT(x) 就被替换为 "无操作"

#ifdef DEBUG
    // 在Debug模式下，DEBUGOUT(x) 会变成一个真正的输出语句
    #define DEBUGOUT(x) (std::cerr << x)
#else
    // 在Release模式下，DEBUGOUT(x) 会变成一个什么都不做的空语句
    #define DEBUGOUT(x)
#endif

#endif // DEBUG_H
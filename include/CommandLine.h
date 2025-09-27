#ifndef COMMANDLINE_H
#define COMMANDLINE_H

// 存储解析后的设置
struct CommandLineArgs {
    bool verbose = false;
    bool median = false;
    bool trader_info = false;
    bool time_traveler = false;
};

// 解析命令行的函数
void parse_command_line(int argc, char** argv, CommandLineArgs& args);

#endif // COMMANDLINE_H
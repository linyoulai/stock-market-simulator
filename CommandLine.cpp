// CommandLine.cpp
// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A2519B6
#include "./include/CommandLine.h"
#include <iostream>
#include <getopt.h>

void print_help() {
    std::cout << "Usage: market [OPTIONS]\n"
              << "A stock market simulation program.\n\n"
              << "Options:\n"
              << "  -v, --verbose         Print verbose output for each trade.\n"
              << "  -m, --median          Print median match price at each timestamp.\n"
              << "  -i, --trader_info     Print trader summary at the end.\n"
              << "  -t, --time_travelers  Print time traveler summary at the end.\n"
              << "  -h, --help            Display this help message and exit.\n";
}

void parse_command_line(int argc, char** argv, CommandLineArgs& args) {
    opterr = false;

    int choice;
    int option_index = 0;
    option long_options[] = {
        { "verbose",        no_argument, nullptr, 'v' },
        { "median",         no_argument, nullptr, 'm' },
        { "trader_info",    no_argument, nullptr, 'i' },
        { "time_travelers", no_argument, nullptr, 't' },
        { "help",           no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };
    const char* const short_options = "vmith";

    while ((choice = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (choice) {
            case 'v':
                args.verbose = true;
                break;
            case 'm':
                args.median = true;
                break;
            case 'i':
                args.trader_info = true;
                break;
            case 't':
                args.time_traveler = true;
                break;
            case 'h':
                print_help();
                exit(0);
            case '?':
                std::cerr << "Error: Unknown command line option" << std::endl;
                exit(1);
        }
    }
}
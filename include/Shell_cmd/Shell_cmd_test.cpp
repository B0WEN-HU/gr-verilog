#include <cstdio>
#include <string>
#include <iostream>

#include "Shell_cmd.h"

int main()
{
    gr::verilog::Shell_cmd bash;
    std::string cmd = "echo $(env)";
    //std::cin >> cmd;
    std::cout << bash.exec(cmd.c_str()) << std::endl;
    bash.print_msg(std::cerr);
    return 0;
}
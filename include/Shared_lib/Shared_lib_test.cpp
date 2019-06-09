#include <cstdio>
#include <string>
#include <iostream>

#include "Shared_lib.h"

#define SLASH '/'


int main()
{
    gr::verilog::Shared_lib libfoo;
    std::string lib_fullname;
    std::cin >> lib_fullname;
    std::size_t filename_pos = lib_fullname.rfind(SLASH);
    std::string lib_name = lib_fullname.substr(filename_pos + 1);
    std::string lib_path = lib_fullname.substr(0, filename_pos + 1);
    std::cout << lib_path << std::endl << lib_name << std::endl;
    
    std::cout << "LOAD_LIB:" << libfoo.load_lib(lib_path.c_str(), lib_name.c_str()) << std::endl;
    std::cout << "LIB_PATH:" << libfoo.get_lib_path() << std::endl;
    std::cout << "LIB_NAME:" << libfoo.get_lib_name() << std::endl;

    std::string func_name;
    gr::verilog::func_ptr f;
    std::cin >> func_name;
    std::cout << "FIND_FUNC:" << (f = libfoo.find_func(func_name.c_str())) << std::endl;

    unsigned int x;
    unsigned int y;
    std::cout << "INPUT x:";
    std::cin >> x;
    f(x, y);
    std::cout << "f(x) = " << y << std::endl;

    libfoo.close_lib();

    return 0;
}
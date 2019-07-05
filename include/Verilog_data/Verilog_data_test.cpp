#include <cstdio>
#include <string>
#include <iostream>

#include "Verilog_data.h"


struct Void_module{
      // This is just a placeholder for the template
      // Void_module should be used in main code of the block
      // The real template <Vmodule> should be used in the shared library
    };


int main ()
{
	gr::verilog::Verilog_data<Void_module> Vmodule;
	Vmodule.add_port("test_port_A", gr::verilog::INPUT_PORT, 32);
  Vmodule.add_port("test_port_B", gr::verilog::INPUT_PORT, 16);

  std::cout << Vmodule.print_port("test_port_A") << std::endl;
  std::cout << Vmodule.print_port("test_port_B") << std::endl;
  std::cout << Vmodule.print_port("test_port_C") << std::endl;

	
	return 0;
}

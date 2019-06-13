#include <cstdio>
#include <string>

#include "Verilog_data.h"

struct Void_module{
      // This is just a placeholder for the template
      // Void_module should be used in main code of the block
      // The real template <Vmodule> should be used in the shared library
    };


int main ()
{
	gr::verilog::Verilog_data<Void_module> Vmodule;
	Vmodule.add_port("test_port", gr::verilog::INPUT_PORT, 32);
	
	return 0;
}

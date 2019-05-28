/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_VERILOG_VERILOG_II_IMPL_H
#define INCLUDED_VERILOG_VERILOG_II_IMPL_H

#include <verilog/verilog_ii.h>
#include <string>

namespace gr {
  namespace verilog {

    class verilog_ii_impl : public verilog_ii
    {
     private:
      /* verilog related private members  */
      
      // The path and name of user's verilog module
      // Construct by (const char *filename)
      std::string verilog_module_path;
      std::string verilog_module_name;
      
      // The path of makefile template
      std::string makefile_template_path;

      // The path of cpp template 
      std::string cpp_template_path;

      // The class that call the shell command make
      // Use sh_make.cmd(std::string cmd) to call bash
      Shell_cmd sh_make;

      // The class that store the verilog module data
      // Including port map and input/output data
      // Use verilog_data.set_input( (ITYPE) in[i] ) to set input data
      // Usr verilog_data.set_output( (OTYPE) out[i] ) to set output data
      Verilog_data verilog_data;

      // The class that control the shared library
      // Use verilog_module_so.load_lib(std::string verilog_module_path, std::string lib_name) to load library
      // Use verilog_module_so.find_func(std::string func_name) to get the function
      // Use verilog_module_so.release_lib(std::string string lib_name) to release the library
      Shared_lib verilog_module_so;

      // typedef void (*Simulation_func)(const verilog_data.get_input_addr(), verilog_data.get_output_addr());
      Simulation_func sim;
      
      /* verilog related private members  */

     public:
      verilog_ii_impl(const char *filename);
      ~verilog_ii_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace verilog
} // namespace gr

#endif /* INCLUDED_VERILOG_VERILOG_II_IMPL_H */


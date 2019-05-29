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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "verilog_ii_impl.h"

namespace gr {
  namespace verilog {

    verilog_ii::sptr
    verilog_ii::make(const char *filename)
    {
      return gnuradio::get_initial_sptr
        (new verilog_ii_impl(filename));
    }

    /*
     * The private constructor
     */
    verilog_ii_impl::verilog_ii_impl(const char *filename)
      : gr::sync_block("verilog_ii",
              gr::io_signature::make(1, 1, sizeof(ITYPE)),
              gr::io_signature::make(1, 1, sizeof(OTYPE)))
    {
      /* Get module_name and module_path */
      std::string filename_temp(filename);
      std::size_t filename_pos = filename_temp.rfind(SLASH);
      if (std::string::npos == filename_pos) {
        /* TODO: raise_error() */
      }
      this->verilog_module_name = filename_temp.substr(filename_pos + 1);
      this->verilog_module_path = filename_temp.substr(0, filename_pos + 1);

      /* Initialize makefile_template_path and cpp_template_path */
      // TODO: #define MAKEFILE_TEMPLATE_PATH "$(prefix)/gnuradio/gr-verilog/lib"
      this->makefile_template_path = MAKEFILE_TEMPLATE_PATH;
      // TODO: #define CPP_TEMPLATE_PATH "$(prefix)/gnuradio/gr-verilog/lib"
      this->cpp_template_path = CPP_TEMPLATE_PATH;

      /* Call Verilator (Makefile) to generate the cpp code */
      // There will be a Shell_cmd object created in the function to
      // run configure.sh
      // configure.sh will copy the makefile template and modify it
      // for the verilog module
      // the Shell_cmd will run make at the verilog module path
      try {
        this->generate_verilator_file();
      } catch (...) {
        /* TODO: Handle the error */
      }
      

      /* Initialize port_map that stored in verilator_data */
      // The port map is the structure of input/output ports of
      // verilog module
      // Verilog_data should have a method to parse the code(.cpp/.v) and
      // extract the port structure of verilog module.
      try {
        this->init_port_map();
      } catch (...) {
        /* TODO: Handle the error */
      }
      

      /* Generate shared library and initialize verilog_module_so */
      // There will be a Shell_cmd object created in the function to
      // run make at the verilog module path
      try {
        this->generate_so();
      } catch (...) {
        /* TODO: Handle the error */
      }
      
      /* Load the shared library that generated above */
      // The function should also initialize the external veriable
      // in the shred library
      // Call verilog_module_so.find_func(VERILOG_INIT_FUNC)
      // Call verilog_module_so.find_func(VERILOG_RESET_FUNC)
      // shared library function reset() should be defalt generated
      // There would be a function called general_sim()
      // general_sim() could accept input and output of all port with
      // the help of port map stored in the verilog_data
      try {
        this->load_lib();
      } catch (...) {
        /* TODO: Handle the error */
      }

      /* Initialize sim */
      this->sim = NULL;
    }

    /*
     * Our virtual destructor.
     */
    verilog_ii_impl::~verilog_ii_impl()
    {
    }

    int
    verilog_ii_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE *in = (const ITYPE *) input_items[0];
      OTYPE *out = (OTYPE *) output_items[0];
      if (NULL == this->sim)
      {
        // TODO: #define VERILOG_SIMULATION_FUNC "$(the_name_of_the_function)"
        try {
          // Find the function of simulation in the shared library
          sim = verilog_module_so.find_func(VERILOG_SIMULATION_FUNC);
        } catch (...) {
          /* TODO: Handle the error */
        }
      }

      // Do <+signal processing+>
      for (unsigned int i = 0; i < noutput_items; i++)
      {
        this->verilog_data.set_input(in[i]);
        this->verilog_data.set_output(out[i]);
        this->verilog_data.convert();
        sim(verilog_data.get_input_addr(), verilog_data.get_output_addr());
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace verilog */
} /* namespace gr */


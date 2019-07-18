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
#include "verilog_axi_ii_impl.h"

#include "verilog/constants.h"

#include "Shell_cmd.h"
#include "Shared_lib.h"

#define AXI_MODULE_CL_MAKEFILE "axi_module_cl.mk"
#define CPP_TEMPLATE_NAME "axi_module.cpp"
#define SHARED_LIB_NAME "lib_axi_module.so"
#define M_dir "obj_dir"

#define GNU_RADIO_PREFIX "/home/bowen/Documents/GSoC/gr-verilog/"
#define MAKEFILE_TEMPLATE_PATH GNU_RADIO_PREFIX "lib/"
#define CPP_TEMPLATE_PATH GNU_RADIO_PREFIX "lib/"

#define _EXIT_SUCCESS 0
#define _EXIT_FAILURE -1

namespace gr {
  namespace verilog {

    verilog_axi_ii::sptr
    verilog_axi_ii::make(const char *filename)
    {
      return gnuradio::get_initial_sptr
        (new verilog_axi_ii_impl(filename));
    }

    /*
     * The private constructor
     */
    verilog_axi_ii_impl::verilog_axi_ii_impl(const char *filename)
      : gr::sync_block("verilog_axi_ii",
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

      // Reset the initial time
      this->main_time = 0;

      
      /* Call Verilator (Makefile) to generate the cpp code */
      // There will be a Shell_cmd object created in the function to
      // run configure.sh
      // configure.sh will copy the makefile template and modify it
      // for the verilog module
      // the Shell_cmd will run make at the verilog module path
      
      //try {
      //  this->generate_verilator_file();
      //} catch (...) {
      //  /* TODO: Handle the error */
      //}

      /* Generate shared library and initialize verilog_module_so */
      // There will be a Shell_cmd object created in the function to
      // run make at the verilog module path
      // ! generate_verilator_file will be included in the this->generate_so()
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
    verilog_axi_ii_impl::~verilog_axi_ii_impl()
    {
      /* Release the shared library */
      try {
        this->release_lib();
      } catch (...) {
        /* TODO: Handle the error */
      }
    }

    int
    verilog_axi_ii_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE *in = (const ITYPE *) input_items[0];
      OTYPE *out = (OTYPE *) output_items[0];

      // Initial and Reset the module
      if (NULL == this->sim)
      {
        typedef void (*Initial_func) (void);
        typedef void (*Reset_func) (void);
        
        Initial_func axi_init;
        Reset_func axi_reset;

        axi_init  =
            (Initial_func)this->verilog_module_so.find_func("AXI_init");
        axi_reset =
            (Reset_func)this->verilog_module_so.find_func("AXI_reset");

        if ((NULL == axi_init) or (NULL == axi_reset)) {
          return _EXIT_FAILURE;
        }

        axi_init();
        axi_reset();

        this->sim =
            (Simulation_func)this->verilog_module_so.find_func("AXI_transfer");
      }

      // Do <+signal processing+>
      for (unsigned int i = 0; i < noutput_items; i++)
      {
        this->sim(in[i], out[i],this->main_time);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    /* gr::verilog::verilog_axi_ii private member functions */

    int
    verilog_axi_ii_impl::generate_so() throw(std::runtime_error)
    {
      // const char
      const std::string ENTER = "\n";
      const std::string BACKSLASH = "\\";

      // compile and link error code
      int cl_err_code = 0;

      // Shell_cmd class
      Shell_cmd bash;
      std::string cmd = "";
      
      // $ cd ${verilog_module_path}
      cmd += "cd ";
      cmd += this->verilog_module_path;
      cmd += ENTER;

      // $ cp ${makefile_template_path}/axi_module_cl.mk ${verilog_module_path}
      // #define AXI_MODULE_CL_MAKEFILE "axi_module_cl.mk"
      cmd += "cp ";
      cmd += this->makefile_template_path + AXI_MODULE_CL_MAKEFILE;
      cmd += " " + this->verilog_module_path;
      cmd += ENTER;

      // $ make -j -f axi_module_cl.mk \
      //   USER_VL_FILENAME=user_module_name.v \
      //   USER_CPP_FILENAME=axi_module.cpp \
      //   M_DIR=obj_dir
      cmd += std::string("") + "make -j -f " + AXI_MODULE_CL_MAKEFILE;
      cmd += std::string(" ") + "USER_VL_FILENAME=" + this->verilog_module_name;
      cmd += std::string(" ") + "USER_CPP_FILENAME=" + CPP_TEMPLATE_NAME;
      cmd += std::string(" ") + " M_DIR=" + M_dir;
      cmd += ENTER;

      cmd += ENTER;
      bash.exec(cmd.c_str());
      // TODO: FIX: check and handle the error
      // if (error) { cl_err_code = -1 }

      return cl_err_code;
    }

    int
    verilog_axi_ii_impl::load_lib() throw(std::runtime_error)
    {
      int lib_err_code;
      lib_err_code = 
        this->verilog_module_so.load_lib((this->verilog_module_path + M_dir).c_str(),
                                         SHARED_LIB_NAME);
      if (-1 == lib_err_code) {
        // TODO: throw(std::runtime_error);
      }

      return lib_err_code;
    }

    int
    verilog_axi_ii_impl::release_lib() throw(std::runtime_error)
    {
      this->verilog_module_so.close_lib();
    }

    /* gr::verilog::verilog_axi_ii private member functions */

  } /* namespace verilog */
} /* namespace gr */


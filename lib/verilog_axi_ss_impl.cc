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
#include "verilog_axi_ss_impl.h"
#include <gnuradio/thread/thread.h>
#include <unistd.h>
#include <stdexcept>
#include <cmath>


#include "verilog/constants.h"

#include "verilog/Shell_cmd.h"
#include "verilog/Shared_lib.h"

#define AXI_MODULE_CL_MAKEFILE "axi_module_cl.mk"
#define CPP_TEMPLATE_NAME "axi_module.cpp"
#define HEADER_TEMPLATE_NAME "axi_module.h"
#define SHARED_LIB_NAME "lib_axi_module.so"
#define M_dir "obj_dir"

#define MAKEFILE_TEMPLATE_PATH templatedir()
#define CPP_TEMPLATE_PATH templatedir()

#define _EXIT_SUCCESS 0
#define _EXIT_FAILURE -1

namespace gr {
  namespace verilog {

    verilog_axi_ss::sptr
    verilog_axi_ss::make(const char *filename, bool overwrite, float IO_ratio,
                         const char *verilator_options, unsigned int module_flag, unsigned int skip_output_items)
    {
      return gnuradio::get_initial_sptr
        (new verilog_axi_ss_impl(filename, overwrite, IO_ratio,
                                 verilator_options, module_flag, skip_output_items));
    }

    /*
     * The private constructor
     */
    verilog_axi_ss_impl::verilog_axi_ss_impl(const char *filename, bool overwrite, float IO_ratio,
                                             const char *verilator_options, unsigned int module_flag,
                                             unsigned int skip_output_items)
      : gr::block("verilog_axi_ss",
              gr::io_signature::make(1, 1, sizeof(ITYPE)),
              gr::io_signature::make(1, 1, sizeof(OTYPE)))
    {
      /* Get module_name and module_path */
      std::string filename_temp(filename);
      std::size_t filename_pos = filename_temp.rfind(SLASH);
      if (std::string::npos == filename_pos) {
        GR_LOG_WARN(d_logger, "filename error");
      }

      this->verilog_module_name = filename_temp.substr(filename_pos + 1);
      this->verilog_module_path = filename_temp.substr(0, filename_pos + 1);

      // Test access
      this->test_access(filename,
                        (std::string("\ncan't access verilog file in: ") +
                         this->verilog_module_path).c_str());

      /* Initialize makefile_template_path and cpp_template_path */
      this->makefile_template_path = MAKEFILE_TEMPLATE_PATH;
      this->cpp_template_path = CPP_TEMPLATE_PATH;
      // Test access
      this->test_access((this->makefile_template_path + AXI_MODULE_CL_MAKEFILE).c_str(),
                        (std::string("\ncan't access makefile template in: ") +
                         this->makefile_template_path).c_str());
      this->test_access((this->cpp_template_path + CPP_TEMPLATE_NAME).c_str(),
                        (std::string("\ncan't access cpp template in: ") +
                         this->cpp_template_path).c_str());
      this->test_access((this->cpp_template_path + HEADER_TEMPLATE_NAME).c_str(),
                        (std::string("\ncan't access header template in: ") +
                         this->cpp_template_path).c_str());

      // Reset the initial time
      this->main_time = 0;

      // Initial skip_output_items
      this->skip_output_items = skip_output_items;

      // Set overwrite
      this->overwrite = overwrite;

      // Set IO_ratio
      this->IO_ratio = IO_ratio;

      // Set verilator options
      this->verilator_options = std::string(verilator_options);

      // Set module_flag
      this->module_flag = module_flag;

      /* Call Verilator (Makefile) to generate the cpp code */
      // There will be a Shell_cmd object created in the function to
      // run configure.sh
      // configure.sh will copy the makefile template and modify it
      // for the verilog module
      // the Shell_cmd will run make at the verilog module path

      // Check enviroments : make, g++, verilator
      this->check_env("make", "can't find make");
      this->check_env("g++", "can't find g++");
      this->check_env("verilator", "can't find verilator");

      //try {
      //  this->generate_verilator_file();
      //} catch (...) {
      //
      //}

      /* Generate shared library and initialize verilog_module_so */
      // There will be a Shell_cmd object created in the function to
      // run make at the verilog module path
      // ! generate_verilator_file will be included in the this->generate_so()
      try {
        // obtain exclusive access for duration of generate_so()
        gr::thread::scoped_lock lock(this->vl_mutex);

        this->generate_so();
      } catch (std::runtime_error) {
        GR_LOG_ERROR(d_logger,
                     boost::format("%s: %s")
                     % this->verilog_module_path.c_str()
                     % strerror(errno));
        throw;
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
        // obtain exclusive access for duration of load_lib()
        gr::thread::scoped_lock lock(this->vl_mutex);

        this->load_lib();
      } catch (std::runtime_error) {
        GR_LOG_ERROR(d_logger,
                     boost::format("%s: %s")
                     % this->verilog_module_path.c_str()
                     % strerror(errno));
        throw;
      }

      /* Initialize sim */
      this->sim = NULL;
    }

    /*
     * Our virtual destructor.
     */
    verilog_axi_ss_impl::~verilog_axi_ss_impl()
    {
      // There should not be any errors in destructor
      typedef void (*Close_func) (void);
      Close_func axi_close;
      axi_close =
            (Close_func)this->verilog_module_so.find_func("AXI_close");
      if (axi_close != NULL)
        axi_close();

      this->release_lib();
    }

    void
    verilog_axi_ss_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = (int) round(IO_ratio * noutput_items);
    }

    int
    verilog_axi_ss_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const ITYPE *in = (const ITYPE *) input_items[0];
      OTYPE *out = (OTYPE *) output_items[0];

      // Initial and Reset the module
      if (NULL == this->sim)
      {
        typedef void (*Initial_func) (void);
        typedef void (*Reset_func) (unsigned int flag);

        Initial_func axi_init;
        Reset_func axi_reset;

        axi_init  =
            (Initial_func)this->verilog_module_so.find_func("AXI_init");
        axi_reset =
            (Reset_func)this->verilog_module_so.find_func("AXI_reset");

        if ((NULL == axi_init) or (NULL == axi_reset)) {
          throw std::runtime_error("can't find correct AXI_init or AXI_reset in shared library");
          return _EXIT_FAILURE;
        }

        axi_init();
        axi_reset(this->module_flag);

        this->sim =
            (Simulation_func)this->verilog_module_so.find_func("AXI_async_transfer_ss");

        if (NULL == this->sim) {
          throw std::runtime_error("can't find correct AXI_async_transfer_ss in shared library");
          return _EXIT_FAILURE;
        }
      }


      // Do <+signal processing+>
      unsigned int input_i;
      unsigned int output_i;
      for (input_i = 0, output_i = 0; output_i < noutput_items && input_i < ninput_items[0];)
      {
        unsigned char status_code;

        try {
          status_code =
              this->sim(in[input_i], out[output_i], this->main_time);
        } catch (std::runtime_error) {
          GR_LOG_ERROR(d_logger,
                     boost::format("%s: %s")
                     % this->verilog_module_path.c_str()
                     % strerror(errno));
          throw;
        }

        // input
        if (status_code & (1 << 1)) {
          ++input_i;
        }
        // output
        if (status_code & 1) {
          if (this->skip_output_items > 0)
            --this->skip_output_items;
          else
            ++output_i;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (input_i);

      // Tell runtime system how many output items we produced.
      return output_i;
    }


    /* gr::verilog::verilog_axi_ss private member functions */

    int
    verilog_axi_ss_impl::generate_so()
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
      if (!this->overwrite)
        cmd += "-n";
      cmd += std::string(" ") + this->makefile_template_path + AXI_MODULE_CL_MAKEFILE;
      cmd += std::string(" ") + this->makefile_template_path + CPP_TEMPLATE_NAME;
      cmd += std::string(" ") + this->makefile_template_path + HEADER_TEMPLATE_NAME;
      cmd += std::string(" ") + this->verilog_module_path;
      cmd += ENTER;
      /* alternative solution:
      if (this->overwrite) {
        cmd += "cp";
        cmd += std::string(" ") + this->makefile_template_path + AXI_MODULE_CL_MAKEFILE;
        cmd += std::string(" ") + this->cpp_template_path + CPP_TEMPLATE_NAME;
        cmd += std::string(" ") + this->cpp_template_path + HEADER_TEMPLATE_NAME;
        cmd += std::string(" ") + this->verilog_module_path;
        cmd += ENTER;
      }
      else {
        std::string CL_makefile_user_filename =
            (this->verilog_module_path + AXI_MODULE_CL_MAKEFILE);
        std::string CPP_user_filename =
            (this->verilog_module_path + CPP_TEMPLATE_NAME).c_str();

        bool CL_makefile_flag = this->test_access(CL_makefile_user_filename.c_str(), "");
        bool CPP_flag = this->test_access(CPP_user_filename.c_str(), "");

        if (!CL_makefile_flag || !CPP_flag) {
          cmd += "cp ";

          if (!CL_makefile_flag)
            cmd += std::string(" ") + this->makefile_template_path + AXI_MODULE_CL_MAKEFILE;

          if (!CPP_flag) {
            cmd += std::string(" ") + this->cpp_template_path + CPP_TEMPLATE_NAME;
            cmd += std::string(" ") + this->cpp_template_path + HEADER_TEMPLATE_NAME;
          }
          cmd += std::string(" ") + this->verilog_module_path;
          cmd += ENTER;
        }
      } */


      // $ make -j -f axi_module_cl.mk \
      //   USER_VL_FILENAME=user_module_name.v \
      //   USER_CPP_FILENAME=axi_module.cpp \
      //   M_DIR=obj_dir
      cmd += std::string("") + "make -j -f " + AXI_MODULE_CL_MAKEFILE;
      cmd += std::string(" ") + "USER_VL_FILENAME=" + this->verilog_module_name;
      cmd += std::string(" ") + "USER_CPP_FILENAME=" + CPP_TEMPLATE_NAME;
      cmd += std::string(" ") + " M_DIR=" + M_dir;
      // cmd += verilator_options:
      cmd += std::string(" ") + "VERILATOR_OPTIONS=" + this->verilator_options;
      cmd += ENTER;

      cmd += ENTER;

      try {
        cl_err_code = bash.exec(cmd.c_str());
        if (cl_err_code == _EXIT_FAILURE) {
          throw std::runtime_error("Shell_cmd execute error");
        }

        // Output the message
        // bash.print_msg(std::cout);
      }
      catch (...) {
        bash.print_msg(std::cerr);
        throw;
      }

      return cl_err_code;
    }

    int
    verilog_axi_ss_impl::load_lib()
    {
      int lib_err_code;
      lib_err_code =
        this->verilog_module_so.load_lib((this->verilog_module_path + M_dir).c_str(),
                                         SHARED_LIB_NAME);
      if (lib_err_code == _EXIT_FAILURE) {
        throw std::runtime_error("can't load shared library");
      }

      return lib_err_code;
    }

    int
    verilog_axi_ss_impl::release_lib()
    {
      this->verilog_module_so.close_lib();
    }

    bool
    verilog_axi_ss_impl::test_access(const char *filepath, const char *err_msg = "")
    {

      if ( access(filepath, R_OK) == _EXIT_FAILURE ) {
        if (err_msg != "") {
          GR_LOG_ERROR(d_logger,
                       boost::format("%s: %s")
                       % filepath
                       % strerror(errno));
          throw std::runtime_error(err_msg);
        }

        return false;
      }

      return true;
    }

    bool
    verilog_axi_ss_impl::check_env(const char *package, const char *err_msg = "")
    {

      Shell_cmd bash;
      bash.exec((std::string("which ") + package).c_str());
      if (bash.get_msg(0) == "") {
        if (err_msg != "") {
          GR_LOG_ERROR(d_logger,
                       boost::format("%s: %s")
                       % package
                       % strerror(errno));
          throw std::runtime_error(err_msg);
        }

        return false;
      }

      return true;
    }

    /* gr::verilog::verilog_axi_ss private member functions */

  } /* namespace verilog */
} /* namespace gr */


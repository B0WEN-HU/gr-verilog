
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

#ifndef INCLUDED_VERILOG_SHARED_LIB_H
#define INCLUDED_VERILOG_SHARED_LIB_H

#include <cstdio>
#include <string>

namespace gr {
  namespace verilog {

    typedef void (*func_ptr)(void);

#ifdef __linux
    class Shared_lib
    {
     private:
      // Private members

      // The path and name of the library
      std::string lib_name;
      std::string lib_path;

      // The hanle of the shared library
      void *lib_handle;

      // The status of the shared library
      enum {LIB_NULL, LIB_LOAD_FAILURE, LIB_LOAD_SUCCESS} lib_status;

     public:
      // Public members

      Shared_lib();
      ~Shared_lib();

      // The function that load the shared library
      // It will return -1 when there is any error
      // It will return 0 if the shared library was loaded successfully
      int load_lib(const char *ext_lib_path, const char *ext_lib_name);

      // The function that find the function in the library
      // It will return NULL when it cannot find the function
      // It will return the pointer of the function
      func_ptr find_func(const char *func_name) const;

      // The function that close the library
      void close_lib(void);

      // DEBUG
      std::string get_lib_path() const;
      std::string get_lib_name() const;
      std::string get_lib_status() const;
      void * get_lib_handle() const;

    };
#endif

  } // namespace verilog
} // namespace gr

#endif /* INCLUDED_VERILOG_SHARED_LIB_H */


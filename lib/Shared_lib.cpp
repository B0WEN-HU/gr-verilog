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

#include <cstdio>
#include <dlfcn.h>
#include <cstring>
#include <string>
#include "Shared_lib.h"

#define SLASH '/'
#define _EXIT_FAILURE -1
#define _EXIT_SUCCESS  0

namespace gr {
  namespace verilog {

    // The constructor
    Shared_lib::Shared_lib()
      : lib_handle(NULL), lib_status(LIB_NULL),
        lib_path(""), lib_name("")
    {}

    // The destructor
    Shared_lib::~Shared_lib() {
      if(LIB_LOAD_SUCCESS == this->lib_status)
        this->close_lib();
    }

    // The load_lib function of the class
    int Shared_lib::load_lib(const char *ext_lib_path, const char *ext_lib_name)
    {
      // Get the the fullname of the shared library      
      std::string lib_fullname = ext_lib_path;
      // Check the slash at the end of the path
      if (SLASH == lib_fullname.back()) {
        this->lib_path = lib_fullname;
        this->lib_name = ext_lib_name;

        lib_fullname += ext_lib_name;
      }
      else {
        lib_fullname += SLASH;

        this->lib_path = lib_fullname;
        this->lib_name = ext_lib_name;

        lib_fullname += ext_lib_name;
      }
      
      
      // Open the shared library
      this->lib_handle = dlopen(lib_fullname.c_str(), RTLD_LAZY);
      // If the library was not loaded
      if (NULL == this->lib_handle) {
        this->lib_status = LIB_LOAD_FAILURE;
        return _EXIT_FAILURE;
      }

      // Otherwise the library was loaded successfully
      this->lib_status = LIB_LOAD_SUCCESS;
      return _EXIT_SUCCESS;
    }

    // The find_func function of the class
    func_ptr Shared_lib::find_func(const char *func_name) const
    {
      func_ptr func_tmp;
      func_tmp = (func_ptr)dlsym(this->lib_handle, func_name);
      // If the function was not found
      if (NULL == func_tmp) {
        // TODO: throw error
        return NULL;
      }
      // Otherwise
      return func_tmp;
    }

    // The close_lib function of the class
    void Shared_lib::close_lib(void)
    {
      dlclose(this->lib_handle);

      this->lib_path = "";
      this->lib_name = "";
      this->lib_handle = NULL;
      this->lib_status = LIB_NULL;
      return ;
    }

    // DEBUG
    std::string Shared_lib::get_lib_path() const
    {
      return this->lib_path;
    }
    std::string Shared_lib::get_lib_name() const
    {
      return this->lib_name;
    }
    std::string Shared_lib::get_lib_status() const
    {
      switch (this->lib_status) {
        case LIB_NULL:
          return std::string("LIB_NULL");
        case LIB_LOAD_FAILURE:
          return std::string("LIB_LOAD_FAILURE");
        case LIB_LOAD_SUCCESS:
          return std::string("LIB_LOAD_SUCCESS");
        default:
          return std::string("ERROR");
      }
      return std::string("ERROR");
    }
    void * Shared_lib::get_lib_handle() const
    {
      return this->lib_handle;
    }

  } /* namespace verilog */
} /* namespace gr */


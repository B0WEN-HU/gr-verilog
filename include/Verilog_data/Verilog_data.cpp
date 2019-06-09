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
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include "Verilog_data.h"


namespace gr {
  namespace verilog {

    // The constructor
    Verilog_data::Verilog_data() {
      this->port_map.clear();
      this->port_list.clear();
    }

    // The destructor
    Verilog_data::~Verilog_data() {
      this->port_map.clear();
      this->port_list.clear();
    }

    // The parse_port function of the class
    // The function that read the parse the source file and get the port_map and port_list
    // TODO:
    int Verilog_data::parse_port(const char *source_file_path, const char *source_file_type)
    {}

    // the set_input_port function of the class
    // The function that set the input port of the VLmodule
    // It should be used in the shared library rather in the gr-verilog block
    int set_input_port(VLmodule &module, const char *port_name, const ITYPE &value) const
    {
      std::map<std::string, vector<Port_info>::iterator>::iterator port_map_iter;
      port_map_iter = this->port_map.find(std::string(port_name));
      if (port_map_iter != this->port_map.end()) {
        try {
          (port_map_iter->second)->set_input_port(module, value);
        } catch (...) {
          // TODO: Handle the error
        }
        return _EXIT_SUCCESS;
      }
      else {
        return _EXIT_FAILURE;
      }
    }

    int Verilog_data::get_output_port(VLmodule &moduel, const char *port_name, OTYPE &value)
    {
      std::map<std::string, vector<Port_info>::iterator>::iterator port_map_iter;
      port_map_iter = this->port_map.find(std::string(port_name));
      if (port_map_iter != this->port_map.end()) {
        try {
          value = (port_map_iter->second)->get_output_port(module);
        } catch (...) {
          // TODO: Handle the error
        }
        return _EXIT_SUCCESS;
      }
      else {
        return _EXIT_FAILURE;
      }
    }

    
  } /* namespace verilog */
} /* namespace gr */


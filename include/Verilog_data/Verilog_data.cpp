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
    template <class VLmodule>
    Verilog_data<VLmodule>::Verilog_data() {
      this->port_map.clear();
      this->port_list.clear();
    }

    // The destructor
    template <class VLmodule>
    Verilog_data<VLmodule>::~Verilog_data() {
      this->port_map.clear();
      this->port_list.clear();
    }

    // The parse_port function of the class
    // The function that read the parse the source file and get the port_map and port_list
    // TODO:
    template <class VLmodule>
    int Verilog_data<VLmodule>::parse_port(const char *source_file_path, const char *source_file_type)
    {}

    // The add_port function of the class
    // main block version
    template <class VLmodule>
    int Verilog_data<VLmodule>::add_port(const char *port_name, PORT_IO_TYPE iotype, unsigned int port_width)
    {
      //Port_info<VLmodule> port_tmp = Port_info<VLmodule>(iotype, port_width);
      this->port_list.push_back(Port_info<VLmodule>(iotype, port_width));
      
      std::pair<std::map<std::string, std::vector<Port_info<VLmodule>>::iterator>::iterator, bool> map_ret;
      map_ret =
      this->port_map.insert( make_pair(std::string(port_name), this->port_list.end() - 1) );
      if (false == map_ret.second) {
        // TODO: throw error
        // There are duplicate port name
        return _EXIT_FAILURE;
      }
      return _EXIT_SUCCESS;
    }

    // The add_port function of the class
    // shared library version
    // .add_port(&Vmodule::portname, "portname", INPUT_PORT/OUTPUT_PORT, sizeof(Vmodule::portname))
    template <class VLmodule>
    template <class PORT_ADDR>
    int Verilog_data<VLmodule>::add_port(const VLmodule &module, const char *port_name, PORT_IO_TYPE iotype, unsigned int port_width)
    {
      this->port_list.push_back(Port_info<VLmodule>(iotype, port_width, port_addr));
      
      std::pair<std::map<std::string, std::vector<Port_info<VLmodule>>::iterator>::iterator, bool> map_ret;
      map_ret =
      this->port_map.insert( make_pair(std::string(port_name), this->port_list.end() - 1) );
      if (false == map_ret.second) {
        // TODO: throw error
        // There are duplicate port name
        return _EXIT_FAILURE;
      }
      return _EXIT_SUCCESS;
    }


    // the set_input_port function of the class
    // The function that set the input port of the VLmodule
    // It should be used in the shared library rather in the gr-verilog block
    template <class VLmodule>
    int Verilog_data<VLmodule>::set_input_port(VLmodule &module, const char *port_name, const ITYPE &value) const
    {
      std::map<std::string, std::vector<Port_info<VLmodule>>::iterator>::iterator port_map_iter;
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

    // the get_output_port function of the class
    template <class VLmodule>
    int Verilog_data<VLmodule>::get_output_port(const VLmodule &moduel, const char *port_name, OTYPE &value)
    {
      std::map<std::string, std::vector<Port_info<VLmodule>>::iterator>::iterator port_map_iter;
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


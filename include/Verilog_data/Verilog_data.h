
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

#ifndef INCLUDED_VERILOG_VERILOG_DATA_H
#define INCLUDED_VERILOG_VERILOG_DATA_H

#include <cstdio>
#include <string>
#include <vector>
#include <map>      // Could be replaced by unordered_map

#include "gr_verilog_iotype.h"

namespace gr {
  namespace verilog {

    typedef bool PORT_IO_TYPE;
    const PORT_IO_TYPE INPUT_PORT = true;
    const PORT_IO_TYPE OUTPUT_PORT = false;

    struct Void_module{
      // This is just a placeholder for the template
      // Void_module should be used in main code of the block
      // The real template <Vmodule> should be used in the shared library
    };

    template <class VLmodule>
    class Port_info{
     public:

      Port_info(PORT_IO_TYPE iotype, unsigned int port_width)
      {
        this->iotype = iotype;
        switch (port_width)
        {
        case 8:
          this->port_width = UINT8;
          break;
        
        case 16:
          this->port_width = UINT16;
          break;

        case 32:
          this->port_width = UINT32;
          break;
          
        case 64:
          this->port_width = UINT64;
          break;
        
        default:
          // TODO: throw error
          break;
        }
      }
      
      Port_info(PORT_IO_TYPE iotype, unsigned int port_width, uint8_t VLmodule::*port_addr)
      {
        this->iotype = iotype;
        this->port_width = UINT8;
        this->port_ptr.uint8_p = port_addr;
      }

      Port_info(PORT_IO_TYPE iotype, unsigned int port_width, uint16_t VLmodule::*port_addr)
      {
        this->iotype = iotype;
        this->port_width = UNIT16;
        this->port_ptr.uint16_p = port_addr;
      }

      Port_info(PORT_IO_TYPE iotype, unsigned int port_width, uint32_t VLmodule::*port_addr)
      {
        this->iotype = iotype;
        this->port_width = UNIT32;
        this->port_ptr.uint32_p = port_addr;
      }

      Port_info(PORT_IO_TYPE iotype, unsigned int port_width, uint64_t VLmodule::*port_addr)
      {
        this->iotype = iotype;
        this->port_width = UNIT64;
        this->port_ptr.uint64_p = port_addr;
      }

      ~Port_info() {}

      // port data
      PORT_IO_TYPE iotype;
      PORT_TYPE value;
      enum {UINT8, UNIT16, UINT32, UINT64} port_width;

      union
      {
        // template <Vmodule>
        void *void_p;
        uint8_t  VLmodule::*uint8_p;
        uint16_t VLmodule::*uint16_p;
        uint32_t VLmodule::*uint32_p;
        uint32_t VLmodule::*uint64_p;
      } port_ptr;
      
      void set_input_port(VLmodule &module, unsigned int value) {
        if (INPUT_PORT != PORT_IO_TYPE) {
          // TODO: throw error
          return ;
        }

        switch (this->port_width)
        {
        case UINT8:
          module.*((this->port_ptr).uint8_p) = (uint8_t)value;
          break;
        
        case UINT16:
          module.*((this->port_ptr).uint16_p) = (uint16_t)value;
          break;

        case UINT32:
          module.*((this->port_ptr).uint32_p) = (uint32_t)value;
          break;

        case UINT64:
          module.*((this->port_ptr).uint64_p) = (uint64_t)value;
          break;
        
        default:
          // TODO: throw error
          break;
        }

        this->value = value;
      }

      // return the port value
      unsigned int get_output_port(const VLmodule &module) {
        if (OUTPUT_PORT != PORT_IO_TYPE) {
          // TODO: throw error
          return 0;
        }

        unsigned int value_tmp;
        switch (this->port_width)
        {
        case UINT8:
          value_tmp = (unsigned int)module.*((this->port_ptr).uint8_p);
          break;
        
        case UINT16:
          value_tmp = (unsigned int)module.*((this->port_ptr).uint16_p);
          break;

        case UINT32:
          value_tmp = (unsigned int)module.*((this->port_ptr).uint32_p);
          break;

        case UINT64:
          value_tmp = (unsigned int)module.*((this->port_ptr).uint64_p);
          break;
        
        default:
          // TODO: throw error
          break;
        }

        this->value = value_tmp;
        return this->value;
      }

      // TODO: more information
    };

    template <class VLmodule>
    class Verilog_data
    {
     private:
      // Private members

      std::map<std::string, vector<Port_info<VLmodule>>::iterator> port_map;

      std::vector<Port_info<VLmodule>> port_list;

     public:
      // Public members

      Verilog_data();
      ~Verilog_data();

      // The function that parse the source code to get the port map
      // It will return -1 when there is any error
      // It will return 0 if parse the code successfully
      int parse_port(const char *source_file_path, const char *source_file_type);

      // The function that add the port to set up the port_map and port_list
      // It will return -1 when there is any error
      // It will return 0 if parse the code successfully
      // main block version
      // without Vmodule
      int add_port(const char *port_name, PORT_IO_TYPE iotype, unsigned int port_width);

      // The function that add the port to set up the port_map and port_list
      // It will return -1 when there is any error
      // It will return 0 if parse the code successfully
      // shared library version
      // with Vmodule
      int add_port(const VLmodule &module, const char *port_name, PORT_IO_TYPE iotype, unsigned int port_width);

      // The function that set the value of the input port
      // It will return -1 when there is any error
      // It will return 0 if parse the code successfully
      // template <Vmodule>
      int set_input_port(VLmodule &module, const char *port_name, const ITYPE &value) const;

      // The function that get the value of the output port
      // It will return -1 when there is any error
      // It will return 0 if parse the code successfully
      // template <Vmodule>
      int get_output_port(const VLmodule &moduel, const char *port_name, OTYPE &value);
    };

  } // namespace verilog
} // namespace gr

#endif /* INCLUDED_VERILOG_VERILOG_DATA_H */


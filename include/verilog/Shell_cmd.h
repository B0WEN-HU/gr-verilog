
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

#ifndef INCLUDED_VERILOG_SHELL_CMD_H
#define INCLUDED_VERILOG_SHELL_CMD_H

#include <cstdio>
#include <string>
#include <vector>

namespace gr {
  namespace verilog {

#ifdef __linux
    class Shell_cmd
    {
     private:
      // Private members
      
      // The the shell message of the latest call
      std::vector<std::string> msgvec;

     public:
      // Public members

      Shell_cmd();
      ~Shell_cmd();

      // The exec() function of the class
      // It will return -1 when there is any error
      // It will return the line number of message if exit normally
      int exec(const char *cmd);
      
      // The get_msg() function of the class
      // It will return msg (i-th line, from 0)
      std::string get_msg(unsigned int i);

      // The print_msg() function
      // It print out the message that stored in msgvec
      void print_msg(std::ostream &out);

    };
#endif

  } // namespace verilog
} // namespace gr

#endif /* INCLUDED_VERILOG_SHELL_CMD_H */


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
#include "Shell_cmd.h"

#define BUFFER_SIZE 1024
#define _EXIT_FAILURE -1

namespace gr {
  namespace verilog {

    // The constructor
    Shell_cmd::Shell_cmd() {
      this->msgvec.clear();
    }

    // The destructor
    Shell_cmd::~Shell_cmd() {
      this->msgvec.clear();
    }

    // The exec() function of the class
    int Shell_cmd::exec(const char *cmd)
    {
      this->msgvec.clear();
      // Set up the pipe
      FILE *pipe_ptr = popen(cmd, "r");
      if (!pipe_ptr) {
        return _EXIT_FAILURE;
      }

      // Read from the pipe_ptr
      char buf[BUFFER_SIZE];
      while (NULL != fgets(buf, sizeof(buf), pipe_ptr)) {
        if ('\n' == buf[strlen(buf) - 1]) {
          // Remove the '\n'
          buf[strlen(buf) - 1] = '\0';
        }
        this->msgvec.push_back(std::string(buf));
      }

      // Close the pipe
      pclose(pipe_ptr);

      // Return the line number
      return this->msgvec.size();
    }

    
    // The print_msg(std::ostream &out) function
    void Shell_cmd::print_msg(std::ostream &out)
    {
      for (int i = 0; i < this->msgvec.size(); ++i)
      {
        out << (this->msgvec[i]) << std::endl;
      }
    }


  } /* namespace verilog */
} /* namespace gr */


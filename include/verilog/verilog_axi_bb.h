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


#ifndef INCLUDED_VERILOG_VERILOG_AXI_BB_H
#define INCLUDED_VERILOG_VERILOG_AXI_BB_H

#include <verilog/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace verilog {

    /*!
     * \brief <+description of block+>
     * \ingroup verilog
     *
     */
    class VERILOG_API verilog_axi_bb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<verilog_axi_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of verilog::verilog_axi_bb.
       *
       * To avoid accidental use of raw pointers, verilog::verilog_axi_bb's
       * constructor is in a private implementation
       * class. verilog::verilog_axi_bb::make is the public interface for
       * creating new instances.
       */
      static sptr make(const char *filename, bool overwrite, float IO_ratio,
                       const char *verilator_options, unsigned int module_flag,
                       unsigned int skip_output_items);
    };

  } // namespace verilog
} // namespace gr

#endif /* INCLUDED_VERILOG_VERILOG_AXI_BB_H */


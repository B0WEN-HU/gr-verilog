#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2019 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import verilog_swig as verilog

class qa_verilog_axi_ii (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src_data = (1, 3, 5, 9, 10)
        expected_result = (1, 3, 5, 9, 10)
        src = blocks.vector_source_i(src_data)
        vl = verilog.verilog_axi_ii("/home/bowen/Downloads/temp/saxi_passthru.v")
        dst = blocks.vector_sink_i()
        
        self.tb.connect(src, vl)
        self.tb.connect(vl, dst)
        self.tb.run()
        # check data
        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data, 5)


if __name__ == '__main__':
    gr_unittest.run(qa_verilog_axi_ii, "qa_verilog_axi_ii.xml")

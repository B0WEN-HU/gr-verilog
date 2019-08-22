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

class qa_verilog_axi_ff (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src_data = (1.2, 3.8, 5.7, 9.4, 10.2, 12.4, 17.5, 19.2, 21.4, 12.3, 45.5, 29.3)
        expected_result = (1.2, 3.8, 5.7, 9.4, 10.2, 12.4, 17.5, 19.2, 21.4, 12.3, 45.5, 29.3)
        src = blocks.vector_source_f(src_data)
        vl = verilog.verilog_axi_ff("/home/bowen/Downloads/temp/saxi_passthru.v", True, 1.0, "-Wall", 0, 0)
        dst = blocks.vector_sink_f()

        self.tb.connect(src, vl)
        self.tb.connect(vl, dst)
        self.tb.run()
        # check data
        

        result_data = dst.data()
        print (expected_result)
        print (result_data)
        def round_f(x):
            return round(x, 3)
        round_result_data = tuple(map(round_f, result_data))
        print (round_result_data)
        self.assertFloatTuplesAlmostEqual(expected_result, round_result_data, 12)

    def test_002_t (self):
        # set up fg
        src_data = (1.2, 3.8, 5.7, 9.4, 10.2, 12.4, 17.5, 19.2, 21.4)
        expected_result = (2.4, 7.6, 11.4, 18.8, 20.4, 24.8, 35.0, 38.4, 42.8)
        src = blocks.vector_source_f(src_data)
        vl = verilog.verilog_axi_ff("/home/bowen/Downloads/double/double_axi.v", True, 1.0, "", 0, 0)
        dst = blocks.vector_sink_f()

        self.tb.connect(src, vl)
        self.tb.connect(vl, dst)
        self.tb.run()
        # check data
        result_data = dst.data()
        print (expected_result)
        print (result_data)
        def round_f(x):
            return round(x, 3)
        round_result_data = tuple(map(round_f, result_data))
        print (round_result_data)
        self.assertFloatTuplesAlmostEqual(expected_result, round_result_data, 9)

if __name__ == '__main__':
    gr_unittest.run(qa_verilog_axi_ff, "qa_verilog_axi_ff.xml")

/* -*- c++ -*- */

#define VERILOG_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "verilog_swig_doc.i"

%{
#include "verilog/verilog_axi_ii.h"
#include "verilog/verilog_axi_ff.h"
%}


%include "verilog/verilog_axi_ii.h"
GR_SWIG_BLOCK_MAGIC2(verilog, verilog_axi_ii);
%include "verilog/verilog_axi_ff.h"
GR_SWIG_BLOCK_MAGIC2(verilog, verilog_axi_ff);

/* -*- c++ -*- */

#define VERILOG_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "verilog_swig_doc.i"

%{
#include "verilog/verilog_ii.h"
%}


%include "verilog/verilog_ii.h"
GR_SWIG_BLOCK_MAGIC2(verilog, verilog_ii);

/* -*- c++ -*- */
/* 
 * DESCRIPTION: The template of the AXI interface
 * This file is the implement of the shared library used in gr-verilog verilog_axi_ii
 * This file should not be modified
 */
#include "axi_module.h"

/*
 * AXI signals:
 * ACLK               input
 * ARESETn            input
 * TVALID_IN          input
 * TREADY_IN          output
 * TVALID_OUT         output
 * TREADY_OUT         input
 * TDATA_IN[31:0]     input
 * TDATA_OUT[31:0]    output
 */

void AXI_init()
{
  int argc = 0;
  char** argv = NULL;
  Verilated::commandArgs(argc, argv);

  top = new Vaxi_module;
}

void AXI_reset()
{
  top->ACLK = 0;
  top->ARESETn = 0;
  top->eval();
  top->ACLK = 1;
  top->eval();
  top->ARESETn = 1;
  top->ACLK = 0;
  top->eval();
}

void AXI_transfer(const unsigned int &gr_input,
                  unsigned int &gr_output,
                  unsigned int &time)
{
  // Suppose the module is SYNC
  // input:ouput => 1:1
  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)true;
  top->TREADY_OUT = (uint8_t)true;

  while (!transfer_flag) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      top->TDATA_IN = (unsigned int)gr_input;
      in_tranfer_flag = true;
    }

    if ((uint8_t)true == top->TVALID_OUT) {
      gr_output = top->TDATA_OUT;
      out_transfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag && out_transfer_flag;
  }

  time += cycle_tmp;
}

void AXI_nop()
{
  top->ACLK = 0;
  top->eval();
  top->ACLK = 1;
}

void AXI_close()
{
  top->final();
  delete top;
}
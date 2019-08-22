/* -*- c++ -*- */
/* 
 * DESCRIPTION: The template of the AXI interface
 * This file is the implement of the shared library used in gr-verilog verilog_axi_ii
 * This file should not be modified
 */
#include "axi_module.h"
#include <iostream>
#include <complex>
#include <cmath>

// define the _USER_MODIFIED_ to avoid unnecessary code being compiled
/* #define _USER_MODIFIED_ */

#define MAX_ITERATION 512 
#define SKIP_OUTPUT_ITEMS 0

#define ACLK          ACLK
#define ARESETn       ARESETn
#define TVALID_IN     TVALID_IN
#define TREADY_IN     TREADY_IN
#define TVALID_OUT    TVALID_OUT
#define TREADY_OUT    TREADY_OUT
#define TDATA_IN      TDATA_IN
#define TDATA_OUT     TDATA_OUT
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

unsigned int skip_output_items = 0;


void AXI_init()
{
  int argc = 0;
  char** argv = NULL;
  Verilated::commandArgs(argc, argv);

  skip_output_items = SKIP_OUTPUT_ITEMS;

  if (NULL == top)
    top = new Vaxi_module;
}

void AXI_reset(unsigned int module_flag)
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

/* Deprecated
void AXI_sync_transfer_ii(const unsigned int &gr_input,
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
      gr_output = (unsigned int)top->TDATA_OUT;
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
*/

#ifndef _USER_MODIFIED_
/* verilog_axi_ii */
unsigned char AXI_async_transfer_ii(const unsigned int &gr_input,
                                    unsigned int &gr_output,
                                    unsigned int &time)
{
  // Suppose the module is not SYNC
  // input:ouput != 1:1
  unsigned char status_code = 0;
  /* status_code    status
   *    00          ERROR
   *    01          N/O (only ouput)
   *    10          I/N (only input)
   *    11          I/O (both input and output)
   */

  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)true;
  top->TREADY_OUT = (uint8_t)true;
  
  while (!transfer_flag && cycle_tmp < MAX_ITERATION) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      top->TDATA_IN = (unsigned int)gr_input;
      in_tranfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    if ((uint8_t)true == top->TVALID_OUT) {
      if (skip_output_items > 0) {
        --skip_output_items;
      }
      else {
        gr_output = (unsigned int)top->TDATA_OUT;
        out_transfer_flag = true;
      }
    }

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag || out_transfer_flag;
  }

  status_code =
      ((unsigned char)in_tranfer_flag << 1) +
      (unsigned char)out_transfer_flag;

  time += cycle_tmp;

  return status_code;
}

unsigned char AXI_transfer_out_i(unsigned int &gr_output,
                                 unsigned int &time)
{
  // Suppose the module is not SYNC
  // input:ouput != 1:1
  unsigned char status_code = 0;
  /* status_code    status
   *    00          ERROR
   *    01          N/O (only ouput)
   *    10          I/N (only input)
   *    11          I/O (both input and output)
   */

  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)false;
  top->TREADY_OUT = (uint8_t)true;
  
  while (!transfer_flag && cycle_tmp < MAX_ITERATION) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      in_tranfer_flag = true;
      break;
    }

    if ((uint8_t)true == top->TVALID_OUT) {
      gr_output = (unsigned int)top->TDATA_OUT;
      out_transfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag || out_transfer_flag;
  }

  status_code =
      ((unsigned char)in_tranfer_flag << 1) +
      (unsigned char)out_transfer_flag;

  time += cycle_tmp;

  return status_code;
}
/* verilog_axi_ii */

/* verilog_axi_ss */
unsigned char AXI_async_transfer_ss(const unsigned short &gr_input,
                                    unsigned short &gr_output,
                                    unsigned int &time)
{
  // Suppose the module is not SYNC
  // input:ouput != 1:1
  unsigned char status_code = 0;
  /* status_code    status
   *    00          ERROR
   *    01          N/O (only ouput)
   *    10          I/N (only input)
   *    11          I/O (both input and output)
   */

  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)true;
  top->TREADY_OUT = (uint8_t)true;
  
  while (!transfer_flag && cycle_tmp < MAX_ITERATION) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      top->TDATA_IN = (unsigned short)gr_input;
      in_tranfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    if ((uint8_t)true == top->TVALID_OUT) {
      if (skip_output_items > 0) {
        --skip_output_items;
      }
      else {
        gr_output = (unsigned short)top->TDATA_OUT;
        out_transfer_flag = true;
      }
    }

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag || out_transfer_flag;
  }

  status_code =
      ((unsigned char)in_tranfer_flag << 1) +
      (unsigned char)out_transfer_flag;

  time += cycle_tmp;

  return status_code;
}
/* verilog_axi_ss */

/* verilog_axi_ff */
unsigned int float_to_fix(const float &float_data,
                          const unsigned int &integer_bits,
                          const unsigned int &decimal_bits)
{
  /* overflow detection:
  // integer_bits + decimal_bits should <= 32
  if (integer_bits + decimal_bits > 32) {
    // throw(std::runtime_error);
    return -1;
  }
  if (float_data >= (1 << integer_bits)) {
    // throw(std::runtime_error);
    return -1;
  }
  */
  return round(float_data * (1 << decimal_bits));
}
float fix_to_float(const unsigned int &fix_data,
                   const unsigned int &integer_bits,
                   const unsigned int &decimal_bits)
{
  return (float)((int)fix_data) / (1 << decimal_bits);
}
unsigned char AXI_async_transfer_ff(const float &gr_input,
                                    float &gr_output,
                                    unsigned int &time)
{
  // Suppose the module is not SYNC
  // input:ouput != 1:1
  unsigned char status_code = 0;
  /* status_code    status
   *    00          ERROR
   *    01          N/O (only ouput)
   *    10          I/N (only input)
   *    11          I/O (both input and output)
   */

  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)true;
  top->TREADY_OUT = (uint8_t)true;
  
  while (!transfer_flag && cycle_tmp < MAX_ITERATION) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      // data conversion function
      // convert float-point number to fix-point number
      // unsigned int float_to_fix(float_data, integer_bits, decimal_bits)
      // 16-bit integer and 16-bit decimal by default
      top->TDATA_IN = float_to_fix(gr_input, 16, 16);
      in_tranfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    if ((uint8_t)true == top->TVALID_OUT) {
      if (skip_output_items > 0) {
        --skip_output_items;
      }
      else {
        // data conversion function
        // convert fix-point number to float-point number
        // float fix_to_float(fix_data, integer_bits, decimal_bits)
        // 16-bit integer and 16-bit decimal by default
        gr_output = fix_to_float(top->TDATA_OUT , 16, 16);
        out_transfer_flag = true;
      }
    }

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag || out_transfer_flag;
  }

  status_code =
      ((unsigned char)in_tranfer_flag << 1) +
      (unsigned char)out_transfer_flag;

  time += cycle_tmp;

  return status_code;
}
/* verilog_axi_ff */


/* verilog_axi_cc */
unsigned int complex_to_fix(/*...*/)
{
  /* Please implement this function before use */
  static bool flag = true;
  if (flag) {
    flag = false;
        std::cout << "Please implement this function before use" << std::endl;
  }
  return 0;
}
std::complex<float> fix_to_complex(/*...*/)
{
  /* Please implement this function before use */
  return 0;
}
unsigned char AXI_async_transfer_cc(const std::complex<float> &gr_input,
                                    std::complex<float> &gr_output,
                                    unsigned int &time)
{
  // Suppose the module is not SYNC
  // input:ouput != 1:1
  unsigned char status_code = 0;
  /* status_code    status
   *    00          ERROR
   *    01          N/O (only ouput)
   *    10          I/N (only input)
   *    11          I/O (both input and output)
   */

  unsigned int cycle_tmp = 0;
  bool in_tranfer_flag = false;
  bool out_transfer_flag = false;
  bool transfer_flag = false;
  
  top->TVALID_IN  = (uint8_t)true;
  top->TREADY_OUT = (uint8_t)true;
  
  while (!transfer_flag && cycle_tmp < MAX_ITERATION) {
    
    if ((uint8_t)true == top->TREADY_IN) {
      /* Please implement this function before use */
      top->TDATA_IN = complex_to_fix(/*...*/);
      in_tranfer_flag = true;
    }

    top->ACLK = 0;
    top->eval();
    top->ACLK = 1;
    top->eval();

    if ((uint8_t)true == top->TVALID_OUT) {
      if (skip_output_items > 0) {
        --skip_output_items;
      }
      else {
        /* Please implement this function before use */
        gr_output = fix_to_complex(/*...*/);
        out_transfer_flag = true;
      }
    }

    ++cycle_tmp;

    transfer_flag = in_tranfer_flag || out_transfer_flag;
  }

  status_code =
      ((unsigned char)in_tranfer_flag << 1) +
      (unsigned char)out_transfer_flag;

  time += cycle_tmp;

  return status_code;
}
/* verilog_axi_cc */
#endif


void AXI_nop()
{
  top->ACLK = 0;
  top->eval();
  top->ACLK = 1;
}

void AXI_close()
{
  top->final();
  if (top != NULL)
  {
    delete top;
    top = NULL;
  }
}
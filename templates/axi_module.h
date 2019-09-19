#pragma once

#include "Vaxi_module.h"
#include "verilated.h"

#include <complex>

#ifdef __cplusplus
extern "C"
{
#endif

static Vaxi_module* top = NULL;

void AXI_init();

void AXI_reset(unsigned int module_flag);

// Deprecated
void AXI_sync_transfer_ii(const unsigned int &gr_input,
                          unsigned int &gr_output,
                          unsigned int &time);

/* verilog_axi_ii */
unsigned char AXI_async_transfer_ii(const unsigned int &gr_input,
                                    unsigned int &gr_output,
                                    unsigned int &time);

unsigned char AXI_transfer_out_i(unsigned int &gr_output,
                                 unsigned int &time);
/* verilog_axi_ii */

/* verilog_axi_ss */
unsigned char AXI_async_transfer_ss(const unsigned short &gr_input,
                                    unsigned short &gr_output,
                                    unsigned int &time);
/* verilog_axi_ss */

/* verilog_axi_bb */
unsigned char AXI_async_transfer_bb(const unsigned char &gr_input,
                                    unsigned char &gr_output,
                                    unsigned int &time);
/* verilog_axi_b */

/* verilog_axi_ff */
unsigned char AXI_async_transfer_ff(const float &gr_input,
                                    float &gr_output,
                                    unsigned int &time);
/* verilog_axi_ff */

/* verilog_axi_cc */
unsigned char AXI_async_transfer_cc(const std::complex<float> &gr_input,
                                    std::complex<float> &gr_output,
                                    unsigned int &time);
/* verilog_axi_cc */

void AXI_nop();

void AXI_close();

#ifdef __cplusplus
}
#endif

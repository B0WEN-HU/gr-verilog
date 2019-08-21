#pragma once

#include "Vaxi_module.h"
#include "verilated.h"

#ifdef __cplusplus
extern "C"
{
#endif

static Vaxi_module* top = NULL;

void AXI_init();

void AXI_reset(unsigned int module_flag);

/* verilog_axi_ii */
void AXI_sync_transfer_ii(const unsigned int &gr_input,
                          unsigned int &gr_output,
                          unsigned int &time);
                  
unsigned char AXI_async_transfer_ii(const unsigned int &gr_input,
                                    unsigned int &gr_output,
                                    unsigned int &time);

unsigned char AXI_transfer_out_i(unsigned int &gr_output,
                                 unsigned int &time);
/* verilog_axi_ii */


/* verilog_axi_ff */
unsigned char AXI_async_transfer_ff(const float &gr_input,
                                    float &gr_output,
                                    unsigned int &time);
/* verilog_axi_ff */

void AXI_nop();

void AXI_close();

#ifdef __cplusplus
}
#endif

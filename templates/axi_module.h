#pragma once

#include "Vaxi_module.h"
#include "verilated.h"

#ifdef __cplusplus
extern "C"
{
#endif

static Vaxi_module* top = NULL;

void AXI_init();

void AXI_reset(unsigned int skip_n);

void AXI_sync_transfer_ii(const unsigned int &gr_input,
                          unsigned int &gr_output,
                          unsigned int &time);
                  
unsigned char AXI_async_transfer_ii(const unsigned int &gr_input,
                                    unsigned int &gr_output,
                                    unsigned int &time);

unsigned char AXI_transfer_out_i(unsigned int &gr_output,
                                 unsigned int &time);

void AXI_nop();

void AXI_close();

#ifdef __cplusplus
}
#endif

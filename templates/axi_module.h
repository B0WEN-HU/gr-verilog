#pragma once

# include "Vaxi_module.h"
# include "verilated.h"

#ifdef __cplusplus
extern "C"
{
#endif

static Vaxi_module* top;

void AXI_init();

void AXI_reset();

void AXI_transfer(const unsigned int &gr_input,
                  unsigned int &gr_output,
                  unsigned int &time);
                  
unsigned char AXI_1_transfer(const unsigned int &gr_input,
                             unsigned int &gr_output,
                             unsigned int &time);

unsigned char AXI_transfer_out(unsigned int &gr_output,
                               unsigned int &time);

void AXI_nop();

void AXI_close();

#ifdef __cplusplus
}
#endif

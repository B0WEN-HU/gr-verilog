# gr-verilog -*- Makefile -*-
# DESCRIPTION: Makefile for compiling the source and building the shared library
# axi_module_cl.mk  (compile and link)
# make -j -f axi_module_cl.mk USER_VL_FILENAME=user_module_name.v USER_CPP_FILENAME=axi_module.cpp M_DIR=obj_dir

# Check for sanity to avoid later confusion
ifneq ($(words $(CURDIR)),1)
 $(error Unsupported: GNU Make cannot build in directories containing spaces, build elsewhere: '$(CURDIR)')
endif

######################################################################
# Set up variables

# If $VERILATOR_ROOT isn't in the environment, we assume it is part of a
# package inatall, and verilator is in your path. Otherwise find the
# binary relative to $VERILATOR_ROOT (such as when inside the git sources).
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
VERILATOR_COVERAGE = verilator_coverage
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
VERILATOR_COVERAGE = $(VERILATOR_ROOT)/bin/verilator_coverage
endif

# Call from block
USER_VL_FILENAME =
USER_CPP_FILENAME =
M_DIR =
VERILATOR_OPTIONS =

VM_PREFIX = Vaxi_module

VERILATOR_FLAGS =
# Options from block
VERILATOR_FLAGS += $(VERILATOR_OPTIONS)
# Generate -fPIC code for shared library
VERILATOR_FLAGS += -CFLAGS -fPIC
# Change the module to fixed name
VERILATOR_FLAGS += --prefix $(VM_PREFIX)
# Generate C++ in executable form
VERILATOR_FLAGS += -cc --exe
VERILATOR_FLAGS += -o lib_axi_module.so
# Warn abount lint issues; may not want this on less solid designs
# VERILATOR_FLAGS += -Wall


######################################################################
default: run

run:
	@echo
	@echo "-- Verilator "

	@echo
	@echo "-- VERILATE ----------------"
	$(VERILATOR) $(VERILATOR_FLAGS) $(USER_VL_FILENAME) $(USER_CPP_FILENAME)

	@echo
	@echo "-- COMPILE & LINK-----------"
# To compile, we can either just do what Verilator asks,
# or call a submakefile where we can override the rules ourselves
	$(MAKE) -j -C $(M_DIR) -f $(VM_PREFIX).mk USER_LDFLAGS=-shared

	@echo
	@echo "-- DONE --------------------"
	@echo

	
######################################################################
# Other targets

show-config:
	$(VERILATOR) -V

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.so
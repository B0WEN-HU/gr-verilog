
# gr-verilog
========

**GSoC19 project: Cycle-accurate Verilog Design Simulation Integration**

This is an OOT module for GNU Radio integrating verilog simultion. This module can run Verilog simulation at runtime in GNU Radio based on [Verilator](https://www.veripool.org/wiki/verilator/), an open-source Verilog simulator.

So far, serveral blocks namely `verilog_axi_ii`, `verilog_axi_ff`, `verilog_axi_ss` and `verilog_axi_cc`, hereinafter referred to as `verilog_axi_xx`, were implemented.

The `verilog_axi_xx` block can be used in both python code or GNU Radio Companion.

## Installation

First you need to install the dependencies (see below).

Then, you need to download this repository
```bash
$ git clone https://github.com/B0WEN-HU/gr-verilog.git
```

After this, gr-verilog should be installed as any other GNU Radio out-of-tree module.
```bash
$ cd gr-verilog
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```
If your GNU Radio is installed in other directory (rather than /usr/local), then replace the cmake line `cmake ..` above with: `cmake -DCMAKE_INSTALL_PREFIX=<directory of GNU Radio> ..`

## Dependencies

gr-verilog requires:

`GNU Radio` version 3.7.11 or newer.

`GNU Make` version 4.1 or newer.

`Verilator` version 3.916 or newer.

`gcc` version 7.4.0 or newer.

## Usage

`verilog_axi_xx` blocks require user's Verilog module to follow [AXI4-stream](https://static.docs.arm.com/ihi0051/a/IHI0051A_amba4_axi4_stream_v1_0_protocol_spec.pdf) input and output interfaces and protocols. If your design does not use AXI4-stream interfaces and protocols, you need to have an additional top Verilog module to wrap it up in order to use `verilog_axi_xx` blocks. The `verilog_axi_xx` blocks use only part of the interfaces of AXI4-stream protocol, which are listed below:

```
// These are input and output ports required in the Verilog design
 * AXI signals:
 * ACLK               input
 * ARESETn            input
 * TVALID_IN          input
 * TREADY_IN          output
 * TVALID_OUT         output
 * TREADY_OUT         input
 * TDATA_IN[31:0]     input
 * TDATA_OUT[31:0]    output
```

Once you have a Verilog module with the interfaces required, now you can test your Verilog module in GNU Radio. The easiest way is to use GNU Radio Companion. You should be able to find a block named `Verilog AXI` in GNU Radio Companion if you installed this OOT module correctly. You need to choose the type of the block (usually Int), specify the Verilog file, and leave other options as default. Then, if everything goes well, you can use your Verilog design just like any other blocks in GNU Radio.

You can also use the `verilog_axi_xx` blocks in python. First you need to import the module:

```python
import verilog
```
After that, you need to use following code to instantiate a `verilog_axi_xx` object with parameters wrapped in `"${ }"`. `${type.fcn}` represents the data type of the block `ii` for `Int`, `ff` for `Float`, `ss` for `Short` and `cc` for `Complex`. `${file}` is the filename of the Verilog file, `String` type, it should be an absolute path. `${overwrite}` is `Boolean` type, indicates wether to overwrite the template files in user's Verilog file directory. `${IO_ratio}` is `Float` type, it represents the ratio of number of input and output items, the right parameter can help the scheduler improve the performance. `${verilator_options}` is `String` type, the block passes the options to Verilator. `${skip_output_items}` is `Int` type, the block will skip first `skip_output_items` output items.

```python
vl = verilog.verilog_verilog_axi_${type.fcn}(${file}, ${overwrite}, ${IO_ratio}, ${verilator_options}, ${module_flag}, ${skip_output_items})
```

## Examples
You can find the example `apps/verilog_axi_ii_demo.grc`. you need to specify the Verilog file for `Verilog AXI` block before running the flow graph. You can use the Verilog file `double_axi.v` in `examples/double/`. `double_axi.v` just take the input and shift 1 bit left (which double the input). Then, you should be able to see the results in the GUI.

The example `apps/verilog_axi_ff_demo.grc` shows how `Verilog AXI` works with `Float` input and output. You can use a very same Verilog module as above, `double_axi.v`. You can see more obvious effects of what `double_axi.v` does. But how could the same Verilog module be able to deal with both integer and float number? Well, actually there is a pair of functions in the cpp template `templates/axi_module.cpp` named `float_to_fix` and `fix_to_float`, which can automatically convert floating point number and fixed point number. You may need to modify the function in order to fit in certain design.

## Hint
The `Complex` type of `Verilog AXI`, `verilog_axi_cc`, is not the block that is ready to use, you definitely will not use only 32 bits to represent a complex number, so you have to some do modifications on the template file in order to fit in your design.

## Future Work
Add more examples.

Bring verilog_general_xx into the module.
#!/bin/sh

# Cleanup old directories
rm -rf obj_dir
rm -f counter.vcd

# Run Verilator to translate Verilog into C++, including C++ testbench
verilator -Wall --cc --trace top.sv bin2bcd.sv counter.sv --exe top_tb.cpp

# Build C++ project via make automatically generated by Verilator
make -j -C obj_dir/ -f Vtop.mk Vtop

# Run executable simulation file
obj_dir/Vtop
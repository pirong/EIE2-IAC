// Mandatory header files
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){

    Verilated::commandArgs(argc, argv);

    // Initialise counter module as Vcounter
    Vtop* top = new Vtop;

    // Dump waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Initialise VBuddy
    if (vbdOpen() != 1) return (-1);
    vbdHeader("Lab 1: Counter");
    // vbdSetMode(1);

    // Set initial signal levels
    top->clk = 1;
    top->rst = 1;
    top->en = 1;

    // Run simulation for 300 clock cycles
    for (int i=0; i<300; i++){

        // Toggle the clock twice, evaluating on both edges
        for (int clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }

        // Send count value to VBuddy
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);
        vbdCycle(i+1);

        // Change rst and en signals during simulation
        top->rst = 0;
        top->en = 1;
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
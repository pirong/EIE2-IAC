// Mandatory header files
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){

    Verilated::commandArgs(argc, argv);

    // Initialise counter module as Vcounter
    Vcounter* top = new Vcounter;

    // Dump waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Initialise VBuddy
    if (vbdOpen() != 1) return (-1);
    vbdHeader("Lab 1: Counter");
    vbdSetMode(1);

    // Set initial signal levels
    top->clk = 1;
    top->rst = 1;
    top->ld = 1;
    top->v = vbdValue();

    // Run simulation for 300 clock cycles
    for (int i=0; i<300; i++){

        // Toggle the clock twice, evaluating on both edges
        for (int clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }

        /* Send count value to VBuddy
        vbdHex(4, (int(top->count) >> 12) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        */
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);


        // Change rst and en signals during simulation
        top->rst = 0;
        top->ld = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
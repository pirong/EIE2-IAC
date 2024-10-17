// Mandatory header files
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){

    Verilated::commandArgs(argc, argv);

    // Initialise counter module as Vcounter
    Vcounter* top = new Vcounter;

    // Dump waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Set initial signal levels
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // Run simulation for 300 clock cycles
    for (int i=0; i<300; i++){

        // Toggle the clock twice, evaluating on both edges
        for (int clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }

        // Change rst and en signals during simulation
        top->rst = (i<2) | (i == 5);
        top->en = (i>4) and (i<15 or i>16);
        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}
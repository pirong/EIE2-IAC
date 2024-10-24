// Mandatory header files
#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){

    Verilated::commandArgs(argc, argv);

    // Initialise counter module as Vcounter
    Vsinegen* sinegen = new Vsinegen;

    // Dump waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    sinegen->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Initialise VBuddy
    if (vbdOpen() != 1) return (-1);
    vbdHeader("Lab 2: SineGen");
    // vbdSetMode(1);

    /*
    const int min_incr = 1;
    const int max_incr = 10;
    const int min_vbd_val = 1;
    const int max_vbd_val = 100;
    */

    // Set initial signal levels
    sinegen->clk = 1;
    sinegen->rst = 0;
    sinegen->en = 1;
    sinegen->incr = 1;
    sinegen->offset = vbdValue();
    
    int SIM_CYCLES = 1000000;
    // Run simulation for 300 clock cycles
    for (int i=0; i<SIM_CYCLES; i++){

        // Toggle the clock twice, evaluating on both edges
        for (int clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            sinegen->clk = !sinegen->clk;
            sinegen->eval();
        }

        // Send count value to VBuddy
        vbdPlot(int(sinegen->dout1), 0, 255);
        vbdPlot(int(sinegen->dout2), 0, 255);
        vbdCycle(i+1);

        sinegen->offset = vbdValue();

        /*
        double r = (double) (vbd_val - min_vbd_val) / (max_vbd_val - min_vbd_val);
        int new_inc = r * (max_vbd_val - min_vbd_val) + min_vbd_val;
        new_inc = new_inc < min_incr? min_incr : new_inc;
        new_inc = new_inc > max_incr? max_incr: new_inc;
        sinegen -> incr = new_inc;
        */

        // Change rst and en signals during simulation
        // top->rst = 0;
        // top->en = 1;
        if (Verilated::gotFinish() || (vbdGetkey()=='q')) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
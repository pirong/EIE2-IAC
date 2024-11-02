#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include "../vbuddy.cpp" // include vbuddy code
#define MAX_SIM_CYC 100000

int main(int argc, char **argv, char **env)
{
    int cmd_seq = 1;
    int cmd_delay = 0;

    // Force delay to only last 1 cycle
    bool isDelayPassed = false;

    // Measure reaction time when all lights are null
    bool in_sequence, is_timing;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop *top = new Vtop;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("challenge.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("L3T4: F1");
    vbdSetMode(1); // Flag mode set to one-shot

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->cmd_seq = cmd_seq;
    top->cmd_delay = cmd_delay;
    top->trigger = 0;
    top->N = 30;

    // run simulation for MAX_SIM_CYC clock cycles
    for (int simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        // dump variables into VCD file and toggle clock
        for (int tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        
        vbdBar(top->data_out & 0xFF);
        
        // set up input signals of testbench
        top->rst = (simcyc < 2);
        top->cmd_seq = top->cmd_seq_output;

        // Force delay pulse to only last 1 cycle
        if (top->cmd_delay_output and isDelayPassed == false){
            top->cmd_delay = 1;
            isDelayPassed = true;
        } else if (top->cmd_delay_output and isDelayPassed == true) {
            top->cmd_delay = 0;
        } else {
            isDelayPassed = false;
        }

        // Get reaction time
        if (top->data_out){
            in_sequence = true;
        }
        int flag_val = vbdFlag();

        if(!is_timing){
            top->trigger = flag_val;
        } else if (flag_val) {
            is_timing = false;
            in_sequence = false;
            int runtime = vbdElapsed();
            vbdHex(4, (runtime >> 12) & 0xF);
            vbdHex(3, (runtime >> 8) & 0xF);
            vbdHex(2, (runtime >> 4) & 0xF);
            vbdHex(1, (runtime) & 0xF);
        }
        if (top->data_out == 0){
            if (in_sequence) {
                is_timing = true;
                in_sequence = false;
                vbdInitWatch();
            }
        }
        

        vbdCycle(simcyc);

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }

    vbdClose(); // ++++
    tfp->close();
    exit(0);
}
#include "Vcounter.h" //note Vcounter.h for module counter
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){
    int i;      // counts n of clk cycles to simulate
    int clk;    // module clk signal

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter; //instantiate the counter module as Vcounter (DUT)
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp-> open ("counter.vcd");
    // ^^ turn on signal tracing and tell Verilator to dump
    // the waveform data to counter.vcd

    //initialize simulation inputs (top is top-level entity)
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (int i=0; i<300; i++){
        //dump variables into VCD file and toggle clk
        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
            //forces module to evaluate on both edges of clk
        }
        top->rst = (i<2) | (i == 22); //change rst and enable signals during simulation
        top->en = ((i>4)&&(i<14)) | (i>18);
        if(Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}
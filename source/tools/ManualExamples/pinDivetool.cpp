

#include <stdio.h>
#include "pin.H"

FILE* trace;


static UINT64 cnt = 0;

void printFPContext(CONTEXT* ctx) 
{
    FPSTATE* fpstate = new FPSTATE;
    PIN_GetContextFPState(ctx, fpstate);
    delete fpstate;
}

VOID printip(VOID* ip) 
{ 
    cnt++;
    // fprintf(trace, "%p\n", ip); 
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    // Insert a call to printip before every instruction, and pass it the IP
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printip, IARG_INST_PTR, IARG_END);
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printFPContext, IARG_CONTEXT, IARG_END);
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    fprintf(trace, "ins cnt = %ld\n", cnt);
    fprintf(trace, "#eof\n");
    fclose(trace);
}


INT32 Usage()
{
    PIN_ERROR("This pintool is designed to test the physical context function of pin\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}



int main(int argc, char* argv[])
{
    trace = fopen("pinDivetool.out", "w");

    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
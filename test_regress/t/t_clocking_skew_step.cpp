// -*- mode: C++; c-file-style: "cc-mode" -*-
//
// DESCRIPTION: Verilator: Verilog Test module
//
// This file ONLY is placed under the Creative Commons Public Domain.
// SPDX-FileCopyrightText: 2026 Andrei Girjoaba
// SPDX-License-Identifier: CC0-1.0

#include <verilated.h>

#include <memory>
#include <string>

#include VM_PREFIX_INCLUDE

int main(int argc, char** argv) {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    contextp->threads(1);
    contextp->commandArgs(argc, argv);

    const std::unique_ptr<VM_PREFIX> topp{new VM_PREFIX{contextp.get(), "top"}};
    topp->clk = 0;
    topp->eval();

    // Step time by 1 and toggle the clock every 5: edges at 5, 15, 25
    while ((contextp->time() < 100) && !contextp->gotFinish()) {
        contextp->timeInc(1);
        const uint64_t t = contextp->time();
        if (t % 5 == 0) topp->clk = !topp->clk;
        topp->eval();
        // The rounded-zero skew drives at the edge, the skew of 1 one unit later
        const bool v0Exp = t >= 5 && t < 15;
        const bool v1Exp = t >= 6 && t < 16;
        if (topp->v0 != v0Exp || topp->v1 != v1Exp) {
            const std::string msg
                = "%Error: at " + std::to_string(t) + " got v0=" + std::to_string(topp->v0)
                  + " v1=" + std::to_string(topp->v1) + " expected v0=" + std::to_string(v0Exp)
                  + " v1=" + std::to_string(v1Exp);
            vl_fatal(__FILE__, __LINE__, "main", msg.c_str());
            break;
        }
    }

    if (!contextp->gotFinish()) {
        vl_fatal(__FILE__, __LINE__, "main", "%Error: Timeout; never got a $finish");
    }
    topp->final();
    return 0;
}

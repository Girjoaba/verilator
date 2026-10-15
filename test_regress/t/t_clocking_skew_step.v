// DESCRIPTION: Verilator: Verilog Test module
//
// This file ONLY is placed under the Creative Commons Public Domain.
// SPDX-FileCopyrightText: 2026 Andrei Girjoaba
// SPDX-License-Identifier: CC0-1.0

`timescale 1ns / 1ns

// A skewed drive is applied in the eval() of its time step, so a harness that steps time
// itself never misses a time slot. A skew below the time precision rounds to zero.
module t (
    input clk,
    output bit v1,  // Driven with an output skew of 1
    output bit v0  // Driven with an output skew that rounds to 0
);
  clocking cb1 @(posedge clk);
    default output #1;
    output v1;
  endclocking
  clocking cb0 @(posedge clk);
    default output #0.1;
    output v0;
  endclocking

  initial begin
    @(cb1);
    cb1.v1 <= 1;
    cb0.v0 <= 1;
    @(cb1);
    cb1.v1 <= 0;
    cb0.v0 <= 0;
    @(cb1);
    $write("*-* All Finished *-*\n");
    $finish;
  end
endmodule

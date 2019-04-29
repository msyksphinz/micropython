/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpstate.h"

#if MICROPY_NLR_RISCV

#undef nlr_push

__attribute__((naked)) unsigned int nlr_push(nlr_buf_t *nlr) {

  __asm volatile (
      "sw  x2,  8 (x10) \n" // save regs...
      "sw  x8,  12(x10) \n"
      "sw  x9,  16(x10) \n"
      "sw  x18, 20(x10) \n"
      "sw  x19, 24(x10) \n"
      "sw  x20, 28(x10) \n"
      "sw  x21, 32(x10) \n"
      "sw  x22, 36(x10) \n"
      "sw  x23, 40(x10) \n"
      "sw  x24, 44(x10) \n"
      "sw  x25, 48(x10) \n"
      "sw  x26, 52(x10) \n"
      "sw  x27, 56(x10) \n"
      "sw  ra,  60(x10) \n"
      "j   nlr_push_tail \n" // do the rest in C
                  );

  return 0; // needed to silence compiler warning
}

NORETURN void nlr_jump(void *val) {
    MP_NLR_JUMP_HEAD(val, top)

    __asm volatile (
    "lw  sp,  8 (%0) \n" // restore regs...
    "lw  s0,  12(%0) \n"
    "lw  s1,  16(%0) \n"
    "lw  s2,  20(%0) \n"
    "lw  s3,  24(%0) \n"
    "lw  s4,  28(%0) \n"
    "lw  s5,  32(%0) \n"
    "lw  s6,  36(%0) \n"
    "lw  s7,  40(%0) \n"
    "lw  s8,  44(%0) \n"
    "lw  s9,  48(%0) \n"
    "lw  s10, 52(%0) \n"
    "lw  s11, 56(%0) \n"
    "lw  ra,  60(%0) \n"
    "li  a0, 1       \n" // return 1, non-local return
    "ret             \n" // return
    :                               // output operands
    : "r"(top)                      // input operands
    :                               // clobbered registers
    );

    for (;;); // needed to silence compiler warning
}

#endif // !MICROPY_NLR_SETJMP && defined(__riscv32__)

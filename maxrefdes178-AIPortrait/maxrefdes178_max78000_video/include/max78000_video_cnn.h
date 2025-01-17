/*******************************************************************************
 * Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef _MAX78000_VIDEO_CNN_H_
#define _MAX78000_VIDEO_CNN_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>
typedef int32_t q31_t;
typedef int16_t q15_t;

/* Return codes */
#define CNN_FAIL 0
#define CNN_OK 1

/*
  SUMMARY OF OPS
  Hardware: 615,316,944 ops (611,837,952 macc; 3,478,992 comp; 0 add; 0 mul; 0 bitwise)
    Layer 0: 24,285,184 ops (23,789,568 macc; 495,616 comp; 0 add; 0 mul; 0 bitwise)
    Layer 1: 32,215,040 ops (31,719,424 macc; 495,616 comp; 0 add; 0 mul; 0 bitwise)
    Layer 2: 16,107,520 ops (15,859,712 macc; 247,808 comp; 0 add; 0 mul; 0 bitwise)
    Layer 3: 17,904,128 ops (17,842,176 macc; 61,952 comp; 0 add; 0 mul; 0 bitwise)
    Layer 4: 4,019,136 ops (3,902,976 macc; 116,160 comp; 0 add; 0 mul; 0 bitwise)
    Layer 5: 6,911,520 ops (6,830,208 macc; 81,312 comp; 0 add; 0 mul; 0 bitwise)
    Layer 6: 6,870,864 ops (6,830,208 macc; 40,656 comp; 0 add; 0 mul; 0 bitwise)
    Layer 7: 1,517,824 ops (1,517,824 macc; 0 comp; 0 add; 0 mul; 0 bitwise)
    Layer 8: 27,320,832 ops (27,320,832 macc; 0 comp; 0 add; 0 mul; 0 bitwise)
    Layer 9: 27,347,936 ops (27,320,832 macc; 27,104 comp; 0 add; 0 mul; 0 bitwise)
    Layer 10: 27,320,832 ops (27,320,832 macc; 0 comp; 0 add; 0 mul; 0 bitwise)
    Layer 11: 27,375,040 ops (27,320,832 macc; 54,208 comp; 0 add; 0 mul; 0 bitwise)
    Layer 12: 15,611,904 ops (15,611,904 macc; 0 comp; 0 add; 0 mul; 0 bitwise)
    Layer 13: 53,898,240 ops (53,526,528 macc; 371,712 comp; 0 add; 0 mul; 0 bitwise)
    Layer 14: 214,601,728 ops (214,106,112 macc; 495,616 comp; 0 add; 0 mul; 0 bitwise)
    Layer 15: 32,215,040 ops (31,719,424 macc; 495,616 comp; 0 add; 0 mul; 0 bitwise)
    Layer 16: 32,215,040 ops (31,719,424 macc; 495,616 comp; 0 add; 0 mul; 0 bitwise)
    Layer 17: 31,719,424 ops (31,719,424 macc; 0 comp; 0 add; 0 mul; 0 bitwise)
    Layer 18: 15,859,712 ops (15,859,712 macc; 0 comp; 0 add; 0 mul; 0 bitwise)

  RESOURCE USAGE
  Weight memory: 279,264 bytes out of 442,368 bytes total (63%)
  Bias memory:   876 bytes out of 2,048 bytes total (43%)
*/

/* Number of outputs for this network */
#define CNN_NUM_OUTPUTS 247808

/* Use this timer to time the inference */
#define CNN_INFERENCE_TIMER MXC_TMR0

/* Port pin actions used to signal that processing is active */
#define CNN_START
#define CNN_COMPLETE
#define SYS_START
#define SYS_COMPLETE

/* Run software SoftMax on unloaded data */
void softmax_q17p14_q15(const q31_t* vec_in, const uint16_t dim_vec, q15_t* p_out);
/* Shift the input, then calculate SoftMax */
void softmax_shift_q17p14_q15(q31_t* vec_in, const uint16_t dim_vec, uint8_t in_shift, q15_t* p_out);

/* Stopwatch - holds the runtime when accelerator finishes */
extern volatile uint32_t cnn_time;

/* Custom memcopy routines used for weights and data */
void memcpy32(uint32_t* dst, const uint32_t* src, int n);
void memcpy32_const(uint32_t* dst, int n);

/* Enable clocks and power to accelerator, enable interrupt */
int cnn_enable(uint32_t clock_source, uint32_t clock_divider);

/* Disable clocks and power to accelerator */
int cnn_disable(void);

/* Perform minimum accelerator initialization so it can be configured */
int cnn_init(void);

/* Configure accelerator for the given network */
int cnn_configure(void);

/* Load accelerator weights */
int cnn_load_weights(void);

/* Verify accelerator weights (debug only) */
int cnn_verify_weights(void);

/* Load accelerator bias values (if needed) */
int cnn_load_bias(void);

/* Start accelerator processing */
int cnn_start(void);

/* Force stop accelerator */
int cnn_stop(void);

/* Continue accelerator after stop */
int cnn_continue(void);

/* Unload results from accelerator */
int cnn_unload(uint32_t* out_buf);

/* Turn on the boost circuit */
int cnn_boost_enable(mxc_gpio_regs_t* port, uint32_t pin);

/* Turn off the boost circuit */
int cnn_boost_disable(mxc_gpio_regs_t* port, uint32_t pin);

#endif /* _MAX78000_VIDEO_CNN_H_ */

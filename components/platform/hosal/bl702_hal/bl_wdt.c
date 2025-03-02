/*
 * Copyright (c) 2016-2022 Bouffalolab.
 *
 * This file is part of
 *     *** Bouffalolab Software Dev Kit ***
 *      (see www.bouffalolab.com).
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Bouffalo Lab nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <bl702_timer.h>
#include "bl_wdt.h"
#define MAX_MS_WDT (65535)

static uint8_t wdt_enabled = 0;
static int wdt_value = 0;

static void Timer_Watchdog_Case(int ms)
{
    WDT_Disable();
    WDT_Set_Clock(TIMER_CLKSRC_32K, 31);
    WDT_SetCompValue(1 * ms);

    WDT_ResetCounterValue();
    WDT_IntMask(WDT_INT, MASK);

    WDT_Enable();
}

void bl_wdt_feed(void)
{
    WDT_ResetCounterValue();
}

void bl_wdt_disable(void)
{
    WDT_Disable();
    wdt_enabled = 0;
}

int bl_wdt_init(int ms)
{
    if (ms >= MAX_MS_WDT) {
        return -1;
    }
    Timer_Watchdog_Case(ms);
    wdt_value = ms;
    wdt_enabled = 1;
    return 0;
}

int bl_wdt_restore(void)
{
    if (!wdt_enabled) {
        return -1;
    }
    Timer_Watchdog_Case(wdt_value);
    return 0;
}

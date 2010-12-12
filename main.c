/*
 * Copyright (c) 2010 Benjamin Nadland <benjamin.nadland@freenet.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <io.h>
#include <signal.h>

/** PIN ALIASES **/
/* outputs */
#define LED_RED BIT0
#define LED_GREEN BIT6
/* inputs */
#define BUTTON BIT3

/** INTERRUPT FLAGS **/
/* we check for these in the main loop */
/* char means we have 16 bits (BIT0-BITF) */
volatile unsigned char interrupt_flags;
#define TIMER_A_FLAG BIT0
#define BUTTON_FLAG BIT1

void
init(void)
{
	/** WATCHDOG **/
	/* stop the watchdog */
	WDTCTL = WDTPW + WDTHOLD;
	
	/** TIMER **/
	TACCR0 = 1000;
	/* SMCLK */
	TACTL |= TASSEL_1;
	/* up mode */
	TACTL |= MC_UPTO_CCR0;
	/* enable interrupts at count to CCR0 */
	TACCTL1 |= CCIE;
	
	/** GPIO **/
	/* set all pins as output and low to prevent floating inputs */
	P1DIR = 0xFF;
	P1OUT = 0;
	/* activate the green led */
	P1OUT |= LED_GREEN;
	
	/** FINAL **/
	/* enable interrupts */
	eint();
}

int
main(void)
{
	init();

	for(;;)
	{
		if(interrupt_flags & TIMER_A_FLAG)
		{
			/* reset flag */
			interrupt_flags &= ~TIMER_A_FLAG;
			/* toggle leds */
			P1OUT ^= (LED_GREEN + LED_RED);
		}

		/* enter sleep mode */
		LPM3;
	}
}

/** INTERRUPT SERVICE ROUTINES **/

interrupt(TIMERA1_VECTOR)
timer_a(void)
{
	/* reset interrupt flag */
	TACCTL1 &= ~CCIFG;
	/* set flag */
	interrupt_flags |= TIMER_A_FLAG;
	/* exit sleep mode to execute main loop */
	LPM3_EXIT;
}

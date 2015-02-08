#include "contiki.h"
#include <stdio.h>

#include "dev/leds.h"

/*
 * Blink a led once every second.
 */


PROCESS(blinking_led, "Led blinking");
AUTOSTART_PROCESSES(&blinking_led);

PROCESS_THREAD(blinking_led, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer timer;

  etimer_set(&timer, CLOCK_SECOND);
  
  printf("Hello, world!\n");

  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
      printf("Curent time: %d\n", RTIMER_NOW());
    etimer_restart(&timer);
    leds_toggle(LEDS_RED);
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

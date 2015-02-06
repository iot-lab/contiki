IoT-LAB Hello World !
=====================

Prints "Hello World !" and echoes whatever arrives on the serial link.

This example shows how to use the two serial link input APIs:
* byte-level input: ``uart1_set_input()``
* line-level event: ``serial_line_event_message``

Notes:
* printf does not currently support the %l modifier (e.g. %lu)
* line buffered input is limited to 80 characters (Contiki's default)

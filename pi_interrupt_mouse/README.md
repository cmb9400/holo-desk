#Arduino Leonardo

The most recent code flashed to the leo is pi_interrupt_mouse.ino (5/2/2016).

In order to tell the leo to move the cursor of the client device, send it a signal over USB Serial in the format of:

`xxxxxyyyyy.`

where the x's are the X-coordinates, the y's are the Y-coordinates, and the period ends the message.
both x and y are in the range [0-32767].

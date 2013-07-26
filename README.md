Brisophone
==========
This is a semi-random music generator based on a STM32F4 Discovery board and a simple expansion board providing four push buttons and a 10 LED bargraph.

It's a cousin of my previous "Ambiantiseur" project.

Now we have 3 effects : LP filter, chorus/flanger, delay.

Through the 4 buttons you can edit about 20 parameters.

All push buttons are active low. They are connected to PD8, PD9, PD10, PD11.

The first two are used to choose the parameter you'd like to modify : you scroll the list forward or backward. The bargraph displays a configuration of LEDs for each parameter.

The two second buttons are generally used for increasing or decreasing the parameter value or sometimes toggle a value.

The bargraph (common cathods) is connected to pins PE6, PE7, .... PE14, PE15. Each output is active at high level (3V). I've used 120 ohms resistors. The display is multiplexed (one Led is on at a time) and not bright...This system doesn't work very well in fact....

Early demos :

http://youtu.be/H8p6o9Nr7k8

http://youtu.be/NJDcqiR5RQg

This project was built with Coocox.org's free IDE : CoIDE 1.7 and the GNU Tools for ARM Embedded Processors (https://launchpad.net/gcc-arm-embedded).

The main project file to open with CoIDE is "Brisophone.coproj".

Be sure to adapt your settings :

- Toolchain location  : Project > Select your Toolchain Path ...

- libm location : Configuration window > link > linked libraries > add ...

( example : C:\program files\gnu tools arm embedded\4.7 2013q2\arm-none-eabi\lib\armv7e-m\fpu\  )


Brisophone
==========
This is a semi-random music generator based on a STM32F4 Discovery board and a simple expansion board providing four push buttons and a 10 LED bargraph.

It's a cousin of my previous "Ambiantiseur" project.

Now we have 3 effects : LP filter, chorus/flanger, delay.

Through the 4 buttons you can edit about 20 parameters.

Early demos :

http://youtu.be/H8p6o9Nr7k8

http://youtu.be/NJDcqiR5RQg

This project was built with Coocox.org's free IDE : CoIDE 1.7.4.

The main project file to open with CoIDE is "Brisophone.coproj".

Be sure to adapt your settings :

- Toolchain location  : Project > Select your Toolchain Path ...

- libm location : Configuration window > link > linked libraries > add ...

( example : C:\program files\gnu tools arm embedded\4.7 2013q2\arm-none-eabi\lib\armv7e-m\fpu\  )


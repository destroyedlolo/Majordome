This example will :

    Open a 2004 LCD I2C screen (**.Renderer**)
    Display some messages on it (**.Decoration**)

And then, when you interrupt the process (i.e. `Control-C`), will cleanly turn off the screen (**.shutdown**).

> [!NOTE]
> - you need to customize Renderer as per your own configuration
> - Will work as well on 1602 screen but obviously the result with be weird

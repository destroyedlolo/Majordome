This example demonstrates very basic usage of Painting (a.k.a Sub Surface).

> [!NOTE]
> - Will work as well on 1602 screen but obviously the result with be weird

# Components
## LCD.Renderer
Open a **2004 LCD I2C** screen.
> [!TIP]
> - you need to customize Renderer as per your own hardware configuration
## LCD.shutdown
Turns off the screen at Majordome's shutdown.
## LCD.Decoration
Fills the screen with a filled box character.
## Sub.Painting
Creates a subsurface starting at 5x1 and 10x2 long.
## Sub.Decoration
Clear and display a message on the painting.
> [!IMPORTANT]  
> Without this decoration, the painting remain invisible
> (by default, a painting is fully transparent).

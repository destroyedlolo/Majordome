# .Painting
Subpart of a surface to paint on. 

## Renderer specifics
## I2C LCD
- The Painting is sharing the same memory as its parent. Consequently, what is behind
is destroyed.
- Drawing is enforced not going outside the Painting area.

## Syntax
Decorations are basically **[Lua tasks](../Task(lua).md)** and are following the
same syntax.

In the header of the script (comment block at the very beginning of the script),
each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals
See [this page](../Headers%20and%20Shared%20Directives.md#general-directives).
### Dependancies
See [this page](../Headers%20and%20Shared%20Directives.md#dependancies)

### Painting's owns
#### -->> Origin=*x,y*
Origin coordinate of the painting as per its parent.<br>
If not set : `0,0`

#### -->> Size=*w,h*
Size of the surface.<br>
If not set : guessed from parent's size

## at Lua side
### Exported object
The surface to apply on is embodied as **MAJORDOME_PAINTING**


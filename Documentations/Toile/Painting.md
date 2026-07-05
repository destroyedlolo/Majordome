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

#### -->> Persistent
This object caches its content, ensuring data is retained even when hidden.

#### -->> hidden
This object is hidden by default at launch.  
This property only applies to persistent paintings

## at Lua side
### Exported object
The surface to apply on is embodied as **MAJORDOME_PAINTING**

### Exposed Methods

- `getContainer()` returns the container (directory) in which this Painting has been defined
- `getName()` returns Painting's name
- `isEnabled()` returns a boolean reflecting if this Painting is enabled or not
- `Enable()` to enable this Painting
- `Disable()` to disable this Painting
- `isVisible()` retuns a boolean reflecting if this Painting is visible or not
- `setVisibility(boolean)` changes Painting visibilities

Additionally, all Seléné methods applicable to Surface and SubSurface can be used.

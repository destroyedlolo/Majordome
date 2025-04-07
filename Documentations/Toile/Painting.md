# .Painting
Subpart of a surface to paint on. 

## Renderer specifics
## LCD
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

#### -->> name=
Unique name to identify the Painting. If not set, uses the filename.
`-->> name=toto`
#### -->> quiet
Remove some trace.

### Painting owns
#### Indicate the parent surface
As Renderers and Paintings can share the same name even being totally distinct
objects, we have to specify which is the kind of our parent.
##### -->> Renderer Parent=
Create this Painting upon a given Renderer.
##### -->> Painting Parent=
Create this Painting upon a given Painting.

#### -->> Origin=*x,y*
Origin coordinate of the painting as per its parent.<br>
If not set : `0,0`

#### -->> Size=*w,h*
Size of the surface.<br>
If not set : guessed from parent's size

## at Lua side
### Exported object
The surface to apply on is embodied as **MAJORDOME_PAINTING**


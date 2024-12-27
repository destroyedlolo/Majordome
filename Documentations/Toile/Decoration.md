# .Decoration
Decorations is a way to paint a surface (and by extension a Renderer).

## Syntax

Decorations are basically **[Lua tasks](../Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the Renderer. If not set, uses the filename.
`-->> name=toto`
#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

### Decoration owns

## at Lua side
### Exported object
The surface to apply on is embodied as **MAJORDOME_RENDERER**

## Examples
Displays some messages on an LCD 2004 screen.

```lua
-- This is an example of Decoration script
--
-- In the header of the script (comment bloc at the very beginning of
-- the script), each line starting with -->> are Majordome's commands.
-- Consequently, '--->>' are commented out commands.
--
-- Name of this script
-- if not set, takes the filename
--->> name=LCD
--
-- On which object this decoration will be applied ?
-- On a Surface object
--->> ApplyOn=
-- On a Renderer
-->> ApplyOnRenderer=LCD
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet

-- The surface to apply on is embodied as MAJORDOME_RENDERER

MAJORDOME_RENDERER:WriteString("Majordome : ".. string.format("%.4f", MAJORDOME_VERSION))

MAJORDOME_RENDERER:SetCursor(0,2)
MAJORDOME_RENDERER:WriteString("Bla bla")

MAJORDOME_RENDERER:SetCursor(0,3)
for c=33,52 do
	MAJORDOME_RENDERER:WriteString(string.char(c))
end
```

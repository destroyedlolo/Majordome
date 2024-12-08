# .renderer
Renderers are links to physical devices to build user interfaces (UI) with **Majordome**.<br>
Typical examples are going from small textual LCDs to large graphical screens,
but also vocal generators ... whatever [Séléné](https://github.com/destroyedlolo/Selene) can interact with.

## Syntax

Renderers are basically **[Lua tasks](../Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the Renderer. If not set, uses the filename.
`-->> name=toto`
#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

### Renderer owns
#### -->> fatal
If set, failure of this renderer script will crash Majordome.

## at Lua side
### Lua script
The script is expected to return a Séléné renderer object. Any other return (or no return at all) is considered a script failure.

> [!TIP]
> Renderers are tightly linked to Selene and the underlying hardware. Most of the time, it requires some customizations at the operating system level and access to Séléné administrative API.

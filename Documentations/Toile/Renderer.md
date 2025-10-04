# .Renderer
Renderers are links to physical devices to build user interfaces (UI) with **Majordome**.<br>
Typical examples are going from small textual LCDs to large graphical screens,
but also vocal generators ... whatever [Séléné](https://github.com/destroyedlolo/Selene) can interact with.

## Syntax

Renderers are basically **[Lua tasks](../Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).
### Dependancies
See [this page](Headers%20and%20Shared%20Directives.md#dependancies)

### Renderer owns
#### -->> fatal
If set, failure of this renderer script will crash Majordome.

## at Lua side
### Lua script
The script is expected to return a Séléné renderer object. Any other return (or no return at all) is considered a script failure.

> [!NOTE]
> Renderers are tightly linked to Selene and the underlying hardware. Most of the time, it requires some customizations at the operating system level and access to Séléné administrative API.

## Examples
This example of code is creating a renderer for a textual LCD 16x2 screen (well, work as well with any size).

```lua
-- LCD 1602 renderer
--
-- Name of this renderer
-- if not set, takes the filename
--->> name=Toto
--
-- Failure will crash Majordome
-->> Fatal
--
-- If remove some trace
--->> quiet

-- We need administrative APIs to load an additional module : SelLCD.
Selene.exposeAdminAPI()
Selene.Use("SelLCD")

-- Mandatory directive to
-- - rethink Séléné late binding dependencies
-- - exposes Séléné API within this script
Majordome.LetsGo() -- ensure late building dependencies

--
-- Hereafter, a typical Séléné code to link with the LCD screen.
--

-- Initialize the screen handle.
-- Suitable for my BananaPI under Arch/Gentoo
local lcd = SelLCD.Init(1, 0x27, true, false)
if not lcd then
	lcd = SelLCD.Init(2, 0x27, true, false)
end
lcd:SetTiming(500, 0)

return lcd
```

> [!TIP]
> This script is also the perfect place for one shot initialization, like characters' customization. 

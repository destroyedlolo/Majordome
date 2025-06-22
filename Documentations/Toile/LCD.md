# .LCD
Create a **[renderer](Renderer.md)** for tiny LCD textual screens like famous **16x2**, **20x4**, ...).<br>
Renderers are links to physical devices to build user interfaces (UI) with **Majordome**.

> [!NOTE]
> Renderers are tightly linked to the underlying hardware. Some changes may be required at OS level, especially on the Raspberry.<br>
> In case of trouble, `i2cdetect` will check the screen is seen.

## Syntax

Renderers are basically **[Lua tasks](../Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).
### Dependancies
See [this page](Headers%20and%20Shared%20Directives.md#dependancies)

### LCD owns
#### -->> I2CBus 
The I2C bus the screen is connected to.<br>
Default is `1`.

#### -->> I2CAddress
Address of the screen on the I2C bus.<br>
Default is `0x27`

#### -->> MultiLines
Tell the screen has more than one line.

#### -->> 11px
Characters on the screen are 11 pixels high.

#### -->> Timing
LCD screens need some delay to process incoming data. The default one is very conservative but leads to a slow screen update. Playing with these parameters can improve screen refresh rate.

Example:
```
-->> Timing=500,0
```

That will set 

- **500 µSeconds** between each data sending
- **0 µSeconds** to process the data (my screen is fast enough to process data during the next arrival)

#### -->> Size
Set the LCD's size in pixels.<br>
```
-->> Size=16,2
```
**16,2** is the default value if unset.

#### -->> fatal
If set, failure of this renderer script will crash Majordome.

## at Lua side
### Lua script
The optional Lua script is expected to return a false if the renderer doesn't have to be created.

## Examples

```lua
-->> desc=Attach an LCD screen
--
---------
-- Device related
---------
--
-->> I2CBus=1
-->> I2CAddress=0x27
-->> MultiLines
-->> Timing=500,0
-->> Size=16,2
--
-->> Fatal
--
```

> [!CAUTION]
> Don't forget the screen is not yet initialized when this script is called.

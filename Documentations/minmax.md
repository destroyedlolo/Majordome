# .minmax
MinMax takes incoming data from a data source (a topic), and generates some statistics on them like minimum, maximum, average, ... values.

## Syntax

MinMax are basically **[Lua tasks](Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the MinMax. If not set, uses the filename.
`-->> name=toto`
#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

#### -->> disabled
This MinMax starts as disabled : stats change and incoming messages are ignored.

### MinMax owns
None

### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current MinMax
- **MAJORDOME_MINMAX** - minmax's name

## at Lua side
### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeMinMax**'s API :
- `getContainer()` returns the container (directory) in which this MinMax has been defined
- `getName()` returns MinMax's name
- `isEnabled()` returns a boolean reflecting if this MinMax is enabled or not
- `Enable()` to enable this MinMax
- `Disable()` to disable this MinMax 
- `Clear()` or `Reset()` to reset data statistics : a new collection is starting
- `getMin()`, `getMax()`, `getAverage()` : some statistics
- `getSum()` : sum of all incoming data since last `Reset()`
- `getSamplesNumber()` : number of samples received since last `Reset()`

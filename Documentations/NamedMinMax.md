# .namedminmax
NamedMinMax generates some statistics from several named dataset (instead of only one as [MinMax](MinMax.md) is doing).

## Syntax

NamedMinMax are basically **[Lua tasks](Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).
### Data arrival
See [this page](Headers%20and%20Shared%20Directives.md#triggering-while-providing-data)
### Dependancies
See [this page](Headers%20and%20Shared%20Directives.md#dependancies)

### NamedMinMax owns
None

## at Lua side
### Lua script
The script is expected to return the name of the data collection to store to.
Consequently, a single NamedMinMax can process several topics, as shown in the provided example.

In addition, it can return a value that will replace the received one.

Consequently :
#### expected returns
- `return false` : rejected data
- `return "my collection"` : accepted and stored in *my collection*
- `return "my collection", 21` : accepted, and forced to *21* which is stored in *my collection*

#### Probably problematic returns
- `return true` : data accepted but orphaned
- `return 2` : stored in "*2*" collection whereas the data is probably expected to be forced to **2**

### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current NamedMinMax
- **MAJORDOME_NAMEDMINMAX** - NamedMinMax's name

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeNamedMinMax**'s API :
- `getContainer()` returns the container (directory) in which this NamedMinMax has been defined
- `getName()` returns NamedMinMax's name
- `isEnabled()` returns a boolean reflecting if this NamedMinMax is enabled or not
- `Enable()` to enable this NamedMinMax
- `Disable()` to disable this NamedMinMax 
- `Clear(name)` or `Reset(name)` to reset data statistics : a new collection is starting (only impacting the one the name is provided)
- `getMin(name)`, `getMax(name)`, `getAverage(name)` : some statistics
- `getSum(name)` : sum of all incoming data since last `Reset()`
- `getSamplesNumber(name)` : number of samples received since last `Reset()`
- `FiguresNames()` : return all figures name's

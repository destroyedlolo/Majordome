# .multikeysminmax
MultiKeysMinMax generates some statistics from several multi keys dataset (instead of only one key as [MinMax](MinMax.md) is doing).

## Syntax

MultiKeysMinMax are basically **[Lua tasks](Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).
### Data arrival
See [this page](Headers%20and%20Shared%20Directives.md#triggering-while-providing-data)
### Dependancies
See [this page](Headers%20and%20Shared%20Directives.md#dependancies)

### MultiKeysMinMax owns
#### -->> NumberOfKeys=
As the name saids, the amount of keys, between **1** and **255**.<br>
As example, if you want to store several figures of a computer named "Galactica", you will have :
- `-->> NumberOfKeys=`**2**
- and keys, will looks like
```
'Galactica', 'CPULoad'
'Galactica', 'Memory'
...
```

## at Lua side
### Lua script
The script is expected to return the keys of the data collection to store to as an *array*.

In addition, it can return a value that will replace the received one.

Consequently, again with 2 keys :
#### expected returns
- `return false` : rejected data
- `return {"my machine", "CPULoad"}` : accepted and stored in *my machine/CPULoad*
- `return {"my machine", "CPULoad"}, 21` : accepted, and forced to *21* which is stored in *my machine/CPULoad*

### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current MultiKeysMinMax
- **MAJORDOME_MULTIKEYSMINMAX** - MultiKeysMinMax's name

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeMultiKeysMinMax**'s API :
- `getContainer()` returns the container (directory) in which this MultiKeysMinMax has been defined
- `getName()` returns MultiKeysMinMax's name
- `isEnabled()` returns a boolean reflecting if this MultiKeysMinMax is enabled or not
- `Enable()` to enable this MultiKeysMinMax
- `Disable()` to disable this MultiKeysMinMax 
- `Clear(name_array)` or `Reset(name_array)` to reset data statistics : a new collection is starting (only impacting the one the name is provided)
- `getMin(name_array)`, `getMax(name_array)`, `getAverage(name_array)` : some statistics
- `getSum(name_array)` : sum of all incoming data since last `Reset()`
- `getSamplesNumber(name_array)` : number of samples received since last `Reset()`
- `FiguresNames()` : return all figures name's

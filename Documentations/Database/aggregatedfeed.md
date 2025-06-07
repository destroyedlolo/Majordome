# .aggregatedfeed

Store data aggregated in a [MinMax](../minmax.md) in a database (with or without name associated).

## Syntax

.aggregatedfeed are Lua script where directives are put in its header as following :
- Each line starting with `-->>` are Majordome's directives.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives
### General directives
See [this page](../Headers%20and%20Shared%20Directives.md#general-directives).
### Triggering
Following directives determine what will trigger this script.<br>
Multiple directives may be present, including those of the same kind.
#### Launched without data
See [this page](../Headers%20and%20Shared%20Directives.md#triggering-without-data)

A Typical example of launching feeding without providing data is when the said data will be taken from another sources such [MinMax](../minmax.md).

### aggregatedfeed's owns
#### -->> Database=
Which database to use.`

#### -->> table=
The table where data will be inserted.
If not set, the same as "name" or the filename.

> [!TIP]
> For convenience, it is preferable that the table name's is only in lower-case. Otherwise, it will have
>  to be surrounded by double quotes in pgsql.

#### Sources 
> [!CAUTION]
> A source is **mandatory**. Only one source can be provided

##### -->> from MinMax=
Indicate the [MinMax](../minmax.md) source.
##### -->> from NamedMinMax=
Indicate the [NamedMinMax](../NamedMinMax.md) source.

#### -->> figure=
Which field to consider. Can be
- **Average** (or **AVG**)
- **Min**
- **Max**
- **Sum**
- **MMA** (for Min, Max, Average)

#### preprocess=
Unlike [feed](feed.md), the main Lua code can't be used to modify inserted data (but if the source is a MinMax and figure is different of MMA) in aggregatedfeed in case of. Here, you can specify a function to be called before the data is inserted in the database.

##### If figure is different of MMA

If the data source is a [MinMax](../minmax.md), the function signature is
```
function (numeric val)
```
where `val` is the original value to insert.<br>
If the data source is a [NamedMinMax](../NamedMinMax.md), the function signature is
```
function (numeric val, string name)
```
where `name` is the collection's name.<br>
In both cases, the function has to return the modified value.

As example, a preprosessing function to convert to do data conversion.
``` Lua
-- Comvert aggregated value to Wh
function toWh( val, name )
	val = val * (SavingAggregation:getEvery() / 3600)
	return val
end
```

##### If figure is MMA

If the data source is a [MinMax](../minmax.md), the function signature is
```
function (numeric min, numeric max, numeric average)
```
which are the original value to insert.<br>
If the data source is a [NamedMinMax](../NamedMinMax.md), the function signature is
```
function (numeric min, numeric max, numeric average, string name)
```
where `name` is the collection's name.<br>
In both cases, the function has to return the modified min, max, average values.

## SQL table definition
### From MinMax
```
CREATE TABLE ntest (
   sample_time TIMESTAMP WITH TIME ZONE,
   figure TEXT NOT NULL,
   value INTEGER -- Or any numeric datatype
);
```

### From NamedMinMax
```
CREATE TABLE ntest (
   sample_time TIMESTAMP WITH TIME ZONE,
   figure TEXT NOT NULL,
   value INTEGER -- Or any numeric datatype
);
```

## at Lua side

### Exposed variables

- **MAJORDOME_Myself** is automatically created and correspond to the current Feed
- **MAJORDOME_AGGREGATEDFEED** - Feed's name

### Data name and validating

The script can return :
- `true` the content is accepted (the default value)
- `false` the content is rejected
- a numeric value : force this value to be injected instead of source's content (MinMax only).

> [!TIP]  
> If comming from a [topic](topic.md), the data are stored in **MAJORDOME_PAYLOAD** variable

> [!CAUTION]
> Validation code is running synchronously : consequently, it has to be 
> * not blocking
> * as fast as possible

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeAggregatedFeed**'s API :
- `getContainer()` returns the container (directory) in which this object has been defined
- `getName()` returns object's name
- `isEnabled()` returns a boolean reflecting if this object is enabled or not
- `Enable()` to enable this object
- `Disable()` to disable this object 

Those are redundant with header declarations, but they are making life easier : all configurations are done in the header, and the code remains unchanged.

- `getTable()` returns the table's name
- `getDatabase()` returns the database as **MajordomepgSQL** object.

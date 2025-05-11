# .feed

Store incomming data in a database.

## Syntax

.feed are Lua script where directives are put in its header as following :
- Each line starting with `-->>` are Majordome's directives.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives

### General directives
See [this page](../Headers%20and%20Shared%20Directives.md#general-directives).
#### Launched with a data furnished
See [this page](../Headers%20and%20Shared%20Directives.md#triggering-while-providing-data)
#### Launched without data
See [this page](../Headers%20and%20Shared%20Directives.md#triggering-without-data)

A Typical example of launching feeding without providing data is when the said data will be taken from another sources such [MinMax](../minmax.md).

### feed's owns
#### -->> Database=
Which database to use.

#### -->> table=
The table where data will be inserted.
If not set, the same as "name" or the filename.

> [!TIP]
> For convenience, it is preferable that the table name's is only in lower-case. Otherwise, it will have
>  to be surrounded by double quotes in pgsql.

## SQL table definition

```
CREATE TABLE test (
   sample_time TIMESTAMP WITH TIME ZONE,
   value INTEGER
);
```

## at Lua side

### Exposed variables

- **MAJORDOME_Myself** is automatically created and correspond to the current Feed
- **MAJORDOME_FEED** - Feed's name

### Validating incoming data
Feed definition file can end with a Lua code : 
- if this code returns "**false**", this data submission is ignored.
- "**true**" or no value, the data is accepted
- In a number is returned, it replace the original value

> [!TIP]
> If comming from a [topic](topic.md), the data are stored in **MAJORDOME_PAYLOAD** variable

> [!CAUTION]
> Validation code is running synchronously : consequently, it has to be 
> * not blocking
> * as fast as possible

Example :
```
if tonumber(MAJORDOME_PAYLOAD) == 85 then
-- Ignore this value, (85 means underpowered 1-wire probe)
	return false;
elseif tonumber(MAJORDOME_PAYLOAD) == 50 then
-- replace 50 by 49
	return 49;
end
```
### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeFeed**'s API :
- `getContainer()` returns the container (directory) in which this Feed has been defined
- `getName()` returns Feed's name
- `isEnabled()` returns a boolean reflecting if this Feed is enabled or not
- `Enable()` to enable this Feed
- `Disable()` to disable this Feed

Those are redundant with header declarations, but they are making life easier : all configurations are done in the header, and the code remains unchanged.

- `getTable()` returns the table's name
- `getDatabase()` returns the database as **MajordomepgSQL** object.

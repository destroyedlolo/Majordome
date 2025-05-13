# .namefeed

Store incomming data in a database associated to a name.

## Syntax

.namedfeed are Lua script where directives are put in its header as following :
- Each line starting with `-->>` are Majordome's directives.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives
### General directives
See [this page](../Headers%20and%20Shared%20Directives.md#general-directives).
### Triggering
Following directives determine what will trigger this script.<br>
Multiple directives may be present, including those of the same kind.
#### Launched with a data furnished
See [this page](../Headers%20and%20Shared%20Directives.md#triggering-while-providing-data)
#### Launched without data
See [this page](../Headers%20and%20Shared%20Directives.md#triggering-without-data)

A Typical example of launching feeding without providing data is when the said data will be taken from another sources such [MinMax](../minmax.md).

### feed's owns
#### -->> Database=
Which database to use.`

#### -->> table=
The table where data will be inserted.
If not set, the same as "name" or the filename.

> [!TIP]
> For convenience, it is preferable that the table name's is only in lower-case. Otherwise, it will have
>  to be surrounded by double quotes in pgsql.


## SQL table definition

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
- **MAJORDOME_NAMEFEED** - Feed's name

### Data name and validating

The script is expected to return the name of the data to store.
Consequently, a single NamedFeed can process several topics, as shown in the provided example.

In addition, it can return a value that will replace the received one.

> [!TIP]  
> If comming from a [topic](topic.md), the data are stored in **MAJORDOME_PAYLOAD** variable

> [!CAUTION]
> Validation code is running synchronously : consequently, it has to be 
> * not blocking
> * as fast as possible

Consequently :
#### expected returns
- `return false` : rejected data
- `return "my data"` : accepted and stored as *my data*
- `return "my data", 21` : accepted, and forced to *21* and stored as *my data*

#### Probably problematic returns
- `return true` : data accepted but orphaned
- `return 2` : stored in "*2*" collection whereas the data is probably expected to be forced to **2**

### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current NamedMinMax
- **MAJORDOME_NAMEDMINMAX** - NamedMinMax's name

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomepgSQL**'s API :
- `getContainer()` returns the container (directory) in which this pgSQL has been defined
- `getName()` returns pgSQL's name
- `isEnabled()` returns a boolean reflecting if this pgSQL is enabled or not
- `Enable()` to enable this pgSQL
- `Disable()` to disable this pgSQL 

Those are redundant with header declarations, but they are making life easier : all configurations are done in the header, and the code remains unchanged.

- `getTable()` returns the table's name
- `getDatabase()` returns the database as **MajordomepgSQL** object.

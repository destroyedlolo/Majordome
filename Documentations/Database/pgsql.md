# .pgsql

[PostgreSQL](https://www.postgresql.org/) database access definition.

## Syntax

.pgsql are simple text file where :
- Each line starting with `-->>` are Majordome's commands.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the MinMax. If not set, uses the filename.
`-->> name=toto`
#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

#### -->> disabled
This MinMax starts as disabled : stats change and incoming messages are ignored.

### psql's owns
#### -->> pgConnectionstring=
Connection string as defined in the [PostgreSQL's documentation](https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-CONNSTRING).
Something like :<br>
`postgresql://[user[:password]@][[host][:port][,...]][/dbname][?name=value[&...]]`

## at Lua side

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomepgSQL**'s API :
- `getContainer()` returns the container (directory) in which this pgSQL has been defined
- `getName()` returns pgSQL's name
- `isEnabled()` returns a boolean reflecting if this pgSQL is enabled or not
- `Enable()` to enable this pgSQL
- `Disable()` to disable this pgSQL 
- `doSQL()` to execute an SQL query (see [Exec_SQL](SamplesCode/Exec_SQL) example).

> [!CAUTION]
> The result of `doSQL() ` is returned as a table to Lua side.
> Consequently, querying a large amount of data can lead to a memory shortage
> and can crash your application.

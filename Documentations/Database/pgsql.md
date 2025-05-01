# .pgsql

[PostgreSQL](https://www.postgresql.org/) database access definition.

## Syntax

.pgsql are simple text file where :
- Each line starting with `-->>` are Majordome's commands.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives

### General directives
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).

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

#### Executing SQL
- `doSQL()` to execute an SQL query (see [Exec_SQL](SamplesCode/Exec_SQL) example).

> [!CAUTION]
> The result of `doSQL() ` is returned as a table to Lua side.
> Consequently, querying a large amount of data can lead to a memory shortage
> and can crash your application.

#### Securing queries
- `EscapeLiteral()` and `EscapeString()` escape a string for use within an SQL command.
- `EscapeIdentifier()` escapes a string for use as an SQL identifier, such as a table, column, or function name.

> [!CAUTION]
> These functions need to connect to the database.

##### Example

This snippet shows how to use these function from a **Feed** or a **NamedFeed**.
```
MAJORDOME_Myself:getDatabase():EscapeString( mystring )
```

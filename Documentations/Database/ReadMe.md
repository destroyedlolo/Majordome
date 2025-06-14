Database
========

Thanks to this extension, Majordome can be used to feed easily a database with incoming data.
As of v6.1, **PostgreSQL** databases are supported.

# Objects

- **[pgsql](pgsql.md)** : Define a PostgreSQL database connection
- **[feed](feed.md)** : Insert a received figure into a database table (single figure per table)
- **[namedfeed](namedfeed.md)** : Insert received named figure in a database table (multiple figures per table)
- **[aggregatedfeed](aggregatedfeed.md)** : Store data aggregated in a *anykind*MinMax in a database.
- **[archiving](archiving.md)** : *Standard* archiving of table. Aggregate the data of a table to a second one after doing some statistics.

> [!Tip]
> Custom queries are handled by [tasks](../Task(lua).md), as shown in the [Exec_SQL](SamplesCode/Exec_SQL) code sample.

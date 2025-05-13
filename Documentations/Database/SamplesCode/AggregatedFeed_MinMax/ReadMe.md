This example shows how feed a database from **MinMax** aggregated data

# Files

![Objects in this example](Diagram.svg)

- `database.pgsql` Database access configuration **MUST BE CUSTOMISED AS PER YOUR NEEDS**
- `Periode.timer` Sample periode time
- `Input.topic` Incoming data topic (here, my electricity consumptio, **MUST BE CUSTOMISED AS PER YOUR NEEDS**)
- `Aggregator.minmax` Gather statistics about incoming data.
- `Feed.aggregatedfeed` Feed the database with generated statistics

This directory is an example of database feeding.

# Prerequisite
- You need first an up-and-running PostgreSQL database
- Create *test* user
```
createdb test
createuser test ...
```
Then login as test then create the target table :
```
CREATE TABLE test (
   sample_time TIMESTAMP WITH TIME ZONE,
   value INTEGER
);
```

# Files

![Objects in this example](Diagram.svg)

- **Test.topic** creates a listening topic `Test` on which data will be received
- **database.pgsql** declares the postgresql database. ***MUST BE CONFIGURED***
- **Test.feed** feeds incoming data into a table name `Test`

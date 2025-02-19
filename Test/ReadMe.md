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
CREATE TABLE ntest (
   sample_time TIMESTAMP WITH TIME ZONE,
   figure TEXT NOT NULL,
   value INTEGER
);
```

# Files
- **Test.topic** creates a listening topic `Test` on which data will be received
- **database.pgsql** declares the postgresql database. ***MUST BE CONFIGURED***
- **test.namedfeed** feeds incoming data into a table name `Test`

# How to test
To test it, create a config file like this :

    # URL to reach the broker
    Broker_URL=tcp://torchwood.local:1883

    # Application directory
    ApplicationDirectory=Documentations/SamplesCode/NamedMinMax_script

And publish data to `Test` topic.

The validation script handles the following data :
- **< 10** : stored as "*orphaned data*"
- **20** : converted to **21**
- **85** : rejected
- **>90** : will return send data to "2" figure.
- all other values are accepted as such in "*my data*".

All the other data are stored in *my data*

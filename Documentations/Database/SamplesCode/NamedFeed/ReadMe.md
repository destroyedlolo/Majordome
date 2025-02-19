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

# Results

```
psql (15.3, serveur 11.7)
Saisissez « help » pour l'aide.

test=> select * from ntest;
          sample_time          |    figure     | value 
-------------------------------+---------------+-------
 2025-02-19 22:38:40.452419+01 | my data       |    21
 2025-02-19 22:39:03.654682+01 | my data       |    10
 2025-02-19 22:39:16.666189+01 | orphaned data |     5
 2025-02-19 22:39:25.376924+01 | orphaned data |     9
 2025-02-19 22:39:34.659751+01 | my data       |    21
 2025-02-19 22:39:48.078592+01 | 2             |    91
 2025-02-19 22:39:52.504817+01 | 2             |    95
 2025-02-19 22:39:56.21702+01  | my data       |    45
(8 lignes)
```
```

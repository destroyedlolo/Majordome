# .archiving

Archive data from one table to another.

The goal is not only to copy data from one table to another, but archiving is doing also some aggregation to save spaces and keep only what is meaningful.

## Syntax

- Each line starting with `-->>` are Majordome's directives.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives
### General directives
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).

### Database related
#### -->> Database=
Which database to use.

#### -->> source=
Source table containing original data.

#### -->> table=
The targeted table where data will be inserted. 
If not set, the same as "name" or the file name.

> [!TIP]
> For convenience, it is preferable that the table name's is only in lower-case. Otherwise, it will have
>  to be surrounded by double quotes in pgsql.

### Selection related

#### -->> Kind

Archiving method as :

##### MinMax

**Min / Max / Average** statistics. It's the default method.
Targeting a table like

```
   Column    |           Type           | Collation | Nullable | Default 
-------------+--------------------------+-----------+----------+---------
 sample_time | timestamp with time zone |           |          | 
 minimum     | integer                  |           |          | 
 maximum     | integer                  |           |          | 
 average     | integer                  |           |          | 
```

##### Sum

Sumarize data of the time frame. It's targeting a table like

```
   Column    |           Type           | Collation | Nullable | Default 
-------------+--------------------------+-----------+----------+---------
 sample_time | timestamp with time zone |           |          | 
 value       | integer                  |           |          | 
```

##### Delta

Delta between the first and the last value. Targeting a table like

```
   Column    |           Type           | Collation | Nullable | Default 
-------------+--------------------------+-----------+----------+---------
 sample_time | timestamp with time zone |           |          | 
 increase    | integer                  |           |          | 
```

Where `increase` is the difference between the minimum and maximum of `value` for the given time frame.

#### ---> AggregateBy=

Can be any range supported by SQL's `DATE_TRUNC()` likes **hour**, **day** (default),
**week**, **month**, **year** ...

> [!Note]
> Notez-bien : check is done at database level. If an unsupported range is provided, it will create an SQL error.

Example :
```
--> AggregateBy=Month
```

#### -->> Keys=

For tables feed by [**namedfeed**](namedfeed.md), indicates the field(s) to group by.<br>
If more than one are provided, they are separated by a comma

```
-->> Keys=counter,figure
```

#### -->> UpTo=
Up to when data will be archived, following SQL syntax.
Will be executed by an SQL sentence like this one `... where sample_time::date > current_date - interval 'arg'`
Consequently, argument should be something like :
- *1 day* to ignore last 1 days (default)
- *3 months* to ignore 3 last months

### Launching

#### -->> when=
Indicates the [**Timer**](../timer.md) to wait for : 
this feed will be triggered when this timer is exhausted.
```
-->> when=15s
```

### Notification
[**Rendez-vous**](../rendezvous.md) are triggered when the archiving process is terminated.

> [!NOTE]
> Handlers attached to these rendez-vous will be called asynchronously. 

> [!CAUTION]
> if the archiving is disabled, no rendezvous will be triggered

> [!TIP]
> If you want the rendez-vous to be called whatever the result, put it in both.

#### -->> SuccessRDV
 The Trigger rendez-vous if no SQL error happened.

#### -->> FailRDV
The Trigger rendez-vous if SQL error happened.

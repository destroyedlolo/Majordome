# .purge

Purge the table's data with the given retention.

## Syntax

- Each line starting with `-->>` are Majordome's directives.
- Consequently, --->> are commented out commands (notice the 3 dashes).

## Directives
### General directives
See [this page](../Headers%20and%20Shared%20Directives.md#general-directives).

### Launching

See [this page](../Headers%20and%20Shared%20Directives.md#triggering-without-data).

### Database related
#### -->> Database=
Which database to use.

#### -->> table=
The targeted table where data will be purged. 
If not set, the same as "name" or the file name.

> [!TIP]
> For convenience, it is preferable that the table name's is only in lower-case. Otherwise, it will have
>  to be surrounded by double quotes in pgsql.

### Selection related

#### -->> Keep=
Up to when data will be purged, following SQL syntax.
Will be executed by an SQL sentence like this one `... where sample_time::date > current_date - interval 'arg'`
Consequently, argument should be something like :
- *1 day* to delete all but last 1 day (default)
- *3 months* to delete data older than 3 last months

### Notification
[**Rendez-vous**](../rendezvous.md) are triggered when the purging process is terminated.

> [!NOTE]
> Handlers attached to these rendez-vous will be called asynchronously. 

> [!CAUTION]
> if the purge is disabled, no rendezvous will be triggered

> [!TIP]
> If you want the rendez-vous to be called whatever the result, put it in both.

#### -->> SuccessRDV
 The Trigger rendez-vous if no SQL error happened.

#### -->> FailRDV
The Trigger rendez-vous if SQL error happened.

Step by step data complete data cycle tutorial
---

In this tutorial, we'll see how to handle a complete data life cycle with Majordome unique 
**No-Code**/**Low-Code** features.

📊 Data will be ingested as **time-series** data into a PostgreSQL database, aggregated as statistics,
archived, and finally purged : **a total data cycle life with low efforts**.<br>
🧾 Graphical diagrams will be generated as well to make easier documentation and understanding
of the overall process.

At user side, data will be visualized through the industries standard Grafana.

---

## Preamble

As an example, we will take data issued by a UPS where data are published through MQTT. A data set is published every 15 seconds.

```
onduleur/input.transfer.low	180
onduleur/input.transfer.high	280
onduleur/input.voltage	234.0
onduleur/battery.charge.warning	20
onduleur/battery.charge	100
onduleur/ups.realpower.nominal	480
onduleur/ups.load	35
```

> [!TIP]
> For devices not able to publish to MQTT,
> my [Marcel](https://github.com/destroyedlolo/Marcel) daemon can be used as a gateway as long as
> it is supported by NUT.

> [!NOTE]
> Installation and configuration of a **MQTT broker** (Mosquitto suggested), **PostgreSQL server**,
> and **Majordome** itself are definitively out of the scope of this tutorial.

---

## ⚙️ Project setup

1. Creates a directory dedicated to this project and copy inside `00_Majordome` from the
examples provided in this repository. It will implemente Majordome's own log cycling.

![structures](Resources/00_Majordome.png)

2. `10_Configuration` will contain a timer definition used to cadence the statistics aggregation.

![structures](Resources/10-Configuration.png)

`5Minutes.timer`
```
-->> desc=Timer to launch an action every 5 minutes
-->> every=300
```

And `database.pgsql` is declare database acces :
```
-- Declare the database to connect to
-- Connection string : see PostgreSQL documentation
-- Something like :
--	postgresql://[user[:password]@][[host][:port][,...]][/dbname][?name=value[&...]]
-->> pgConnectionstring=postgresql://domestik2@localhost/www
```

---

## 📡 Data gathering - 50_UPS

1. 📥 **incoming topic** : `UPS.topic`

Defines were data are comming from.

```
-->> desc=Root of the UPS topic tree
-->> group=UPS
--
-->> topic=onduleur/#
--
-->> quiet
--->> disabled
```

2. 🗃️ **In-Memory Aggregator** : `UPS.namedminmax`

From a business point of view, having such a data rate is useful for the real-time dashboard (*also powered by Majordome 😉*), it's not needed for medium or long term storage in the database : an aggregation on 5 minutes basis is enough.

This collection will have only one key : the name of the figure to store. As a consequence, a **[namedminmax](../../Documentations/NamedMinMax.md)** is used.

```lua
-->> desc=Collect and aggregate data
-->> group=UPS
--
-->> listen=UPS
--
-->> quiet

local figure = MAJORDOME_TOPIC:match("onduleur/(.+)")

if MAJORDOME_VERBOSE then
	SelLog.Log("I", "Got figure ".. figure)
end

return {figure}
```

The `-->> listen=UPS` indicates which topic to listen too, here the defined previously in `UPS.topic`.

The trailing Lua code extracts the figure name from the incoming topic, which is expected as the code's return value.

3. 🐘 **Database storage**


---

## 📦 Archiving

Older data don't need to be so precise : they are archived to save disk space.

---

## 🧹 Purging

Only the archived data are kept : the recent-data table is purged for obsolete data.

---

## 📊 Reporting

---

## 🚀 Running

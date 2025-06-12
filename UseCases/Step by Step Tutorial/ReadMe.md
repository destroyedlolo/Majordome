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

As an example, we will take data issued by a UPS where data are published through MQTT.

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

Create a directory dedicated to this project and copy inside `00_Majordome` from the
examples provided in this repository. It will implemente Majordome's own log cycling.

---

## 📡 Data gathering

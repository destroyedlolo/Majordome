This example demonstrates how **MinMax**'s validation script can reject or modify incoming data.

---

* `Test.topic` : data source
* `TestMinMax.namedminmax` : validation script inside namedminmax object definition
* `30s.timer` : Timeframe to generate statistics on
* `Collector.lua` : display MinMax's statistics

---
To test it, create a config file like this :

    # URL to reach the broker
    Broker_URL=tcp://torchwood.local:1883

    # Application directory
    ApplicationDirectory=Documentations/SamplesCode/NamedMinMax_script

And publish data to `Test` topic.

---

The validation script handles the following data :
- **< 10** : stored as orphaned
- **20** : converted to **21**
- **85** : rejected
- **>90** : will return send data to "2" collection
- all other values are accepted as such.

All the other data are stored in *mycollection*

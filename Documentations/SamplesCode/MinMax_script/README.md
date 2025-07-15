This example demonstrates how **MinMax**'s validation script can reject or modify incoming data.

---
![Objects in this example](Diagram.svg)

* `Test.topic` : data source
* `TestMinMax.minmax` : validation script inside minmax object definition
* `30s.timer` : Timeframe to generate statistics on
* `Collector.lua` : display MinMax's statistics

---
To test it, create a config file like this :

    # URL to reach the broker
    Broker_URL=tcp://torchwood.local:1883

    # Application directory
    ApplicationDirectory=Documentations/SamplesCode/MinMax_script

And publish data to `Test` topic.

---

The validation script handles the following data :
- **20** : converted to **21**
- **85** : rejected
- all other values are accepted as such.

Demonstrates the usage of **MinMax** object : It will gather real-time consumed electricity and, every 10 seconds, will display some statistics about it :
```
Number of samples :    7.0
Min value :    530.0
Max value :    2640.0
Max Average :    1640.0
```

# Files

![Objects in this example](Diagram.svg)


* `ConsPower.topic` : data source
* `TestMinMax.minmax` : statistic generator for this example
* `10s.timer` : Timeframe to generate statistics on
* `Collector.lua` : display MinMax's statistics

# How to test it

To test it, create a config file like this :

	# URL to reach the broker
	Broker_URL=tcp://torchwood.local:1883

	# Application directory
	ApplicationDirectory=Documentations/SamplesCode/Tracker_HowMany

And obviously you have to find out your own data source.

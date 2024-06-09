Send an alarm when my pool pump filter is full.

When the filter of my pool is full, its pump's consumption drops below 600W. This scenario sends an alert when the average of 30 consumption measures drops below 600W.

- `Init.lua` - load Séléné's **SelAverageCollection** module.
- `PumpConsumption.topic` - MQTT top to receive pump's consumption.
- `Alert.topic` - Topic to send alert to
- `CheckConsumption.lua` - processing



# About this use case

This use case shows the "manual way" to do some automation using Majordome.
- Receive messages
- Take decisions using **Lua** scripting,
- Use Séléné's features (here **SelAverageCollection**) to store and process data (notez-bien : the same thing can be achieved using Majordome's MinMax in an easier way)
- ...

> [!TIP]
> Such *low-level* Lua scripting can be useful to make automation too complex
> for Majordome's own objects. But using diretly Séléné's Collection directly
> exhibits generally bad application design.

# Goal

Send an alarm when my pool pump filter is full.

When the filter of my pool is full, its pump's consumption drops below 600W. This scenario sends an alert when the average of 30 consumption measures drops below 600W.



# Files

![Objects in this example](Diagram.svg)

- `Init.lua` - load Séléné's **SelAverageCollection** module.
- `PumpConsumption.topic` - MQTT top to receive pump's consumption.
- `Alert.topic` - Topic to send alert to
- `CheckConsumption.lua` - processing



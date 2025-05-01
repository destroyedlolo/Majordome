# Headers and Shared directives

This page describes :
- how directives are declared in object's header
- which directives are shared among objects

## Definition

- an **event**  is an object that handle a particular condition to triggers its handlers. Example of event can be 
  - MQTT message arriving (see [topics](topic.md)). In such case, a data is provided to handlers, the content of the message
  - when its time to do something (see [timers](timer.md)). In such case, obviously, no data is provided.

- a **Handler** is an object that will be triggered when an event happens. It can be a [Lua Task](Task(lua).md), database [feeding](Database/feed.md), ...

> [!TIP]
> If the handler can execute Lua code, environment variables help to determine what triggered.
 
## Headers

All Majordome's configuration files starts with a header that contains 
**comments** and **Majordome directives**, as follows :

```Lua=
01 --My configuration file
02 --
03 -- A comment line
04 --
05 -->> Enabled Majordome directive
06 --->> Disabled Majordome directive
07 --
08 -- here after a blank line
09
10 -- Now, optional Lua code
11 -->> not anymore a Majordome directive, 
12 -- we're not anymore in the header
```

Heach header's line starts with a double dash `--`.

The first line without these double dash (as line **9**) means that we're not anymore in the header, potentially in a Lua code.

Each line starting with `-->>` (line **5**) are Majordome's directives.<br>
If you want to comment out a directive, use `--->>`, notice the 3 dashes (line **6**).

## General directives
The following directives are applicable to all Majordome's objects.

### -->> name=
Unique name to identify the object. If not set, uses the filename.
```
-->> name=Toto
```
### -->> quiet
Removes some trace.

### -->> disabled
This object is disabled : 
- if a script is not be executed
- if a MQTTTopic, the incoming messages will be ignored
- if a timer, it will not trigger children

### -->> desc=
Add a description to the object.<br>
This field is useful to generate a small description in automatic generated diagrams (using **D2**, see [Majordome **-2**](Readme.md) option).

## Triggering while providing data

Those directives handle handlers

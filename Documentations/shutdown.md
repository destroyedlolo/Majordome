# .shutdown
**Shutdowns** are tasks that are started when Majordome exits.

If most of Majordome own resources are cleaned up/released at exit, typical
**.shutdown** usage is to clean external ones. As an example, an LCD screen as
Toile's renderer :  when Majordome is finished, the screen has to be turned off.
It can be achieved by a **shutdown** task.

## Syntax

**Shutdowns** are basically **[Lua tasks](Task(lua).md)** and are following the same syntax and share most of it's Lua API.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives
### General directives
#### -->> name=
Unique name to identify the topic. If not set, uses the filename.
```
-->> name=Toto
```

#### --> quiet
Removes some trace.

#### -->> disabled
This script won't run.

### Dependancies
Those directives expose them at Lua level without having to manually `find()` them.

#### -->> need_topic=
Creates an object if a value has been received and its value hasn't expired. Otherwise, the object remains unset.

#### -->> require_topic=
Prevents the script to be launched if corresponding value is not valid.<br>

#### -->> need_rendezvous=, -->> need_tracker=, -->> need_timer=, -->> need_task=, -->> need_minmax
Create corresponding object.

#### -->> need_renderer
Only **Toile** plug-in has been compiled

## at Lua side

### Exposed objects
Statistics sequencing and retrieving are done through the **MajordomeNamedMinMax**'s API :
- `getContainer()` returns the container (directory) in which this Shutdown has been defined
- `getName()` returns Shutdown's name
- `isEnabled()` returns a boolean reflecting if this Shutdown is enabled or not
- `Enable()` to enable this Shutdown
- `Disable()` to disable this Shutdown 

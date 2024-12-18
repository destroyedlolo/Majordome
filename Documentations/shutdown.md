# .shutdown
**Shutdowns** are tasks that are started when Majordome exits.

If most of Majordome own resources are cleaned up/released at exit, typical
**.shutdown** usage is to clean external ones. As an example, an LCD screen as
Toile's renderer :  when Majordome is finished, the screen has to be turned off.
It can be achieved by a **shutdown** task.

## Directives
In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` (2 dashes) are Majordome's directives.
If you want to comment out a directive, use `--->>` (3 dashes)
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

### Importing objects
Those directives expose them at Lua level without having to manually `find()` them.

#### -->> need_topic=
Creates an object if a value has been received and its value hasn't expired. Otherwise, the object remains unset.

#### -->> require_topic=
Prevents the script to be launched if corresponding value is not valid.<br>

#### -->> need_rendezvous=, -->> need_tracker=, -->> need_timer=, -->> need_task=, -->> need_minmax
Create corresponding object.

#### -->> need_renderer
Only **Toile** plug-in has been compiled

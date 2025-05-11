A modern lightweight, event driven **Automation and data gathering System**.

![icon](https://github.com/destroyedlolo/HomeDashboard/blob/master/SeleniteDFB/Images/Majordome.png)
[![Last Commit](https://img.shields.io/github/last-commit/destroyedlolo/Majordome?logo=github&label=latest%20commit&style=flat-square)](https://github.com/destroyedlolo/Majordome/releases/latest)
[![Latest Release](https://img.shields.io/github/v/release/destroyedlolo/Majordome?logo=github&label=latest%20release&style=flat-square)](https://github.com/destroyedlolo/Majordome/releases/latest)<br>
(no commit for long time doesn't mean this project is stalling but reached a stable state and I switched to others activities :relaxed:)

----

Build your application as a galaxy of **small and independent short-living services**, in a no-code, low-code way.<br>
Those services will be launched when an event happens and/or conditions are met (*like what AWS Lambda does*).
Easy to learn **Lua code** can be used for complex decision-making.

Written in C++ and being massively a threaded engine, **Majordome** is very resource conservative and has a low system footprint.

As of **V6**, you can also feed industries standard **PostgreSQL** databases with incoming data in a *LowCode* (or even *NoCode*) way. Take a look on [comprehensive provided example](Documentations/Database/SamplesCode).<br>
As of **v6.04** onward, Majordome can generate automatically your automation documentation as [D2 diagrams](https://d2lang.com/).

Following the [KISS principle](https://en.wikipedia.org/wiki/KISS_principle), **Majordome** depends mainly on **MQTT messaging** and **timers** to trigger tasks, delegating real world communication to other dedicated components (like [Marcel](https://github.com/destroyedlolo/Marcel) in my case).

---

Typical use-cases :
---
- Smart home automation [^1]
- SCADA automation
- data logger, from data gathering, to aggregation, database storage and fault detection. Data cycle life (archiving, purging) is also easily managed.
- your imagination is the limit ...

----
> [!IMPORTANT]
> ### Notes about branches
> **Master** branch is the stable, well tested and industries grade version.<br>
> **V6** branch is the "*ahead*" version, under testing and subject to heavy development.<br>
> **Toile** is a special branch dedicated to GUI building. It is generally aligned with a stable **v6** situation and also subject to heavy development for the GUI part. As of the time of writing, only LCD plugin is stable enough for a production grade.
---

[The documentations](/Documentations/) explains how to configure automation within Majordome

---

Any contributions are greatly appreciated.

[^1]: **Majordome**, with the help of **Marcel** for the "*real world*" part, drive my smart house for 7 years without any glitches. See `MaMaison` branch for this complete and real life implementation.

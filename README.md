A lightweight, event driven **Automation System**.

![icon](https://github.com/destroyedlolo/HomeDashboard/blob/master/SeleniteDFB/Images/Majordome.png)
[![Last Commit](https://img.shields.io/github/last-commit/destroyedlolo/Majordome?logo=github&label=latest%20commit&style=flat-square)](https://github.com/destroyedlolo/Majordome/releases/latest)
[![Latest Release](https://img.shields.io/github/v/release/destroyedlolo/Majordome?logo=github&label=latest%20release&style=flat-square)](https://github.com/destroyedlolo/Majordome/releases/latest)<br>
(no commit for long time doesn't mean this project is stalling but reached a stable state and I switched to others activities :relaxed:)

----
> [!IMPORTANT]
> **Master** branch is the stable, well tested and industries grade version.<br>
> **Toile** and **V6** branches are currently on heavy development. It's not yet fully tested and stable (i.e. subject to changes).

----


Within **Majordome**, your application is built around a galaxy of small tasks orchestrated by efficient but resource conservative mechanisms.<br>
You organize your code into independent and **stateless Lua functions** that are called when an event happened and/or conditions are met (*like what AWS Lambda does*). Tasks are executed only if necessary in the massively threaded Majordome engine, resulting in an as reduced as possible system footprint.

As of **V6**, you can also feed industries standard **PostgreSQL** databases with incoming data in a *LowCode* (or even *NoCode*) way. Take a look on [comprehensive provided example](Documentations/Database/SamplesCode).

Following the [KISS principle](https://en.wikipedia.org/wiki/KISS_principle), **Majordome** depends mainly on **MQTT messaging** and **timers** to trigger tasks, delegating real world communication to other dedicated components (like [Marcel](https://github.com/destroyedlolo/Marcel) in my case).

---

Typical use-cases :
---
- Smart home automation [^1]
- SCADA automation
- centralizing data gathering and processing, fault detection
- your imagination is the limit ...

---

[Documentations](/Documentations/) explains how to configure automation within Majordome

---

Any contributions are greatly appreciated.

[^1]: **Majordome**, with the help of **Marcel** for the "*real world*" part, drive my smart house for 7 years without any glitches. See `MaMaison` branch for this complete and real life implementation.

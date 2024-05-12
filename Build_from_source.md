General installation procedure from source
==========================================

This file describes the *general* way to install Majordome from source.

Dependencies
------------

  -	**PAHOc** can be found on [its website](https://eclipse.org/paho/clients/c/) to manage MQTT managing.
  - **Séléné** available from [my github](https://github.com/destroyedlolo/Selene/)

My systems are mostly under **Linux/Gentoo**, but tested also on **Armbian** and **Arch**. 

Installation
------------

  1. install dependencies and C++ compilation suite if needed.
  1. extract Majordome sources "somewhere" (a.k.a in a temporary directory)
  1. install [LFMakeMaker](https://github.com/destroyedlolo/LFMakeMaker)
  1. customise `remake.sh` as per your needs.
  1. execute `remake.sh` to update Makefiles.
  1. `make`
  1. copy **Majordome** binary somewhere in your path (`/usr/local/sbin` is suggested).


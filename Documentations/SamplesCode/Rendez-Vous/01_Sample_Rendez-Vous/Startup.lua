-- Script launched after loading
--
-->> RunAtStartup

-- Find out rendez-vous
local rdv =  MajordomeRendezVous.find("example")

-- check if it's found
assert(rdv, "Can't find rendez-vous")
SelLog.Log("Rendez-vous found, lets launch it")

-- Launch it
rdv:Launch()

-- This is an example of "once" script.
--
-- This script is triggered every 15s, but, thanks to **Once** directive,
-- it can't be relaunched until the 20s waiting time is over. 
--
-->> when=15s
-- If set, only one instance is allowed to run
-->> once

SelLog.Log('I', "Waiting for 20s ...");

-- it is very very not recommended to use Selene.Sleep() (or any SelScripting methods) in Majordome tasks.
-- Here, it's only a convenient way for this TEST !
-- SelScripting has been loaded by "Init.lua"
Selene.Sleep(20);

SelLog.Log('I', "20s passed : leaving");


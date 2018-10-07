-- This is an example of "once" script
--
-->> when=15s
-- If set, only one instance is allowed to run
-->> once

SelLog.log('I', "Waiting for 20s ...");
Selene.Sleep(20);
SelLog.log('I', "20s passed : leaving");


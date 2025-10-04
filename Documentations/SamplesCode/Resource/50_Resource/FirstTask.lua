-->> desc=Task having long run
-->> group=Tasks related
-->> resourceWait=once
-->> when=now

SelLog.Log("1", "1st task starting")
os.execute("sleep 3")
SelLog.Log("1", "1st task finished")

-->> desc=Task having long tun
-->> resourceWait=once
-->> when=now

SelLog.Log("2", "2nd task starting")
os.execute("sleep 3")
SelLog.Log("2", "2nd task finished")

-->> desc=Task having long tun
-->> resourceFail=once
-->> when=now

SelLog.Log("0", "Failling task starting")
os.execute("sleep 3")
SelLog.Log("0", "Failling task finished")

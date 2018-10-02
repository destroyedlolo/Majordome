-- RAZ du chien de garde lorsque le répéteur a envoyé une information
-->> listen=InfoRepWiFi
-->> once

local CdG = MajordomeTimer.find("CdGRepWiFi")
assert( CdG, 'Timer "CdGRepWiFi" pas trouvé' )

CdG:Reset()

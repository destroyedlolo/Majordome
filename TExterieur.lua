-- This script handles stuffs related to "Temperature Exterieur"

function hTExt()
	print( SelShared.get( TExterieur ) )
end

-- Add related topics
TableMerge( Topics, {
	{ topic = TExterieur, trigger=hTExt, trigger_once=true },
} )

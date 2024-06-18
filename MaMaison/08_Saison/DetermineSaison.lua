-- Détermination de la saison en fonction des températures.
-->> when=22h
--
-->> need_topic=Saison
-->> need_topic=SaisonHier
--
-->> require_topic=TExterieur
-->> require_topic=TSalon

SelLog.Log('I',"Détermination de la saison")

-- Log les info ayant permis de déterminer la saison
SelLog.Log('M',"T°Ext = " .. TExterieur:getVal() .. " TSalon = " .. TSalon:getVal())

-- Publication des saisons
SaisonHier:Publish( Saison:getVal() )
if TExterieur:getVal() < 5 then
	Saison:Publish(  'Hiver' )
elseif TSalon:getVal() > 21.5 and TExterieur:getVal() > 15 then
	Saison:Publish( 'Ete' );
else
	Saison:Publish( 'Intersaison' );
end

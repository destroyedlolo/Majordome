# Consignes Lever / Coucher

Ces consignes s'appliquent aux chambres Parents et Enfants sauf celle d'amis qui a ses propres règles.

## Règles

Les consignes sont évaluées sans tenir compte du **mode**. Ce dernier sera pris en compte pour chacune des chambres.

### Lever

Propre à chacune des chambres. Ici, seuls les topics sont définis.

### Coucher

Si le soleil se couche avant la consigne :
	Timer **ConsigneCoucher** à l'heure de coucher du soleil
	Timer **ConsigneCoucherMy** désactivé.
Sinon
	Timer **ConsigneCoucherMy** 5 minutes avant la consigne.
	Timer **ConsigneCoucher** à l'heure de la consigne. 

## Topics

Format hh.mm

Nom | Topic | Commentaire
--- | ----- | -----
ConsigneLever | Majordome/HLever | Heure de lever pour les parents
ConsigneLeverOceane | Majordome/HLever/Oceane | Heure de lever pour Océane
ConsigneLeverJoris | Majordome/HLever/Joris | Heure de lever pour Joris
ConsigneCoucher | Majordome/HCoucher | Heure de coucher

Les configs par défaut sont publiées par **PublicationConsignesParDefaut.lua**

Note : La fermeture des volets se fera au plus tot de la consigne ou du coucher du soleil

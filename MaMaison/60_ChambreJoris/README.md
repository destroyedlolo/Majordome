# Automatismes pour la chambre d'Joris
---

## Ouverture

Si le mode est "*Vacances*" alors les volets restent fermés et la surveillance de température commence a 12h30.
En mode *Absent*, ils s'ouvrent avec la consigne mais il n'y a pas de *My* avant. Surveillance par défaut
~~ Sinon (donc mode *Travail*) ouverture *My* 5 minutes avant la consigne, ouverture a l'heure de la consigne. ~~ (Joris n'en veut pas)

## Fermeture

Fermeture à **ConsigneCoucher**.

Notes :
  - La consigne peut être modifiée si le soleil se couche avant celle fournie
  - pas besoin de la fermeture *My* 5 minutes avant la consigne

## Consignes

Les consignes par défaut sont publiée par **20_ConsignesLeverCoucher/PublicationConsignesParDefaut**

## Surveillance de la température

**NOTE :** La sonde de température de la chambre de Joris étant actuellement HS, la survrillance se fait sur la température de la chambre d'Océane.

La surveillance débute avec les consignes "**DebutSurveillance**" ou "**DebutSurveillanceVacances**" en fonction du mode. Le mode n'a pas d'incidence.
Elle se termine avec "**FinSurveillance**"

En mode "*Absent*", les volets ne sont pas ouvert à la fin de la surveillance

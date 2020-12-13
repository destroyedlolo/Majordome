# Automatismes pour la chambre des parents
---

## Mode "absent"

En mode *Absent*, les volets ne se ferment ni s'ouvrent.
Il n'y a pas de surveillance de température

## Ouverture

Si le mode est "*Vacances*" alors les volets restent fermés et la surveillance de température commence a 12h30.
Sinon (donc mode *Travail*) ouverture *My* 5 minutes avant la consigne, ouverture a l'heure de la consigne.

## Fermeture

Fermeture à **ConsigneCoucher**.

Notes :
  - La consigne peut être modifiée si le soleil se couche avant celle fournie
  - pas besoin de la fermeture *My* pour les parents

## Consignes

Les consignes par défaut sont publiée par **20_ConsignesLeverCoucher/PublicationConsignesParDefaut**

## Surveillance de la température

La surveillance débute avec les consignes "**DebutSurveillance**" ou "**DebutSurveillanceVacances**" en fonction du mode. Le mode n'a pas d'incidence.
Elle se termine avec "**FinSurveillance**"

En mode "*Absent*", les volets ne sont pas ouvert à la fin de la surveillance

### variables partagées
- **ChParentsAbsent** - Contient la valeur précédente du mode de la chambre des parents. Pour éviter d'envoyer plusieurs fois l'ordre de fermeture du volet si on reste en mode *Absent*

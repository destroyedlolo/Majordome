This directory contains my own smart home automation. Use it as example of real implementation.
Notez-Bien : comments are in French.

----

Ce répertoire contient les automatismes de ma propre domotique. Prenez le comme un exemple un utilisation réelle.

## Objets utilisés

Nom (*shared object* et *MQTTTopic*) | Topic | Commentaire
----|----|----
Saison | Majordome/Saison | 
SaisonHier | Majordome/Saison/Hier | Saison de la veille
ModeForce... | Majordome/Mode/Force# | Consigne de mode provenant de l'*IHM* de la domotique
ModeAujourdhui | Majordome/Mode/AujourdHui | Mode de fonctionnement pour aujourd'hui calculé par **Marcel**
ModeDemain | Majordome/Mode/Demain | Mode de fonctionnement pour demain calculé par **Marcel**
Mode | Majordome/Mode | Mode **réel** calculé depuis *ModeAujourdhui* et de *ModeForce*


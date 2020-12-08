This directory contains my own smart home automation. Use it as example of real implementation.
Notez-Bien : comments are in French.

----

Ce répertoire contient les automatismes de ma propre domotique. Prenez le comme un exemple un utilisation réelle.

## Objets utilisés les plus importants

Nom (*shared object* et *MQTTTopic*) | Topic | Commentaire
----|----|----
Saison | Majordome/Saison | 
SaisonHier | Majordome/Saison/Hier | Saison de la veille
ModeAujourdhui | Majordome/Mode/AujourdHui | Mode de fonctionnement pour aujourd'hui calculé par **Marcel**
ModeDemain | Majordome/Mode/Demain | Mode de fonctionnement pour demain calculé par **Marcel**

### Modes forcés
Modes en provenance de l'*IHM* de la domotique.
Les modes les plus *profonds* ont priorité sur les précédents.

Nom (*shared object* et *MQTTTopic*) | Topic | Commentaire
----|----|----
ModeForce | Majordome/Mode/Force | Mode générale
ModeForceChAmis | Majordome/Mode/Force/ChAmis | Mode de la chambre d'amis

### Modes actifs

Nom (*shared object* et *MQTTTopic*) | Topic | Commentaire
----|----|----
Mode | Majordome/Mode | Mode **réel** calculé depuis *ModeAujourdhui* et de *ModeForce*
ModeChAmis | Majordome/Mode/ChAmis | Mode **réel** pour la chambre d'amis

## Traces

Ces topics sont utilisés à fin de monitoring. Ils permettent d'exposer les états des automatismes de **majordome**.

Topic | Valeurs | Commentaire
---|---|---
Majordome/Traces/SuiviCoucherSoleil | `D;heure_debut` / `E;heure_debut` / `F:heure_fin;heure_debut' | Début à quelle heure / EnCour depuis quelle heure / F a quelle heure

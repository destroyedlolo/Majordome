Mode :
------
 - **Manuel** : aucune automatisation
 - **Travail** : personne en journée, levé automatique
 - **Vacances** : présence toute la journée, levé manuel
 - **Absent** : nous ne sommes pas là, simulation de présence

Saison :
--------
déterminé à 5h du matin
 - **Hiver** : T°Extérieur < 2° (si dans la journée > 20°, on passe en Intersaison)
 - **Eté** : T°Salon > 21.5° 	
 - **Intersaison** : autre

----------
Topics
======
Liste des topics de commandes :

 - **Majordome/Mode/AujourdHui** : mode automatique déterminé par le calendrier
 - **Majordome/Mode/Demain** : mode automatique pour demain déterminé par le calendrier
 - **Majordome/Mode/Force** : mode manuel forcé
 - **Majordome/Mode** : mode actif, calculé à partir des précédents
 - **Majordome/Saison** : saison déterminée par la température à 7h

----------
Scenarii
========
----------
Chambres / bureau
==========
	Ouverture à l'heure du levé - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au couché du soleil

----------
Salon
=====
Saison "Ete"
----
	Pas d'ouverture le matin (les fenêtres sont peut-être ouvertes)
	Fermeture 'My' au couché du soleil

Saison "Intersaison"
----
	Ouverture à l'heure du levé - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au couché du soleil

*Voir a partir de quel heure on re-ouvre les volets en été.*

Saison "Hiver"
---------
	Ouverture à 7h45 si soleil levé
	Fermeture au coucher du soleil -1h

----------
Cuisine
-------
	Ouverture à l'heure du levé - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au couché du soleil

----------
Porte du chat
-------------
	Si saison = hiver
		« My » à 21h
	Sinon
		« My » a 1h00 si ModeDemain = « Travail »
		« My » à 2h00 si ModeDemain = « Vacances »

---------
> Written with [StackEdit](https://stackedit.io/).

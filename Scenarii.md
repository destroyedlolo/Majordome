Mode :
------
 - **Manuel** : aucune automatisation
 - **Travail** : personne en journée, lever automatique
 - **Vacances** : présence toute la journée, lever manuel
 - **Absent** : nous ne sommes pas là, simulation de présence

Et uniquement pour les modes forcés :
 - **Auto** : Pas de forçage, mode *automatique*

 - **Majordome/Mode/AujourdHui** : mode automatique déterminé par le calendrier
 - **Majordome/Mode/Demain** : mode automatique pour demain déterminé par le calendrier
 - **Majordome/Mode/Force** : mode manuel forcé (global)
 - **Majordome/Mode/Force/Parents** : mode manuel forcé pour les chambres des parents
 - **Majordome/Mode/Force/Enfants** : mode manuel forcé pour les chambres des enfants
 - **Majordome/Mode/Force/Enfants/Oceane** : mode manuel forcé pour Océane
 - **Majordome/Mode/Force/Enfants/Joris** : mode manuel forcé pour Joris
 - **Majordome/Mode** : mode actif, calculé à partir des précédents (global maison)
 - **Majordome/Mode/Parents** : mode actif, pour les parents
 - **Majordome/Mode/Oceane** : mode actif, pour les parents
 - **Majordome/Mode/Joris** : mode actif, pour les parents

Majordome regarde le mode suivant uniquement si courent est Auto. Ce qui donne
	Majordome/Mode/Oceane :
		- Majordome/Mode/Force
			- Majordome/Mode/Force/Enfants
				- Majordome/Mode/Force/Enfants/Oceane


Saison :
--------
déterminé à 22h
 - **Hiver** : T°Extérieur < 2° (si dans la journée > 20°, on passe en Intersaison)
 - **Eté** : T°Salon > 21.5° & T°Extérieur > 15
 - **Intersaison** : autre

----------
Topics
======
Liste des topics de commandes :

 - **Majordome/Saison** : saison déterminée par la température à 7h
 - **Majordome/HLever** : Heure de lever général
 - **Majordome/HLever/Oceane** : Heure de lever d'Océane
 - **Majordome/HLever/Joris** : Heure de lever de Joris
 - **Majordome/HCoucher** : Heure de coucher des enfants
 
----------
Scenarii
========

	L'heure du couché du soleil est déterminé comme suit :
	- A partir de l'heure déterminé par le webservice météo (topic 'Meteo/?/suset'), on surveille s'il y a une production photovoltaique ('TeleInfo/Production/values/PAPP')
	- si oui, on attend 2 minutes et on refait le tests
	- si non, les volets sont a baisser

----------
Chambres / bureau
==========
	Ouverture à l'heure du lever - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au coucher du soleil

----------
Salon
=====
Saison "Ete"
----
	Pas d'ouverture le matin (les fenêtres sont peut-être ouvertes)
	Fermeture 'My' au couché du soleil

Saison "Intersaison"
----
	Ouverture à l'heure du lever - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au couché du soleil

*Voir a partir de quel heure on re-ouvre les volets en été.*

Saison "Hiver"
---------
	Ouverture à 7h45 si soleil lever
	Fermeture au coucher du soleil -1h

----------
Cuisine
-------
	Ouverture à l'heure du lever - 15m sauf si mode = Vacances où l'ouverture est à 8h15
	Fermeture au couché du soleil

----------
Porte du chat
-------------
	Si saison = hiver
		« My » à 21h
	Sinon
		« My » a 1h00 si ModeDemain = « Travail »
		« My » à 2h00 si ModeDemain = « Vacances »
		Rien si ModeDemain = « Absent »

---------
> Written with [StackEdit](https://stackedit.io/).

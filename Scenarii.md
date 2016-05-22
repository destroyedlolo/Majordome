Mode :
------
 - **Manuel** : aucune automatisation, par pièce
 - **Travail** : personne en journée
 - **Vacances** : présence toute la journée

Saison :
--------
déterminé par la température extérieur à 7h du matin
 - **Hiver** : < 5° (si dans la journée > 20°, on passe en Intersaison)
 - **Eté** : > 10° 	
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
	Saison hiver :
		On laisse fermé
		Si mode « Travail » et T° Ext > 10°C et si production photovoltaïque (le soleil brille),
			ouverture des volets
		Si la production s'arrête : fermeture des volets (peut être ré-ouvert si la condition précédente est remplie)

	Saison été ou Intersaison
		Si mode « Travail »
			Semi-ouverture (position « My » des volets Somfy) 10 minutes avant l'heure du levé, histoire d'avoir un réveil tranquille
			Ouverture à l'heure du levé (« DEBOUT LA DEDANS ! »)
		sinon mode « Vacances »
			On ne fait rien (on se lève quand on veut)

		Ensuite à partir de 9h (mode « Travail »), 10h30 mode « Vacances »
			Si été
				position « My »
			Sinon (Intersaison)
				Si production électrique et si T°Chambre > 18°C
					Position « My » pour garder de la fraicheur
				Sinon (pas de soleil, ou température trop basse)
					on ouvre

----------
Salon
=====
Saison "Ete"
----

	A partir du couché du soleil -1h
		my si T°Ext < T°Int
		Sinon Fermé
	Si Travail
		A 7h15 My
		Ouverture à 7h45
	sinon
		My a 8h
	Jusqu'au coucher du soleil
		My si T°Int > 22 si T°Ext > T°int 

*Voir a partir de quel heure on re-ouvre les volets en été.*

Saison "Intersaison"
--------------------
	si Travail
		Ouverture à 7h45
	sinon
		Ouverture 8h15
	Fermeture au coucher du soleil

Saison "Hiver"
---------
	Ouverture à 7h45 si soleil levé
	Fermeture au coucher du soleil -1h


----------
Cuisine
-------
	Si Saison = « Hiver » à partir de l'heure de levé du soleil (météo)
		on boucle sur
			Si le soleil brille (photovoltaïque) et si la T exterieur > 10°C,
				ouverture des volet.
			Sinon
				fermeture
			On arrête dès que la production photovoltaïque s'arrete ou à l'heure de couché du soleil (météo)
		Fermeture des volets

	Si autre saison
		Si production photo voltaïque et si T°Cuisine > 21
			Volet en position « My »

		A 16h
			Si pas de production photovolatïque et si T°Cuisine < 24°
				On ouvre les volets
			Sinon,
				on attend la fin de la production photovolatique puis on ouvre les volets

----------
Porte du chat
-------------
	Si saison = hiver
		« My » à 21h
	Sinon
		« My » a 1h00 si Mode = « Travail »
		« My » à 2h00 si Mode = « Vacances »

---------
> Written with [StackEdit](https://stackedit.io/).

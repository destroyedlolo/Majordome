Mode :
------
 - **Manuel** : aucune automatisation, par pièce
 - **Travail** : personne en journée
 - **Vacances** : présence toute la journée

Saison :
--------
déterminé par la température extérieur à 7h du matin
 - **Hivers** : < 5° (si dans la journée > 20°, on passe en Intersaison)
 - **Eté** : > 10° 	
 - **Intersaison** : autre

----------
Scenarii
========
----------
Chambres / bureau
-----------------
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
-----
	Si Saison = « Hiver » à partir de l'heure de levé du soleil (météo)
		on boucle sur
			Si le soleil brille (photovoltaïque) et si la T exterieur > 10°C,
				ouverture des volet.
			Sinon
				fermeture
			On arrête dès que la production photovoltaïque s'arrete ou à l'heure de couché du soleil (météo)
		Fermeture des volets

	Sinon, autre saison
		Si mode « Travail »
			Ouverture à l'heure du levé (« DEBOUT LA DEDANS ! »)
			Ouverture porte fenêtre uniquement si saison hors « Eté »
		Sinon
			« My » a l'heure du lever du soleil : ça évite d'avoir les portes fenêtres ouvertes si personne n'est debout

		Ensuite on boucle jusqu'à l'heure de coucher du soleil -1 h
			Si T°Salon > 25
				Volet en "My"
			Si T°Salon comprise en 21 et 25 et si production photo voltaïque
				Volet en « My »
			Si T°Salon < 19
				On ouvre les volets

			A heure de coucher du soleil -1 h
				Ouverture des volets

			A heure de coucher du soleil + 1h
			Si T°Salon > 25
				Volets sauf porte fenêtre en « My » (les fenêtres sont sans doute ouverte)
			Sinon
				Fermeture des volets sauf porte fenêtre

			si T°ext < 10° ou à 22h (mode «Travail ») ou 2h30 (mode « Vacances »)
				Fermeture porte fenetre « My » T°Salon > 25 ou totale sinon


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

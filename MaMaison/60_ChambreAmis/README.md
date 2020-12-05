# Automatisme de la chambre d'amis
-----

## Ouverture / fermeture programmées

~~Si mode "**Absent**" les volets restent fermés.~~

Si le mode est forcé à "**Vacances**" alors les volets restent fermés et la surveillance de température commence a 12h30.
S'il n'est pas forcé, le mode restera à "**Travail**" même si le mode générale est "**Vacances**". Le but est de ne pas laisser la chambre fermée lorsqu'il n'y a pas d'invité.
Fermeture avec le soleil.

## Soleil

L'**hiver**, dans le but d'économiser le chauffage
- *ouverture* à l'heure de lever du soleil + 1h 
- *fermeture* à l'heure du coucher - 1h

Les **autres saisons**, *ouverture* et *fermeture* avec le soleil

## Surveillance de la température

~~Comme la chambre d'amis n'a pas encore de sonde, la surveillance se fait sur celle de la chambre des parents.~~

~~Surveillance en dehors de l'*Hiver*, il faut que le mode soit *Travail* (début de surveillance à l'heure standard) ou *Vacances* (début à 12h30).~~

Surveillance en dehors de l'*Hiver*, il faut que le mode soit *Travail* (début de surveillance à "**DebutSurveillanceChAmis**", 8h15 (avant les autres car c'est la piece la plus exposée sauf) ou *Vacances* (début à "**DebutSurveillanceVacances**").	

## Technique

### timers
- **OuvertureVoletChAmis** - Timer d'ouverture des volets
- **FermetureVoletChAmis** - Timer de fermeture. Actif uniquement en mode *hiver*
### variables partagées
- **ChAmisAbsent** - Contient la valeur précédente du mode de la chambre d'amis. Pour éviter d'envoyer plusieurs fois l'ordre de fermeture du volet si on reste en mode *Absent*


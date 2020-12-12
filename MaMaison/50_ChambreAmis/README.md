# Automatisme de la chambre d'amis
-----

## Ouverture / fermeture programmées

Si le mode est forcé à "**Vacances**" alors les volets restent fermés et la surveillance de température commence a "**DebutSurveillanceVacances**"
S'il n'est pas forcé, le mode restera à "**Travail**" même si le mode générale est "**Vacances**". Le but est de ne pas laisser la chambre fermée lorsqu'il n'y a pas d'invité.
Fermeture avec le soleil.

## Soleil

L'**hiver**, dans le but d'économiser le chauffage
- *ouverture* à l'heure de lever du soleil + 1h 
- *fermeture* à l'heure du coucher - 1h

Les **autres saisons**, *ouverture* et *fermeture* avec le soleil

## Surveillance de la température

Surveillance en dehors de l'*Hiver*, il faut que le mode soit *Travail* (début de surveillance à "**DebutSurveillanceChAmis**", 8h15 (avant les autres car c'est la piece la plus exposée sauf) ou *Vacances* (début à "**DebutSurveillanceVacances**").	

## Technique

### timers
- **OuvertureVoletChAmis** - Timer d'ouverture des volets
- **FermetureVoletChAmis** - Timer de fermeture. Actif uniquement en mode *hiver*


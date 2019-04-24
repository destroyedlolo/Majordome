# Automatisme de la chambre d'amis
-----

## Ouverture / fermeture programmées

Si mode "**Absent**" les volets restent fermés.

Si le mode est "**Vacances**" alors les volets restent fermés et la surveillance de température commence a 12h.
Fermeture avec le soleil.

## Soleil

L'**hiver**, dans le but d'économiser le chauffage
- *ouverture* à l'heure de lever du soleil + 1h 
- *fermeture* à l'heure du coucher - 1h

Les **autres saisons**, *ouverture* et *fermeture* avec le soleil

## Surveillance

Comme la chambre d'amis n'a pas encore de sonde, la surveillance se fait sur celle du bureau.

## Technique

### timers
- **OuvertureVoletChAmis** - Timer d'ouverture des volets
- **FermetureVoletChAmis** - Timer de fermeture. Actif uniquement en mode *hiver*
## variables partagées
- **ChAmisAbsent** - Contient la valeur précédente du mode de la chambre d'amis. Pour éviter d'envoyer plusieurs fois l'ordre de fermeture du volet si on reste en mode *Absent*


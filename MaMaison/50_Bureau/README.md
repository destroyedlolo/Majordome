# Automatisme du bureau
---

## Ouverture / fermeture programmées

L'**hiver**, dans le but d'économiser le chauffage
- *ouverture* à l'heure de lever du soleil + 30m
- *fermeture* à l'heure du coucher - 15m

Les **autres saisons**, *ouverture* et *fermeture* avec le soleil
La surveillance standard est de mise.

## Technique

- **OuvertureVoletBureau** - Timer d'ouverture des volets
- **FermetureVoletBureau** - Timer de fermeture. Actif uniquement en mode *hiver*

## Surveillance de la température

Uniquement en dehors de l'hiver.
Sinon, les volets se ferment *my* lorsque la température atteint 21.5° (fait par le script **MyVoletBureau**).
A la fin de la surveillance, le volet est ré-ouvert uniquement s'il avait été fermé (**FinSurveillanceBureau**)


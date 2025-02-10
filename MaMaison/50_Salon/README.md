# Automatisme du salon
---

## Ouverture / fermeture programmées

L'**été** :
  - pour le volet **salon/Fenetre**, ouverture à la consigne solaire et début de la surveillance à "**SurveillanceOuest**"
  - uniquement une fermeture "**My**" au couché du soleil. Pas de lever le matin (les fenêtres ont peut-être laissée ouvertes pour rafraîchir).

Les autres saisons, *Ouverture* et *fermeture* avec les consignes solaires.

## Surveillance de la température

Uniquement en dehors de l'hiver.
Sinon, les volets se ferment *my* lorsque la température atteint 21.5° (fait par le script **MyVoletSalon**).
A la fin de la surveillance, le volet est ré-ouvert uniquement s'il avait été fermé (**FinDeJourneeSalon**)

Afin de garantir un minimum de lumière pour les plante, La fenêtre centrale ne se ferme que l'après midi, quand le soleil vient de l'Ouest


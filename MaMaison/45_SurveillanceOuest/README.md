# Gestion des surveillances de température pour les pieces donnant à l'ouest
---

Les pièces orientées **Ouest** sont exposées au soleil en début d'après midi.
Pour maximisé la lumière (conso électrique et santé des plantes), les volets se
ferment donc plus tard que le reste de la maison.

Hormis l'heure de début et le fait qu'il n'y a pas de différence pour les vacances, le principe reste le même que pour les autres pièces.

## Principe
---

Si c'est l'"**Hiver**", il n'y a pas de surveillance.
Les **autres saisons**, la surveillance débute à 12h45 (**DebutSurveillanceOuest**) stoppe 2 heures avant le coucher du soleil

## Techniquement
---

### Timers

Les timers suivants gèrent les horaires de la surveillance par défaut, chaque pièce pouvant avoir sa propre surveillance.

* **DebutSurveillanceOuest**
* **FinSurveillance** (depuis *Surveillance* general)

### Rendez-Vous

* **recalculeSurveillanceOuest** - Envoie une notification après le recalcule des horaires de surveillance. Utile dans le cas où une pièce n'a pas les même horaire que la surveillance globale (par exemple, débute sa surveillance 15 minutes près la surveillance globale).

* **activeSurveillanceOuest** et **desactiveSurveillanceOuest** - envoie une notification suite à la détermination de la saison (*hiver* ou *autre*)

### Notes

En saison **Hiver**, les timers sont désactivés ainsi que les trackers qui dépendent des rendez-vous.

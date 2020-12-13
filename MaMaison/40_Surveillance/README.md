# Gestion des surveillances de température.
---

## Principe
---

Si c'est l'"**Hiver**", il n'y a pas de surveillance.
Les **autres saisons**, la surveillance débute à 8h45 (**DebutSurveillance**) ou 12h30 (**DebutSurveillanceVacances**) stoppe 2 heures avant le coucher du soleil

## Techniquement
---

### Timers

Les timers suivants gèrent les horaires de la surveillance par défaut, chaque pièce pouvant avoir sa propre surveillance.

* **DebutSurveillance**
* **FinSurveillance**

### Rendez-Vous

* **recalculeSurveillance** - Envoie une notification après le recalcule des horaires de surveillance. Utile dans le cas où une pièce n'a pas les même horaire que la surveillance globale (par exemple, débute sa surveillance 15 minutes près la surveillance globale).

* **activeSurveillance** et **desactiveSurveillance** - envoie une notification suite à la détermination de la saison (*hiver* ou *autre*)

### Notes

En saison **Hiver**, les timers sont désactivés ainsi que les trackers qui dépendent des rendez-vous.

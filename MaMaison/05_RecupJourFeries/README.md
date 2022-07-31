# Recupération de la liste des jours fériés

Récupération de la liste depuis l'API du gouvernement, et uniquement
pour la métropole : https://api.gouv.fr/documentation/jours-feries

Elle se fait soit au démarrage de Majordome, soit à 2h15 (et plus à minuit 
sinon il aurait fallu faire une usine à gaz pour que la trappe du chat se
ferme à la bonne heure le lendemain d'un jour férié.


## Dependances

Nécessite **dkjson**, **LuaSocket** et **LuaSec**

``` 
luarocks install luasocket
luarocks install luasec
``` 

## Topics

- **.../JourFeries** - liste de jours fériés de l'année (JSON)
- **.../JourFerie/Nom** - Jour férié du jour
- **.../JourFerierSuivant** - Date du jour férié suivant
- **.../JourFerierSuivant/Nom** - Non du jour férié suivant

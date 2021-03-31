# Récupération des informations depuis la BDD
----

Permet aux tableaux de bord de récupérer les anciennes valeurs directement depuis la BDD

## informations reconnues

### Températures

Demandes arrivant sur **DemandeHistorique**.
Le *payload* contient l'information à recupérer.

Nom | contenu DemandeHistorique | probe dans domestik.probe_hardware et host='Maison'
----|---------------------------|--------------
Porte Garage | TSSPorte | Porte Garage
Sous-sol | TSS | Garage
Buanderie | TBuanderie | Buanderie
Cave à vin | TCVin | Cave a vin
Congelo | TCongelo | *N'existe pas*
Bureau | TBureau | Bureau
Chambre Parent | TChParent | Chambre Parents
Salon | TSalon | Salon
Grenier Nord | TGrenierNord | Grenier Nord
Chambre Joris | TChJoris | Chambre Joris
Chambre Oceane | TChOceane | Chambre Oceane
Chambre Amis | TAmis | Chambre Amis
Grenier Sud | TGrenierSud | Grenier Sud
Comble | TComble | Comble
Température extérieure | TDehors | Exterieur

### Valeures groupées

Demandes arrivant sur **DemandeHistoriqueG**.
Le *payload* contient `identifiant,groupe`.

Par exemple

`consomation2,300`

demande la consommation électrique groupées par 5 minutes (300 secondes).


Nom | contenu DemandeHistoriqueG | probe dans domestik.probe_hardware et host='EDF'
----|---------------------------|--------
Consommation électrique | consommation2 | consomation_Puissance
Production électrique | production2 | production_Puissance

### Autres

Nom | contenu DemandeHistorique | table
----|---------------------------|--------
Poulailler | TPoulailler |




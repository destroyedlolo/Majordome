# Gestion du coucher du soleil.
----

La détection du coucher effectif du soleil est basée sur ma génération photovoltaïque (*on n'y gagne pas grand chose, mais ça ne coûtait pas plus cher*) :
Lorsque la consigne _ConsigneCoucherSoleil_ est attente, Majordome surveille la production et on estime que le soleil est couché lorsque la **production est nulle depuis 2 minutes**.

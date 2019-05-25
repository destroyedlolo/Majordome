# Gestion du coucher du soleil.
----

La détection du coucher effectif du soleil est basée sur ma génération photovoltaïque (*on n'y gagne pas grand chose, mais ça ne coûtait pas plus cher*) :
Lorsque la consigne _ConsigneCoucherSoleil_ est attente, Majordome surveille la production et on estime que le soleil est couché lorsque la **production est nulle depuis 2 minutes**.

Il faut donc suivre le timer **CoucherSoleil** pour être informé lorsque le soleil est réellement couché.

Le rendez-vous **CoucherSoleilChange** est activé lorsque **ConsigneCoucherSoleil** est changée.

**Note :** Pour faire des économies l'hiver, la surveillance du coucher débute 15 minutes avant la consigne

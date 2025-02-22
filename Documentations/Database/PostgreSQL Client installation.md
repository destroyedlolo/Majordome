This file containes notes specific to **PostgreSQL database**.

# PostgreSQL clients

If you installed the database engine on the same machine as Majordome will run, all dependencies should be already installed.

If the database engine is not installed, you need to install only **PostgreSQL client** :

## Arch Linux

Install `postgresql-libs`

## Gentoo Linux

Change your use to
```
dev-db/postgresql -server
```
then the usual
```
emerge -va postgresql
```

> [!NOTE]  
>  `psql` client is able to connect to (*very*) old database engine as you can see bellow on my test environment.

```
psql (15.3, serveur 11.7)
Saisissez « help » pour l'aide.

test=> 
```

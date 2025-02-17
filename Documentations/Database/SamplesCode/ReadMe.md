This directory contains several examples to use Majordome's database extension.

# Prerequisites
## Database
Obviously, you need an up-and-running [PostgreSQL](https://www.postgresql.org/) database.

### Notes about remote databases
If you want to a database installed on another server, you need
- configure your database to accept remote connections (which is not the case with most of Linux distributions). Please refer to PostgreSQL's documentation.
- on the machine where Majordome will run, you need at last to install PostgreSQL's library.
- Majordome itself needs to be compiled with PostgreSQL support.

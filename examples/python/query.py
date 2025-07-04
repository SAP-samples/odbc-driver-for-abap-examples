"""Execute a SQL query via ODBC in Python."""

import getpass
import pyodbc

# some documentation in
# https://code.google.com/archive/p/pyodbc/wikis/GettingStarted.wiki
# https://github.com/mkleehammer/pyodbc/wiki
#
# install pyodbc before running the program
# pip install pyodbc
#
# run with
# python query.py


def main():
    """Main function to execute a SQL query."""

    dsn = input("Your ODBC DSN? ")
    user = input("Your Connect User? ")
    pwd = getpass.getpass("Your Connect Password? ")
    query = input("Your SQL Query? ")

    connection = pyodbc.connect(
        "DSN=" + dsn + ";UID=" + user + ";PWD=" + pwd + ";Trace=none;"
    )
    curs = connection.cursor()
    curs.execute(query)
    for row in curs.fetchall():
        print(row)
    curs.close()
    connection.close()


if __name__ == "__main__":
    main()

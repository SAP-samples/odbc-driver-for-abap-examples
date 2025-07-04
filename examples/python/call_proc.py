"""Call a stored procedure in ABAP via ODBC using pyodbc."""

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
# python call_proc.py


def main():
    """Main function to demonstrate calling a stored procedure."""
    dsn = input("Your ODBC DSN? ")
    user = input("Your Connect User? ")
    pwd = getpass.getpass("Your Connect Password? ")

    connection = pyodbc.connect(
        "DSN=" + dsn + ";UID=" + user + ";PWD=" + pwd + ";Trace=none;"
    )
    curs = connection.cursor()
    curs.execute("call dbc2abaptest.get_time_proc( ex_time => RESULT )")

    for row in curs.fetchall():
        print(row)

    curs.close()
    connection.close()


if __name__ == "__main__":
    main()

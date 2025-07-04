"""Example for calling a procedure with a table parameter using pyodbc"""

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
# python call_proc_table.py


def main():
    """Main function to demonstrate calling a stored procedure with table parameters."""

    dsn = input("Your ODBC DSN? ")
    user = input("Your Connect User? ")
    pwd = getpass.getpass("Your Connect Password? ")

    connection = pyodbc.connect(
        "DSN=" + dsn + ";UID=" + user + ";PWD=" + pwd + ";Trace=none;"
    )

    ltt_curs = connection.cursor()
    ltt_curs.execute(
        "CREATE LOCAL TEMPORARY TABLE SYS_TEMP.TABLE_IN "
        "( IND INT4 NOT NULL, TEXT CHAR(30) NOT NULL, NUM INT4 NOT NULL ) ON COMMIT PRESERVE ROWS"
    )
    ltt_curs.close()

    items = [(1, "test1", 100), (2, "test2", 200)]
    insert_curs = connection.cursor()

    # switch on ODBC array insert
    insert_curs.fast_executemany = True
    insert_curs.executemany("INSERT INTO SYS_TEMP.TABLE_IN VALUES( ?, ?, ? )", items)
    insert_curs.close()

    in_scalar = 42
    call_curs = connection.cursor()
    call_curs.execute(
        "CALL DBC2ABAPTEST.TABLE_IN_TABLE_OUT"
        "( IN_SCALAR => ? , "
        "  IN_TABLE => SYS_TEMP.TABLE_IN , "
        "  OUT_TABLE_1 => RESULT , "
        "  OUT_TABLE_2 => RESULT )",
        in_scalar,
    )

    # print first result set
    print("\nFirst result set:")
    for row in call_curs.fetchall():
        print(row)

    # switch to second result set
    call_curs.nextset()

    # print second result set
    print("\nSecond result set:")
    for row in call_curs.fetchall():
        print(row)

    # clean up
    call_curs.close()
    connection.close()


if __name__ == "__main__":
    main()

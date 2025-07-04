"""Array DML demo for Python with pyodbc"""

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
# python array_dml_demo.py


def truncate_data(conn):
    """Truncate dbc2abaptest.writable_view by deleting all rows."""
    truncate_curs = conn.cursor()
    sql_truncate = "DELETE FROM dbc2abaptest.writable_view"
    truncate_curs.execute(sql_truncate)
    row_count = truncate_curs.rowcount
    print("Number of truncated rows: " + str(row_count))
    truncate_curs.close()


def print_data(conn):
    """Print the contents of dbc2abaptest.writable_view."""
    query_cursor = conn.cursor()
    sql_query = "SELECT * FROM dbc2abaptest.writable_view"
    query_cursor.execute(sql_query)
    print("Table contents:")
    for row in query_cursor.fetchall():
        print(row)
    query_cursor.close()


def main():
    """Main function to demonstrate INSERT and DELETE."""
    dsn = input("Your ODBC DSN? ")
    user = input("Your Connect User? ")
    pwd = getpass.getpass("Your Connect Password? ")

    conn = pyodbc.connect(
        "DSN=" + dsn + ";UID=" + user + ";PWD=" + pwd + ";Trace=none;"
    )

    truncate_data(conn)

    # The following test code runs on a writable view entity exposed as
    #   dbc2abaptest.writable_view
    # with columns
    #   key kchar : abap.char(10) not null;
    #       fint  : abap.int4;

    # array data for insert
    array_data = [
        ("a", 17),
        ("b", 42),
    ]

    insert_curs = conn.cursor()
    insert_curs.fast_executemany = True
    sql_insert_with_values_clause = (
        "INSERT INTO dbc2abaptest.writable_view VALUES (?, ?)"
    )
    insert_curs.executemany(sql_insert_with_values_clause, array_data)
    print("Records inserted via array: " + str(len(array_data)))
    print_data(conn)

    # pyodbc always returns row_count=-1 for executemany()
    # https://github.com/mkleehammer/pyodbc/issues/481
    # row_count = insert_curs.rowcount
    # print('Number of inserted rows: ' + str(row_count) )

    sql_insert_select = (
        "INSERT INTO dbc2abaptest.writable_view "
        "SELECT cast( kchar || '_2' as char(10) ), fint+1 FROM dbc2abaptest.writable_view"
    )
    insert_curs.execute(sql_insert_select)
    row_count = insert_curs.rowcount
    insert_curs.close()
    print("Records inserted via select: " + str(row_count))
    print_data(conn)

    delete_curs = conn.cursor()
    delete_curs.fast_executemany = True
    sql_delete = "DELETE FROM dbc2abaptest.writable_view WHERE kchar = ? AND fint = ?"
    delete_curs.executemany(sql_delete, array_data)
    print("Deleted records: " + str(len(array_data)))
    delete_curs.close()
    print_data(conn)

    truncate_data(conn)

    conn.close()


if __name__ == "__main__":
    main()

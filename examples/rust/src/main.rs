use anyhow::Error;
use odbc_api::{
    buffers::TextRowSet, ConnectionOptions, Cursor, Environment, IntoParameter, ResultSetMetadata,
};
use rpassword::read_password;
use text_io::read;

fn simple_select(connection: &odbc_api::Connection) -> Result<(), Error> {
    let sql = "select * from sys.dummy";
    match connection.execute(sql, ())? {
        Some(mut cursor) => {
            if let Some(mut row) = cursor.next_row()? {
                let mut buf = Vec::new();
                row.get_text(1, &mut buf)?;
                let ret = String::from_utf8(buf).unwrap();
                println!("{:?}", ret);
            }
        }
        None => {
            eprintln!("empty result");
        }
    }
    Ok(())
}

fn create_temp_table(connection: &odbc_api::Connection) -> Result<(), Error> {
    let sql = "create local temporary table sys_temp.table_in (ind int4 not null, text char(30) not null, num int4 not null) on commit preserve rows";
    connection.execute(sql, ())?;
    let inputs_for_tab = [(1, "test1", 100), (2, "test2", 200)];
    for (ind, text, num) in inputs_for_tab {
        connection.execute(
            "insert into sys_temp.table_in values(?, ?, ?)",
            (&ind, &text.into_parameter(), &num),
        )?;
    }
    Ok(())
}

fn procedure_call(connection: &odbc_api::Connection) -> Result<(), Error> {
    let sql = "call dbc2abaptest.table_in_table_out(in_scalar => ?, in_table => sys_temp.table_in, out_table_1 => RESULT, out_table_2 => RESULT)";
    let in_scalar = 42;
    match connection.execute(sql, &in_scalar)? {
        Some(mut cursor) => {
            println!("table result:");
            loop {
                let headline: Vec<String> = cursor.column_names()?.collect::<Result<_, _>>()?;
                println!("{}", headline.join(", "));

                let mut buffers = TextRowSet::for_cursor(500, &mut cursor, None)?;
                let mut row_set_cursor = cursor.bind_buffer(&mut buffers)?;
                while let Some(batch) = row_set_cursor.fetch()? {
                    for row_index in 0..batch.num_rows() {
                        let field = batch.at(0, row_index).unwrap_or(&[]);
                        print!("{}", String::from_utf8_lossy(field));
                        for col_index in 1..batch.num_cols() {
                            let field = batch.at(col_index, row_index).unwrap_or(&[]);
                            print!(", {}", String::from_utf8_lossy(field));
                        }
                        print!("\n");
                    }
                }
                cursor = (row_set_cursor.unbind()?).0;

                if let Some(new_cursor) = cursor.more_results()? {
                    println!("\nnext table result:");
                    cursor = new_cursor;
                } else {
                    break;
                }
            }
        }
        None => {
            eprintln!("empty result");
        }
    }

    Ok(())
}

fn main() -> Result<(), Error> {
    println!("DSN:");
    let dsn: String = read!();
    println!("user:");
    let user: String = read!();
    println!("password:");
    let password = read_password().unwrap();

    let environment = Environment::new()?;
    let connection = environment.connect(&dsn, &user, &password, ConnectionOptions::default())?;

    simple_select(&connection)?;
    println!("");
    create_temp_table(&connection)?;
    procedure_call(&connection)?;

    Ok(())
}

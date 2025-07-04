// simple node.js sample to call a procedure
//
// To run the program you need a local node.js installation.
// In addition you may need to run `npm install odbc readline-sync` once.
//
// node call_procedure.js
//
// The program requires a DSN definition for your target database

'use strict';

const odbc         = require('odbc');
const readlineSync = require('readline-sync');


(async () => {

  var myDSN = readlineSync.question('ODBC DSN? ');
  console.log('DSN ' + myDSN );

  var myUser = readlineSync.question('Connect User? ');
  console.log('User ' + myUser );

  var myPwd = readlineSync.question('Password? ', {hideEchoBack: true});

  let connectionString = 'DSN=' + myDSN + ';' + 'Uid=' + myUser + ';' + 'Pwd=' + myPwd;

  const connection = await odbc.connect( connectionString ).catch(error => console.error(error));

  // create local temporary table
  await connection.query('create local temporary table sys_temp.table_in' +
                         '(ind int4 not null, text char(30) not null, num int4 not null) on commit preserve rows')
                  .catch(error => console.error(error));


  // insert data into local temporary table
  let statement;
  statement = await connection.createStatement();
  await statement.prepare('insert into sys_temp.table_in values(?, ?, ?)').catch(error => console.error(error));
  const parameters = [ [1, 'test1', 100], [2, 'test2', 200] ];
  for (let i = 0; i < parameters.length; i++) {
    await statement.bind(parameters[i]).catch(error => console.error(error));
    await statement.execute().catch(error => console.error(error));
  }
  statement.close();

  // call procedure via query
  let in_scalar = 42;
  let result = await connection.query('call dbc2abaptest.table_in_table_out(in_scalar => ?, in_table => sys_temp.table_in, out_table_1 => RESULT, out_table_2 => RESULT)', [in_scalar] )
                               .catch(error => console.error(error));

  // only the first result set is returned
  console.log(result);

  // ignore error due to lack of transaction handling
  connection.close().catch(() => {});

})()

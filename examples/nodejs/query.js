// simple ODBC query tool based on node.js
//
// To run the program you need a local node.js installation.
// In addition you may need to run `npm install odbc readline-sync` once.
//
// node query.js
//
// The program requires a DSN definition for your target database

'use strict';

const odbc = require('odbc');
const readlineSync = require('readline-sync');


(async () => {

  var myDSN = readlineSync.question('ODBC DSN? ');
  console.log('DSN ' + myDSN );

  var myUser = readlineSync.question('Connect User? ');
  console.log('User ' + myUser );

  var myPwd = readlineSync.question('Password? ', {hideEchoBack: true});

  let connectionString = 'DSN=' + myDSN + ';' + 'Uid=' + myUser + ';' + 'Pwd=' + myPwd + ';';

  const connection = await odbc.connect( connectionString ).catch(error => console.error(error));

  let myQuery;

  do {

    myQuery = readlineSync.question('Query? ');
    console.log( myQuery );

    if( myQuery != '' ){

      // single row fetch
      let result = await connection
                          .query( myQuery )
                          .catch(error => console.error(error));
      console.log(result);

      /*
      // array fetch of 100 rows
      let cursor = await connection
                          .query( myQuery , { cursor: true, fetchSize: 100 } )
                          .catch(error => console.error(error));
      let result = await cursor.fetch();
      console.log(result);
      */
    }

  } while( myQuery != '');

  // ignore error due to lack of transaction handling
  connection.close().catch(() => {});

})()

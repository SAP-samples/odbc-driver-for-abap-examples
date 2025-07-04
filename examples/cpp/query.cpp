#include <iostream>
#include <cstring>

#ifdef _WIN64
#include "windows.h"
#endif

// The ODBC header files are required to run the example
// Linux:
// They are installed with the unixODBC-dev package
// Windows:
// typical location:
// C:\Program Files (x86)\Windows Kits\10\Include\<version>\um
#define UNICODE
#include <sqltypes.h>
#include <sqlext.h>
#include <sql.h>
#include <sqlucode.h>

#ifdef __linux__
#include <iconv.h>
#endif

// The example connects to a data source specified by a DSN (Data Source Name), username, and
// password. After this it prepares and executes a parameterized SQL query. Since the "ODBC driver
// for ABAP" is a unicode only ODBC driver, the wide character versions of the ODBC functions,
// denoted by the W suffix (e.g., SQLDriverConnectW, SQLPrepareW) are used.
// The checkError function is used to print diagnostic information if an error occurs.

// The wide character ODBC functions use the data type SQLWCHAR which is equivalent to char_16t.
// On Windows the wchar_t data type is equivalent to char_16t
// while on Linux a conversion between those data types is neccessary.
// This is done with the functions convert_to_u16string and convert_from_sqlwchar.
// The example compiles both on Windows and linux but on Windows
// the code could be largly simplified by removing those conversions.
//
// Before compiling the program first adapt the dsn, user and pass variables
// in the program body.
//

// LINUX:
// The conversion functions in this example use the iconv module
// because it does not require the installation of additional software components.
// The same could be achieded with ICU, boost, codecvt or other modules.
//
// The example uses a DSN in the connection string.
// Therefore, to resolve the individial attributes the unixODBC module needs to be installed.
// The example can be compiled and linked with or without the unixODBC driver manager library.
// If the example is linked without the unixODBC driver manager, and the DSN is replaced with a
// complete connection string, the unixODBC module is not required.
// (1) using the ODBC driver for ABAP directly.
//     Compile the program with:
//     g++ query.cpp -o query -L <install dir of ODBC_driver_for_ABAP.so>                     \
//        -l:ODBC_driver_for_ABAP.so -lc
// (2) The ODBC driver for ABAP is loaded by the unixODBC driver manager
//     g++ query.cpp -o query -lodbc -lc
//

// WINDOWS:
// On Windows the conversion functions are not needed and the code could be simplified.
// On Windows an ODBC driver manager is always present and the example requires a DSN definition.
// SAP ships only the ODBC_driver_for_ABAP.dll file and not the corrsponding lib file.
// Unlike an Linux a DLL file can not be directly linked to an executable.
// So the example needs to be compiled with the ODBC driver manager lib.
//
// To compile the example with Microsoft Visual Studio from command line
// you first need to initialize the command line environment in a cmd window using vcvars64.bat.
// Afterwards you can compile the program using the following cl command.
// cl.exe /EHsc query.cpp /link odbc32.lib
//

void           check_error(SQLRETURN rc, SQLSMALLINT handleType, SQLHANDLE handle);
std::u16string convert_to_u16string(const std::wstring& input);
std::wstring   convert_from_sqlwchar(const SQLWCHAR* input, SQLLEN input_chars);

int main()
{
  // Define handles and variables
  SQLHENV   hEnv  = SQL_NULL_HANDLE;
  SQLHDBC   hDbc  = SQL_NULL_HANDLE;
  SQLHSTMT  hStmt = SQL_NULL_HANDLE;
  SQLRETURN sqlRc;

  // Allocate an environment handle
  sqlRc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
  check_error(sqlRc, SQL_HANDLE_ENV, hEnv);

  // Set the ODBC version environment attribute
  sqlRc = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 0);
  check_error(sqlRc, SQL_HANDLE_ENV, hEnv);

  // Allocate a connection handle
  sqlRc = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
  check_error(sqlRc, SQL_HANDLE_DBC, hDbc);

  // Connect to the data source using Unicode functions
  std::wcout << L"Connecting to the data source..." << std::endl;
  std::wstring dsn     = L"MYDSN";  // Replace with your actual DSN
  std::wstring user    = L"MYUSER"; // Replace with your actual username
  std::wstring pass    = L"MYPWD";  // Replace with your actual password
  std::wstring connStr = L"DSN=" + dsn + L";UID=" + user + L";PWD=" + pass;

  std::u16string connStrU16 = convert_to_u16string(connStr);

  sqlRc = SQLDriverConnectW(hDbc, nullptr, reinterpret_cast<SQLWCHAR*>(&connStrU16[0]), SQL_NTS,
                            nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);
  check_error(sqlRc, SQL_HANDLE_DBC, hDbc);

  // Allocate a statement handle
  sqlRc = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
  check_error(sqlRc, SQL_HANDLE_STMT, hStmt);

  // Prepare the SQL query with parameter markers
  std::wstring   queryStr = L"SELECT SCHEMA_NAME, VIEW_NAME FROM SYS.VIEWS WHERE SCHEMA_NAME = ?";
  std::u16string queryStrU16 = convert_to_u16string(queryStr);

  // Prepare the statement
  sqlRc = SQLPrepareW(hStmt, reinterpret_cast<SQLWCHAR*>(&queryStrU16[0]), SQL_NTS);
  check_error(sqlRc, SQL_HANDLE_STMT, hStmt);

  // Define variables for parameter binding
  std::wstring   schema    = L"SYS";
  std::u16string schemaU16 = convert_to_u16string(schema);
  SQLLEN         cbParamId = SQL_NTS;

  // Bind the parameter
  sqlRc =
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0,
                       reinterpret_cast<SQLWCHAR*>(&schemaU16[0]), schema.size() * 2, &cbParamId);
  check_error(sqlRc, SQL_HANDLE_STMT, hStmt);

  // Execute the prepared statement
  sqlRc = SQLExecute(hStmt);
  check_error(sqlRc, SQL_HANDLE_STMT, hStmt);

  // Fetch and display the result
  std::wcout << L"Fetching result set ..." << std::endl;
  const SQLLEN wcharBufSz = 50;
  SQLWCHAR     schemaNameBuf[wcharBufSz], viewNameBuf[wcharBufSz];
  SQLLEN       schemaNameLen, viewNameLen;

  while (SQL_SUCCESS == (sqlRc = SQLFetch(hStmt)))
  {
    SQLGetData(hStmt, 1, SQL_C_WCHAR, schemaNameBuf, wcharBufSz * sizeof(char16_t), &schemaNameLen);
    SQLGetData(hStmt, 2, SQL_C_WCHAR, viewNameBuf, wcharBufSz * sizeof(char16_t), &viewNameLen);
    std::wstring schemaName =
        convert_from_sqlwchar(schemaNameBuf, schemaNameLen / sizeof(char16_t));
    std::wstring viewName = convert_from_sqlwchar(viewNameBuf, viewNameLen / sizeof(char16_t));
    std::wcout << L"Schema Name: " << schemaName << L", View Name: " << viewName << std::endl;
  }

  // Clean up
  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  SQLDisconnect(hDbc);
  SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
  SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

  return 0;
}

void check_error(SQLRETURN rc, SQLSMALLINT handleType, SQLHANDLE handle)
{
  if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
  {
    SQLSMALLINT recNr = 0;
    SQLWCHAR    sqlState[6], messageText[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER  nativeError;
    SQLSMALLINT textLen;

    while (SQLGetDiagRecW(handleType, handle, ++recNr, sqlState, &nativeError, messageText,
                          sizeof(messageText) / sizeof(SQLWCHAR), &textLen) == SQL_SUCCESS)
    {
      std::wstring sqlStateStr    = convert_from_sqlwchar(sqlState, 6);
      std::wstring messageTextStr = convert_from_sqlwchar(messageText, textLen);
      std::wcerr << L"SQLState: " << sqlStateStr << L", NativeError: " << nativeError
                 << L", Message: " << messageTextStr << std::endl;
    }
    exit(EXIT_FAILURE);
  }
}

std::u16string convert_to_u16string(const std::wstring& input)
{
#ifdef _WIN64
  std::u16string u16str(input.begin(), input.end());
  return u16str;
#else
  std::u16string output(input.size() * 2, u'\0'); // UTF-16 surrogate pairs can take up to 2 chars

  iconv_t conv = iconv_open("UTF-16LE", "UTF-32LE");
  if (conv == reinterpret_cast<iconv_t>(-1))
  {
    std::cerr << "iconv_open failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  char*  inputBuf        = const_cast<char*>(reinterpret_cast<const char*>(input.c_str()));
  char*  outputBuf       = reinterpret_cast<char*>(output.data());
  size_t inputBytesLeft  = input.size() * sizeof(wchar_t);
  size_t outputBytesLeft = output.size() * sizeof(char16_t);

  if (iconv(conv, &inputBuf, &inputBytesLeft, &outputBuf, &outputBytesLeft) ==
      static_cast<size_t>(-1))
  {
    std::cerr << "iconv failed" << std::endl;
    exit(EXIT_FAILURE);
  }
  output.resize(output.size() - outputBytesLeft / sizeof(char16_t));

  std::u16string result(output.data());
  iconv_close(conv);
  return result;
#endif
}

std::wstring convert_from_sqlwchar(const SQLWCHAR* input, SQLLEN inputChars)
{
#ifdef _WIN64
  std::wstring result(reinterpret_cast<wchar_t*>(input), inputChars);
  return result;
#else
  std::wstring output(inputChars, L'\0');

  iconv_t conv = iconv_open("UTF-32LE", "UTF-16LE");
  if (conv == reinterpret_cast<iconv_t>(-1))
  {
    std::cerr << "iconv_open failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  char*  inputBuf        = const_cast<char*>(reinterpret_cast<const char*>(input));
  char*  outputBuf       = reinterpret_cast<char*>(output.data());
  size_t inputBytesLeft  = inputChars * sizeof(SQLWCHAR);
  size_t outputBytesLeft = output.size() * sizeof(wchar_t);

  if (iconv(conv, &inputBuf, &inputBytesLeft, &outputBuf, &outputBytesLeft) ==
      static_cast<size_t>(-1))
  {
    std::cerr << "iconv failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::wstring result(output);
  iconv_close(conv);
  return result;
#endif
}

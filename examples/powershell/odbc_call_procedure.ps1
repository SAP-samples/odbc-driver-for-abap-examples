# run in powershell window
# odbc_call_procedure.ps1

# input connection parameters
Write-Output "";
$dsn = Read-Host "ODBC DSN";
$uid = Read-Host "ABAP User";
$pwdsec = Read-Host -AsSecureString "ABAP Password";
$pwdbstr = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($pwdsec)
$pwdvalue = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($pwdbstr)
Write-Output "";

# connect via ODBC
$sqlConnection = New-Object System.Data.ODBC.ODBCConnection
$sqlConnection.connectionstring =
"DSN=" + $dsn +
";UID=" + $uid +
";PWD=" + $pwdvalue

Write-Output ("Opening Connection to: " + "DSN=" + $dsn + ";UID=" + $uid)

$sqlConnection.Open()
Write-Output ("Connection State: " + $sqlConnection.State)

# prepare and execute the procedure
$sqlCmd = $sqlConnection.CreateCommand()
$sqlCmd.CommandText = "CALL Dbc2AbapTest.get_time_proc( ex_time => RESULT )"
$result = $sqlCmd.ExecuteReader()
Write-Output "Result:";
while ($result.Read()) {
    Write-Output ("time_point: " + $result.GetValue(0).ToString())
    Write-Output ("timestamp_utc: " + $result.GetValue(1).ToString())
}

$result.Close()
$sqlConnection.Close()

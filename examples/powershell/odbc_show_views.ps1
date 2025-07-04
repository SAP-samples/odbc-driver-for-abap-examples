# run in powershell window
# odbc_show_views.ps1

# input connection parameters
Write-Output "";
$dsn = Read-Host "ODBC DSN?";
$uid = Read-Host "ABAP User?";
$pwdsec = Read-Host -AsSecureString "ABAP Password";
$pwdbstr = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($pwdsec)
$pwdvalue = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($pwdbstr)
Write-Output "";

# Connect via ODBC
$SqlConnection = New-Object System.Data.ODBC.ODBCConnection
$SqlConnection.connectionstring =
    "DSN=" + $dsn +
    ";UID=" + $uid +
    ";PWD=" + $pwdvalue

Write-Output $SqlConnection.connectionstring

Write-Output "Opening Connection to:"
$constr_without_pwd = "DSN=" + $dsn + ";UID=" + $uid
Write-Output $constr_without_pwd
$SqlConnection.Open()
Write-Output "Connection State:"
$SqlConnection.State

$ScalarSqlQuery = "select count(*) from SYS.VIEWS"
$Command = New-Object System.Data.ODBC.ODBCCommand($ScalarSQLQuery, $SQLConnection)
$Reply = $Command.executescalar()
Write-Output "";
Write-Output "Number of exposed entities:";
$Reply
Write-Output "";

$Sqlcmd = $SqlConnection.CreateCommand()
$Sqlcmd.CommandText = "SELECT SCHEMA_NAME, VIEW_NAME FROM SYS.VIEWS WHERE SCHEMA_NAME = 'SYS' "
$SysTabResult = $Sqlcmd.ExecuteReader()
Write-Output "Exposed System Views:";
while ($SysTabResult.Read())
{
    $SysTabResult.GetValue(0) + "." + $SysTabResult.GetValue(1)
}
$SysTabResult.Close()
Write-Output "";

$SqlConnection.Close()

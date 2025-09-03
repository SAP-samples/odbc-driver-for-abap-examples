# the following gems need to be installed before running this program
# gem install dbi
# gem install dbd-odbc
# gem install io-console
#
# the dbi gem is deprecated
#
# run the program with 
# ruby show_views_dbi.rb

require 'dbi'
require 'io/console'

begin
  # prompt for DSN, user and password
  dsn = (print 'DSN: '; gets&.rstrip)
  username = (print 'User: '; gets&.rstrip)
  $stdout.print "Password: "
  password = $stdin.noecho(&:gets)

  # Establish the database connection
  conn = DBI.connect('DBI:ODBC:' + dsn, username, password)
  puts "Connection successful!"

  # Example SQL query to fetch data
  query = "SELECT SCHEMA_NAME, VIEW_NAME FROM SYS.VIEWS WHERE SCHEMA_NAME = 'SYS' "

  # Prepare the statement
  stmt = conn.prepare(query)

  # Execute the statement
  stmt.execute

  # Fetch and print the result
  stmt.fetch do |row|
    puts row.inspect
  end

  # Clean up
  stmt.finish
  conn.disconnect
  puts "Disconnected from the database."

rescue DBI::DatabaseError => e
  puts "An error occurred"
  puts "Error code:    #{e.err}"
  puts "Error message: #{e.errstr}"
ensure
  # Ensure resources are released if an error occurs
  stmt.finish if stmt && !stmt.finished?
  conn.disconnect if conn && conn.connected?
end

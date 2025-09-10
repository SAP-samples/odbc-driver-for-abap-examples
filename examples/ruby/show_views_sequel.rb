# the following gem needs to be installed before running this program
# gem install ruby-odbc
# gem install sequel
# gem install io-console
#
# run the program with 
# ruby show_views_sequel.rb

require 'sequel'
require 'io/console'

begin
  # prompt for DSN, user and password
  dsn = (print 'DSN: '; gets&.rstrip)
  username = (print 'User: '; gets&.rstrip)
  $stdout.print "Password: "
  password = $stdin.noecho(&:gets)
  
  # Establish the database connection
  conn = Sequel.odbc(dsn, user: username, password: password)
  puts "Connection successful!"

  # Example SQL query to fetch data
  query = "SELECT SCHEMA_NAME, VIEW_NAME FROM SYS.VIEWS WHERE SCHEMA_NAME = 'SYS' "

  # Execute the statement and fetch the result
  result = conn[query].all

  # Print the result
  result.each do |row|
    puts row.inspect
  end

rescue Sequel::DatabaseError => e
  puts "An error occurred"
  puts "Error message: #{e.message}"
ensure
  # Disconnect from the database
  conn.disconnect
  puts "Disconnected from the database."
end

# the following packages need to be installed before running this program
# install.packages("odbc") 
# install.packages("getPass") 
#
# run the program with
# Rscript show_views.R


# Load the odbc pnd getPass package
library(odbc)
library(getPass)

# Function for user input
user_input <- function(prompt) {
  if (interactive()) {
    return(readline(prompt))
  } else {
    cat(prompt)
    return(readLines("stdin", n=1))
  }
}

# Define the Database Connection Parameters
dsn <- user_input("DSN: ");
uid <- user_input("User: ");
pwd <- getPass("Password: ");

# Create the connection string
connection_string <- sprintf("DSN=%s;UID=%s;PWD=%s", dsn, uid, pwd)

# Create a connection to the database
con <- dbConnect(odbc(), .connection_string = connection_string)

# Verify the connection
if (dbIsValid(con)) {
  cat("Connection successful.\n")
} else {
  stop("Connection failed.")
}

# Define the SQL query
sql_query <- "SELECT SCHEMA_NAME, VIEW_NAME FROM SYS.VIEWS WHERE SCHEMA_NAME = 'SYS'"

# Execute the query and retrieve the data
result <- dbGetQuery(con, sql_query)

# Print the retrieved data
print(result)

# Close the connection
dbDisconnect(con)

cat("Connection closed.\n")

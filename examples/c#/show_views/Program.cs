
using System.Data.Odbc;

public class Program
{
    public static void Main()
    {
        try
        {
            // Prompt for DSN, username, and password (re-prompts until non-empty)
            string dsn = ReadInput("Enter your DSN: ");
            string user = ReadInput("Enter your Username: ");
            string pwd = ReadPassword("Enter your Password: ");

            // Create and open the ODBC connection
            using var connection = new OdbcConnection($"DSN={dsn};UID={user};PWD={pwd};");
            connection.Open();

            // Prepare and execute the SQL command to fetch all views
            using var command = new OdbcCommand("SELECT * FROM SYS.VIEWS", connection);
            using var reader = command.ExecuteReader();

            // Print the first column (the view name) of each row 
            while (reader.Read())
            {
                Console.WriteLine(reader[0]?.ToString());
            }
        }
        catch (OdbcException exception)
        {
            // Handle ODBC-specific errors
            Console.Error.WriteLine($"ODBC error: {exception.Message}");
        }
        catch (Exception exception)
        {
            // Handle any other unexpected errors
            Console.Error.WriteLine($"Unexpected error: {exception.Message}");
        }
    }

    private static string ReadInput(string prompt)
    {
        string input;
        do
        {
            Console.Write(prompt);
            input = Console.ReadLine() ?? string.Empty;
        } while (string.IsNullOrWhiteSpace(input));
        return input;
    }

    private static string ReadPassword(string prompt)
    {
        string password;
        do
        {
            Console.Write(prompt);
            password = string.Empty;
            ConsoleKeyInfo key;
            do
            {
                key = Console.ReadKey(true);
                if (key.Key == ConsoleKey.Backspace && password.Length > 0)
                {
                    password = password.Remove(password.Length - 1);
                    Console.Write("\b \b");
                }
                else if (!char.IsControl(key.KeyChar))
                {
                    password += key.KeyChar;
                    Console.Write("*");
                }
            } while (key.Key != ConsoleKey.Enter);
            Console.WriteLine();
        } while (string.IsNullOrWhiteSpace(password));
        return password;
    }
}

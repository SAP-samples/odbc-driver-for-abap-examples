namespace Devtoberfest.OrderApp
{
    using System;
    using System.Data.Odbc;
    using System.IO;

    public class Program
    {
        public static void Main()
        {
            switch (ReadInput("1 - Create Order\n2 - Delete Order\n3 - Summarize Result\n"))
            {
                case "1":
                    CreateOrder();
                    break;
                case "2":
                    DeleteOrder();
                    break;
                case "3":
                    SummarizeResult();
                    break;
                default:
                    Console.WriteLine("Invalid input. Please enter 1, 2, or 3.");
                    return;
            }
        }

        private static string BuildConnectionString()
        {
            string dsn = ReadConfigFromFile("DSN");
            string user = ReadConfigFromFile("UID");
            string pwd = ReadConfigFromFile("PWD");
            return $"DSN={dsn};UID={user};PWD={pwd};";
        }

        private static Guid GetGuidFromDb(OdbcConnection connection)
        {
            try
            {
                using var uuidCommand = new OdbcCommand("SELECT UUID() FROM SYS.DUMMY", connection);
                return new Guid((byte[])uuidCommand.ExecuteScalar()!);
            }
            catch (OdbcException exception)
            {
                Console.Error.WriteLine($"ODBC error: {exception.Message}");
                Environment.Exit(1);
                return Guid.Empty; // This line will never be reached, but is needed for compilation.
            }
        }

        private static (int BeerOrderAmount, int FoodOrderAmount, bool Confirmed) GetCreateOrderInput()
        {
            int beerOrderAmount, foodOrderAmount;
            while (true)
            {
                var beerInput = ReadInput("Enter Beer Order Amount: ");
                if (int.TryParse(beerInput, out beerOrderAmount))
                    break;
                Console.WriteLine("Invalid number. Please try again.");
            }
            while (true)
            {
                var foodInput = ReadInput("Enter Food Order Amount: ");
                if (int.TryParse(foodInput, out foodOrderAmount))
                    break;
                Console.WriteLine("Invalid number. Please try again.");
            }
            var confirmedInput = ReadInput("Create order? (y/n): ");
            return (beerOrderAmount, foodOrderAmount, confirmedInput.ToLower() == "y");
        }

        private static void CreateOrder()
        {
            var (BeerOrderAmount, FoodOrderAmount, Confirmed) = GetCreateOrderInput();

            if (Confirmed)
            {
                try
                {
                    using var connection = new OdbcConnection(BuildConnectionString());
                    connection.Open();

                    using var insertOrderCommand = new OdbcCommand(
                        "INSERT INTO Z_DEVTOBER_ORDER_SYS_SQL.DEVTOBERORDER VALUES ( ?, UTCL_CURRENT(), ?, ? )", connection);

                    var uuid = GetGuidFromDb(connection);
                    insertOrderCommand.Parameters.AddWithValue(null, uuid.ToByteArray());
                    insertOrderCommand.Parameters.AddWithValue(null, BeerOrderAmount);
                    insertOrderCommand.Parameters.AddWithValue(null, FoodOrderAmount);

                    insertOrderCommand.ExecuteNonQuery();

                    Console.WriteLine($"Order with ID {uuid} created.");
                }
                catch (OdbcException exception)
                {
                    Console.Error.WriteLine($"ODBC error: {exception.Message}");
                    Environment.Exit(1);
                }
            }
        }

        private static (Guid OrderId, bool Confirmed) GetDeleteOrderInput()
        {
            Guid orderId;
            while (true)
            {
                var orderIdInput = ReadInput("Enter Order ID: ");
                if (Guid.TryParse(orderIdInput, out orderId))
                    break;
                Console.WriteLine("Invalid GUID. Please try again.");
            }
            var confirmedInput = ReadInput("Delete order? (y/n): ");
            return (orderId, confirmedInput.Equals("y", StringComparison.CurrentCultureIgnoreCase));
        }

        private static void DeleteOrder()
        {
            var (OrderId, Confirmed) = GetDeleteOrderInput();

            if (Confirmed)
            {
                try
                {
                    using var connection = new OdbcConnection(BuildConnectionString());
                    connection.Open();

                    using var deleteOrderCommand = new OdbcCommand(
                        "DELETE FROM Z_DEVTOBER_ORDER_SYS_SQL.DEVTOBERORDER WHERE DevtoberOrderId = ?", connection);

                    deleteOrderCommand.Parameters.AddWithValue(null, OrderId.ToByteArray());

                    if (deleteOrderCommand.ExecuteNonQuery() != 0)
                    {
                        Console.WriteLine($"Order with ID {OrderId} deleted.");
                    }
                    else
                    {
                        Console.WriteLine($"Order with ID {OrderId} not found.");
                    }
                }
                catch (OdbcException exception)
                {
                    Console.Error.WriteLine($"ODBC error: {exception.Message}");
                    Environment.Exit(1);
                }
            }
        }

        private static (int LowYear, int HighYear, int SumAdjustment) GetSummarizeResultInput()
        {
            int lowYear, highYear, sumAdjustment;
            while (true)
            {
                var lowYearInput = ReadInput("Enter Low Year Filter (e.g., 2024): ");
                if (int.TryParse(lowYearInput, out lowYear))
                    break;
                Console.WriteLine("Invalid number. Please try again.");
            }
            while (true)
            {
                var highYearInput = ReadInput("Enter High Year Filter (e.g., 2026): ");
                if (int.TryParse(highYearInput, out highYear))
                    break;
                Console.WriteLine("Invalid number. Please try again.");
            }
            while (true)
            {
                var sumAdjustmentInput = ReadInput("Enter Beer/Food Order Amount Adjustment: ");
                if (int.TryParse(sumAdjustmentInput, out sumAdjustment))
                    break;
                Console.WriteLine("Invalid number. Please try again.");
            }
            return (lowYear, highYear, sumAdjustment);
        }

        private static void SummarizeResult()
        {
            var (LowYear, HighYear, SumAdjustment) = GetSummarizeResultInput();

            try
            {
                using var connection = new OdbcConnection(BuildConnectionString());
                connection.Open();

                using var createLttCommand = new OdbcCommand(
                    "CREATE LOCAL TEMPORARY TABLE SYS_TEMP.TABLE_RANGES" +
                    " ( LOW INT4 NOT NULL, HIGH INT4 NOT NULL )" +
                    " ON COMMIT PRESERVE ROWS"
                , connection);

                createLttCommand.ExecuteNonQuery();

                using var insertLttCommand = new OdbcCommand(
                    "INSERT INTO SYS_TEMP.TABLE_RANGES VALUES ( ?, ? )", connection);

                insertLttCommand.Parameters.AddWithValue(null, LowYear);
                insertLttCommand.Parameters.AddWithValue(null, HighYear);

                insertLttCommand.ExecuteNonQuery();

                using var callResultProc = new OdbcCommand(
                    "CALL Z_DEVTOBER_ORDER_SYS_SQL.FilterDevtoberResult(" +
                    "it_year_filter => SYS_TEMP.TABLE_RANGES," +
                    "iv_sum_adjustment => ?," +
                    "et_result => RESULT," +
                    "et_result_filtered => RESULT )", connection);

                callResultProc.Parameters.AddWithValue(null, SumAdjustment);

                using var reader = callResultProc.ExecuteReader();

                Console.WriteLine("Unfiltered Results:");
                while (reader.Read())
                {
                    Console.WriteLine($"Year: {reader.GetInt32(0)}, Beer Sold: {reader.GetInt64(1)}, Food Sold: {reader.GetInt64(2)}");
                }

                if (reader.NextResult())
                {
                    Console.WriteLine("Filtered Results:");
                    while (reader.Read())
                    {
                        Console.WriteLine($"Year: {reader.GetInt32(0)}, Beer Sold: {reader.GetInt64(1)}, Food Sold: {reader.GetInt64(2)}");
                    }
                }
            }
            catch (OdbcException exception)
            {
                Console.Error.WriteLine($"ODBC error: {exception.Message}");
                Environment.Exit(1);
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

        private static string ReadConfigFromFile(string key)
        {
            var configFilePath = ".env";
            if (File.Exists(configFilePath))
            {
                var lines = File.ReadAllLines(configFilePath);
                foreach (var line in lines)
                {
                    var parts = line.Split('=', 2);
                    if (parts.Length == 2 && parts[0].Trim() == key)
                    {
                        return parts[1].Trim();
                    }
                }
            }
            return string.Empty;
        }
    }
}

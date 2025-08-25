# Show Views Example (C#)

This example demonstrates how to connect to an ABAP system using ODBC from a C# application and display available views. For more information, see the ODBC driver for ABAP documentation.

The ODBC driver for ABAP is only available as a 64-bit driver, which is why this project is configured to build and run as a 64-bit (x64) application. This is enforced by setting the `PlatformTarget` property in the `show_views.csproj` file.

## Prerequisites

- .NET 9.0 SDK or later
   - Older versions may work as well, but have not been tested.
- The `System.Data.Odbc` NuGet package (installed automatically if you build the project)
- An ODBC DSN configured for the ABAP ODBC driver
    - The ODBC driver for ABAP must be installed on your system
    - You need a valid DSN, username, and password for your ABAP system

## How to Run

1. Open a terminal and navigate to the `examples/c#/show_views` directory:
   ```
   cd examples/c#/show_views
   ```
2. Run the example:
   ```
   dotnet run
   ```
3. When prompted, enter your DSN, username, and password. The program will connect to the ABAP system and print the list of views.

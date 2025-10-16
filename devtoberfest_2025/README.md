# Devtoberfest OrderApp (C#)

This project was created as part of a demo for **Devtoberfest 2025**.

This project demonstrates how to interact with an ABAP system using ODBC from a C# console application. The OrderApp allows you to create, delete, and summarize orders stored in an ABAP backend. For a detailed walkthrough, please refer to the [recording](https://www.youtube.com/live/1oc1XnJMag8) of Devtoberfest 2025 session (second part).

## Prerequisites

- **.NET 9.0 SDK or later**  
  _Older versions may work as well, but have not been tested._

- **The `System.Data.Odbc` NuGet package**  
  _(Installed automatically when you build the project)_

- **An ODBC DSN configured for the ABAP ODBC driver**
    - The ODBC driver for ABAP must be installed on your system
    - The ODBC driver for ABAP is only available as a 64-bit driver, so this project is configured to build and run as a 64-bit (x64) application. This is enforced by setting the `PlatformTarget` property in the `OrderApp.csproj` file.
    - DSN, username, and password must be configured in a `.env` file in the project directory.

- **Required ABAP Artifacts**
    - The necessary ABAP artifacts (such as service definition, service binding, CDS entities and classes) must be created in your ABAP system before running this application. Please refer to the `ABAP` folder in this project for details on which artifacts are needed and how to create them.

## How to Run

1. Open a terminal and navigate to the `devtoberfest_2025/c#/OrderApp` directory:
2. Ensure your `.env` file is present and correctly configured.
3. Run the application

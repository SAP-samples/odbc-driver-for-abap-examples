# Examples for the ABAP ODBC driver
![REUSE status](https://api.reuse.software/badge/github.com/SAP-samples/odbc-driver-for-abap-examples)

## Description
This repository contains examples for using the _ODBC driver for ABAP_ in various programming languages.
Information on how to run the example programs and prerequisites can be found either as comments in the code or in `readme.md` files in the respective directories.

The folder `examples` contains the actual examples, most are sorted in subfolders for each programming language, the folder `unix` contains a configuration template for the `unixodbc` driver manager.

The folder `ABAP` contains a text representation of the ABAP objects required by the examples.
The contents is also released as the ABAP package `BASIS/SABP_MAIN/SABP/SABP_DBC2ABAP/SABP_DBC2ABAP_TEST`, but some objects might not be available in older releases.

## Requirements
All examples require the _ODBC driver for ABAP_ to be installed on the client.
It can be downloaded from the SAP Marketplace.

On Windows, the standard ODBC driver manager is used.
For Unix systems, the ODBC driver manager [unixodbc](https://www.unixodbc.org/) is required for almost all examples.
Other driver managers are not supported.

Some features showcased in the examples have minimum version requirements:

| Features | SAP system version | ODBC driver version |
| --- | --- | --- |
| `select` | 2021 FPS01 | 01.00.0000 |
| `call` | 2025 / 2502 | 01.00.0018 |
| `insert`, `delete` | 2025 / 2508 | 01.00.0021 |

## Download and Installation
The examples in this repository are provided as source code.
They can be obtained by cloning the repository or by downloading individual files via the browser.

## How to obtain support
[Create an issue](https://github.com/SAP-samples/odbc-driver-for-abap-examples/issues) in this repository if you find a bug or have questions about the content.

The official documentation for the _ODBC driver for ABAP_ exists in three flavours:
- [ABAP Platform](https://help.sap.com/docs/ABAP_PLATFORM_NEW/b5670aaaa2364a29935f40b16499972d/4082fe1b66164eeb8aa41192166526af.html?locale=en-US&version=latest)
- [SAP BTP](https://help.sap.com/docs/BTP/65de2977205c403bbc107264b8eccf4b/4082fe1b66164eeb8aa41192166526af.html?locale=en-US&version=Cloud)
- [S/4HANA Cloud](https://help.sap.com/docs/SAP_S4HANA_CLOUD/6aa39f1ac05441e5a23f484f31e477e7/4082fe1b66164eeb8aa41192166526af.html?locale=en-US&version=latest)

There is a number of SAP Technology blog posts discussing the various aspects of the _ODBC driver for ABAP_:
- [Consuming CDS View Entities Using ODBC-Based Client Tools](https://community.sap.com/t5/technology-blog-posts-by-sap/consuming-cds-view-entities-using-odbc-based-client-tools/ba-p/13509459)
- [Using the ODBC driver for ABAP on Linux](https://community.sap.com/t5/technology-blog-posts-by-sap/using-the-odbc-driver-for-abap-on-linux/ba-p/13513705)
- [SQL Queries on CDS objects Exposed as SQL Service](https://community.sap.com/t5/technology-blog-posts-by-sap/sql-queries-on-cds-objects-exposed-as-sql-service/ba-p/13548513)
- [Access CDS Objects Exposed as SQL Services Using Microsoft SQL Server](https://community.sap.com/t5/technology-blog-posts-by-sap/access-cds-objects-exposed-as-sql-services-using-microsoft-sql-server/ba-p/13561745)
- [Data Science with SAP S/4HANA – How to connect HANA-ML with Jupyter Notebooks (Python)](https://community.sap.com/t5/supply-chain-management-blog-posts-by-sap/data-science-with-sap-s-4hana-how-to-connect-hana-ml-with-jupyter-notebooks/ba-p/13578429)
- [Build an Azure Data Factory Pipeline with the ODBC Driver for ABAP](https://community.sap.com/t5/technology-blog-posts-by-sap/build-an-azure-data-factory-pipeline-with-the-odbc-driver-for-abap/ba-p/13612960)
- [Taking Data Federation to the Next Level: Accessing Remote ABAP CDS View Entities in SAP HANA Cloud](https://community.sap.com/t5/technology-blog-posts-by-sap/taking-data-federation-to-the-next-level-accessing-remote-abap-cds-view/ba-p/13635034)
- [Expose and Use ABAP-Managed Database Procedures (AMDPs) in SQL Services](https://community.sap.com/t5/technology-blog-posts-by-sap/expose-and-use-abap-managed-database-procedures-amdps-in-sql-services/ba-p/13992244)
- [Rust and ABAP ODBC](https://community.sap.com/t5/technology-blog-posts-by-sap/rust-and-abap-odbc/ba-p/14118190)

## Contributing
Please see our [guideline for contributions to SAP Open Source Software Projects](CONTRIBUTING.md).

## Code of Conduct
Please see the [SAP Open Source Code of Conduct](CODE_OF_CONDUCT.md).

## License
Copyright (c) 2025 SAP SE or an SAP affiliate company. All rights reserved. This project is licensed under the Apache Software License, version 2.0 except as noted otherwise in the [LICENSE](LICENSE) file.

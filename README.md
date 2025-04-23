# Frame Logger
A tool which capture packets in an inteligence manner. This can run in memory safe way.

# License

This project is dual-licensed under:

- [GPLv3](./LICENSE.gpl) for open-source use
- [Business Source License (BSL)](./LICENSE.bsl) for limited commercial use

For non-commercial, open-source use: you may use GPLv3.  
For production/commercial use before 2028: contact `rishithaminol@gmail.com`

⚠️ **Important:** This project uses a dual-license strategy: GPLv3 for open community use, and BSL 1.1 to delay production/commercial use until 2028. After that, all code becomes fully GPLv3. The project encourages community contributions and will always offer a free software pathway.

# Log categories
Currently there are two types of log categories.

* Logs which are specific to the tool
* Logs which are captured packets

# Guidelines

* If a variable is used by multiple functions, make sure to make them thread safe.

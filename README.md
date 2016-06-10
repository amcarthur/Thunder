# Thunder
A DLL injector made for educational purposes. Relies on the [Blackbone](https://github.com/DarthTon/Blackbone) project and [AsmJit](https://github.com/kobalicek/asmjit).
Will  eventually fully support x86 and x64 injection.

## Todo:
1. Finish the CLR injection routines.
2. Add much more subsystem validation for interoperability between the target process and the injector.
3. Add DLL image validation to make sure the DLL can be injected into the target process.
4. Add exception handling.
5. Support all the different injection features and options that Blackbone supports.
6. Build shared and static libraries for API usage.
7. Add an "auto injection" feature to wait for a specified process to launch.
8. I'm sure tons more.

This project is not ready or intended for use by anyone at the moment.

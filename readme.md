# mcode

mcode is a C utility that allows you to manipulate and convert binary files. It provides features like converting executable files into machine code, searching for binary sequences in files, and replacing binary sequences.

## Features

1. Create Machine Code: Converts an executable (.exe) file to a binary file in machine code.

2. Machine Code Mapping: Generates a hexadecimal representation of an executable file using xxd.

3. Apply Modifications: Converts a binary file (machine code) back to an executable.

4. Find Binary Sequence: Searches and decodes a binary sequence in a binary file.

5. Replace Binary Sequence: Replaces one binary sequence with another in a binary file.

# Windows

```powershell
./make.ps1 --build
mkdir C:\\mcode
cp build\mcode.exe C:\\mcode
```

# Linux

```powershell
make
make install
mcode --version
```
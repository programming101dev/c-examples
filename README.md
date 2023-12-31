# c-examples Repository Guide

Welcome to the `c-examples` repository. This guide will help you set up and run the provided scripts.

## **Table of Contents**

1. [Prerequisites](#prerequisites)
2. [Cloning the Repository](#cloning-the-repository)
3. [Setting Up the `CC` Environment Variable](#setting-up-the-cc-environment-variable)
4. [Running the `generate-build.sh` Script](#running-the-generate-buildsh-script)
5. [Executing the Generated `build.sh` Script](#executing-the-generated-buildsh-script)
6. [Conclusion](#conclusion)
7. [Cleanup](#cleaning-up-the-repository)
8. [Running the `cleanup.sh` Script](#running-the-cleanupsh-script)

## **Prerequisites**

- Git
- Bash
- Clang & GCC compilers

## **Cloning the Repository**

Clone the repository using the following command:

```bash
git clone https://github.com/programming101dev/c-examples.git
```

Navigate to the cloned directory:

```bash
cd c-examples
```

## **Setting up the CC Environment Variable**

To permanently set the CC environment variable for your user, you should modify your shell's configuration file.
Depending on the shell you're using, you should edit the appropriate configuration file:

For Bash, edit the ~/.bashrc file to include ```export CC=gcc``` or ```export CC=clang```.

```bash
nano ~/.bashrc
```

For Zsh, edit the .zshrc file to include ```export CC=gcc``` or ```export CC=clang```.

```bash
nano ~/.zshrc
```

For Csh, edit the ~/.cshrc file to include ```setenv CC gcc``` or ```setenv CC clang```.

```bash
nano ~/.bashrc
```

Save and close the file. Then, reload your shell configuration to apply the changes:

```bash
source ~/.bashrc
```

```bash
source ~/.zshrc
```

```bash
source ~/.cshrc
```

Now, the CC environment variable is permanently set to your chosen C compiler.

## **Running the generate-build.sh Script**

First, ensure the script is executable:

```bash
chmod +x generate-makefiles.sh
```

Now, execute the script:

```bash
./generate-makefiles.sh
```

This will check which compiler flags are supported on your system and subsequently generate a build.sh script tailored
to your system.

## **Executing the Generated build.sh Script**

Before running the generated script, make sure it's executable:

```bash
chmod +x run-makefiles.sh
```

Run the build.sh script:

```bash
./run-makefiles.sh
```

This script will compile the C files in the repository using the supported flags from the generate-build.sh script.

## **Conclusion**

You've now successfully cloned the c-examples repository, generated a build script tailored to your system, and compiled
the C examples using that script. If you encounter any issues or need further guidance, please refer to the repository's
main documentation or raise an issue on the GitHub repository.

## **Cleaning up the Repository**

To delete the binaries, use the cleanup script. This script:

- Searches for files with the extensions `.out`, `.so`, or `.dylib` in the current directory and its subdirectories.
- Deletes the found files and prints a message indicating the file has been deleted.
- If any file cannot be deleted, an error message is displayed, and the script terminates.
- On successful completion, a message `Deletion complete.` is displayed.

To run the cleanup script, navigate to the repository directory and execute it:

## **Running the cleanup.sh Script**

First, ensure the script is executable:

```bash
chmod +x cleanup_script_name.sh
```

Run the cleanup.sh script:

```bash
./cleanup.sh
```
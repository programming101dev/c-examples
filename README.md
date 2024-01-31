# c-examples Repository Guide

Welcome to the `c-examples` repository. This guide will help you set up and run the provided scripts.

## **Table of Contents**

1. [Cloning the Repository](#cloning-the-repository)
2. [Prerequisites](#prerequisites)
3. [Running the `change-compiler.sh` Script](#running-the-generate-buildsh-script)
4. [Running the `build.sh` Script](#executing-the-generated-buildsh-script)
5. [Cleanup](#cleaning-up-the-repository)
6. [Running the `cleanup.sh` Script](#running-the-cleanupsh-script)

## **Cloning the Repository**

Clone the repository using the following command:

```bash
git clone https://github.com/programming101dev/c-examples.git
```

Navigate to the cloned directory:

```bash
cd c-examples
```

## **Prerequisites**

- to ensure you have all of the required tools installed, run:
```bash
./check-env.sh
```

If you are missing tools follow these [instructions](https://docs.google.com/document/d/1ZPqlPD1mie5iwJ2XAcNGz7WeA86dTLerFXs9sAuwCco/edit?usp=drive_link).

## **Running the generate-build.sh Script**

First, ensure the script is executable:

```bash
chmod +x *.sh
```

Now, execute the script:

```bash
./change-compiler.sh -c <compiler>
```

This will check which compiler flags are supported on your system and subsequently generate a build.sh script tailored
to your system.

## **Executing the Generated build.sh Script**

Before running the generated script, make sure it's executable:

```bash
chmod +x build.sh
```

Run the build.sh script:

```bash
./build.sh
```

This script will compile the C files in the repository using the supported flags from the generate-build.sh script.

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
chmod +x cleanup.sh
```

Run the cleanup.sh script:

```bash
./cleanup.sh
```
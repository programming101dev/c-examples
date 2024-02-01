# c-examples Repository Guide

Welcome to the `c examples` repository. This guide will help you set up and run the provided scripts.

## **Table of Contents**

1. [Cloning the Repository](#cloning-the-repository)
2. [Prerequisites](#Prerequisites)
3. [Running the `change-compiler.sh` Script](#running-the-change-compilersh-script)
4. [Running the `build.sh` Script](#running-the-buildsh-script)

## **Cloning the Repository**

Clone the repository using the following command:

```bash
git clone https://github.com/programming101dev/c-examples.git
```

Navigate to the cloned directory:

```bash
cd c-examples
```

Ensure the scripts are executable:

```bash
chmod +x *.sh
```

## **Prerequisites**

- to ensure you have all of the required tools installed, run:
```bash
./check-env.sh
```

If you are missing tools follow these [instructions](https://docs.google.com/document/d/1ZPqlPD1mie5iwJ2XAcNGz7WeA86dTLerFXs9sAuwCco/edit?usp=drive_link).

## **Running the change-compiler.sh Script**

Tell make which compiler you want to use:

```bash
./change-compiler.sh -c <compiler>
```

To the see the list of possible compilers:

```bash
cat supported_c_compilers
```

## **Running the build.sh Script**

To build the program run:

```bash
./build.sh
```

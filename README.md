# c-examples Reference Snippets

This repository is a source-level reference archive for small native C/POSIX
operations. It is not the p101 curriculum or an acceptance suite: the
standalone playground tracks own the guided lessons, executable defects,
expected findings, and progress receipts.

The snippets intentionally use native interfaces so readers can compare them
with the corresponding p101 wrappers. They are not evidence that a p101
wrapper, failure path, or platform contract is correct.

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
cat supported_c_compilers.txt
```

## **Running the build.sh Script**

The historical examples are built only when generated per-example Makefiles
are present. A fresh clone contains the source archive, not those generated
Makefiles; use the playgrounds for a self-contained build-and-fix workflow.

```bash
./build.sh
```

All source files and the repository license are Apache-2.0 licensed.

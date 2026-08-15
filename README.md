# c-examples Reference Snippets

This repository is a source-level reference archive for small native C/POSIX
operations. It is not the p101 curriculum or an acceptance suite: the
standalone playground tracks own the guided lessons, executable defects,
expected findings, and progress receipts.

The snippets intentionally use native interfaces so readers can compare them
with the corresponding p101 wrappers. They are not evidence that a p101
wrapper, failure path, or platform contract is correct.

## **Cloning the Repository**

Clone the repository using the following command:

```bash
git clone https://github.com/programming101dev/c-examples.git
```

Navigate to the cloned directory:

```bash
cd c-examples
```

This is deliberately a source-reference archive rather than a build product.
The snippets have different runtime assumptions and some are intentionally
unsafe demonstrations, so the repository does not advertise a misleading
aggregate build command. Use the standalone playground tracks for governed
configure, build, test, and repair exercises.

All source files and the repository license are Apache-2.0 licensed.

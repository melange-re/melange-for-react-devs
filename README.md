# Melange for React Developers

Source code for the book [Melange for React
Developers](https://react-book.melange.re/).

## Quick Start

```shell
npm run init

# In separate terminals:
npm run watch
npm run serve
```

## Commands

All the build commands are defined in the `scripts` field of `package.json`.
This is completely optional, and other tools like `make` could be used.

You can see all available commands by running `npm run`. There are explanations
of each command in the `scriptsComments` field of the `package.json` file. Here
are a few of the most useful ones:

- `npm run init`: set up opam local switch and download OCaml, Melange and
JavaScript dependencies
- `npm run install-opam-npm`: install OCaml, Melange, and JavaScript
  dependencies
- `npm run watch`: watch the filesystem and have Melange rebuild on every
change
- `npm run serve`: serve the application with a local HTTP server

## Book

The files for the book associated with this repo are in the `docs` directory.
See `docs/README.md` for details.

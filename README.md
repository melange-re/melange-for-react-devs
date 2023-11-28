# Melange for React Developers

Source code for the book [Melange for React
Developers](https://react-book.melange.re/).

## Quick Start

```shell
make init

# In separate terminals:
make watch
make serve
```

When running `make init`, you may encounter an error like this:

```
[ERROR] Could not determine which packages to install for this switch:
  * Missing dependency:
    - melange >= 2.0.0
    no matching version
```

To address this, first run `opam update`, then rerun `make init`.

## Serving apps

By default, running `make serve` will serve the Counter app located in
`src/counter`. To run another app, you can `cd` into its directory and run
`make serve` there. For example, to run the Celsius Converter app:

```
cd src/celsius-converter-option
make serve
```

Another way is to specify the name of the app via the `app` environment variable
before running `make serve` in the root directory, e.g.

```
app=celsius-converter-option make serve
```

## Commands

You can see all available commands by running `make help` or just `make`. Here
are a few of the most useful ones:

- `make init`: set up opam local switch and download OCaml, Melange and
JavaScript dependencies
- `make install`: install OCaml, Melange, and JavaScript dependencies
- `make watch`: watch the filesystem and have Melange rebuild on every change
- `make serve`: Serve an application using a local HTTP server

## Book

The files for the book associated with this repo are in the `docs` directory.
See `docs/README.md` for details.

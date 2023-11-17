# Introduction to Dune

Depending on how you've been following along, you may have several components in
your project. Since these components don't have much in common with each other,
it makes sense to put them in separate, independent apps. To do that, we'll have
to spend a little time with [Dune](https://dune.build/). Dune is a build system
designed for OCaml projects, and it has many [useful
features](https://melange.re/v2.1.0/build-system/#features). For our purposes,
the feature of primary interest is its [built-in support for
Melange](https://dune.readthedocs.io/en/stable/melange.html).

## `dune-project` file

The `dune-project` file specifies metadata for a project, and should appear in
the root directory of your project. If you've been using the [starter
project](/installation/#download-and-run-the-starter-project), then you've been
using Dune this whole time and therefore already have a `dune-project` file:

<<< @/../dune-project{clj}

The line

```clj
(using melange 0.1)
```

is necessary because we have to manually enable the Melange extension for Dune
in order to use it. Note that the version of the Melange Dune extension is
independent of the version of Melange we're using.

Technically, `dune-project` accepts many more metadata fields, but it’s best to
keep it minimal. Other than `name`, it makes more sense to put the rest of your
project's metadata fields in your `.opam` file, which we'll cover
[later](/todo/).

`dune-project` files use
[S-expressions](https://en.wikipedia.org/wiki/S-expression), which might make
you think of the [Lisp programming
language](https://en.wikipedia.org/wiki/Lisp_(programming_language)). However,
S-expressions are just a convenient syntax for encoding structured data, and
Dune doesn't have the power of a full scripting language.

Each S-expression at the top level is a known as a *stanza*. All the possible
stanzas you can use in `dune-project` can be found in Dune’s [Stanza
Reference](https://dune.readthedocs.io/en/stable/dune-files.html#dune-project).

## `dune` files

Besides `dune-project`, Dune also looks at the `dune` files in our project.
Basically, `dune` files tell Dune about directories, executables, libraries,
tests, and anything else of interest. For example, here's the `dune` file inside
the root directory of your project:

<<< @/../dune{clj}

Like `dune-project`, a `dune` file consists of one or more *stanzas*. The only
stanza here is
[dirs](https://dune.readthedocs.io/en/stable/dune-files.html#dirs), which tells
Dune which directories to include in the build. Note that the stanzas accepted
in `dune` files are not the same as the ones accepted by `dune-project`. See all
possible `dune` stanzas in Dune's [Stanza
Reference](https://dune.readthedocs.io/en/stable/dune-files.html#dune).

## `melange.emit` stanza

Let's now take a look at the `src/dune` file:

<<< @/../src/counter/dune{clj}

The main stanza of interest for us is
[melange.emit](https://dune.readthedocs.io/en/stable/melange.html#melange-emit),
which tells Dune to turn our OCaml files into JavaScript files. The fields we
give to `melange.emit` here are `target`, `libraries`, `preprocess`, and
`module_systems`, which are ones that we need to use for pretty much every
Melange project.

A note about the `target` field: It is referenced in the `serve` npm script,
whose command is

```bash
webpack serve --open --mode development --entry ./_build/default/src/output/src/Index.js
```

If you change the value of `target` to, say, `stuff`, the value of the `--entry`
option must be changed to

```
./_build/default/src/stuff/src/Index.js
```

For more details about where JavaScript output files end up in the build
directory, see [JavaScript artifacts
layout](https://melange.re/v2.1.0/build-system/#javascript-artifacts-layout).

::: info
In this dune file, we're only concerned with building JavaScript to run in the
browser, but if we also wanted to build JavaScript to run on Node, we'd include
another `melange.emit` stanza specifically for that. See
[melange-opam-template](https://github.com/melange-re/melange-opam-template/blob/main/src/dune)
for an example of building for Node.
:::

## Counter app directory

Create a new directory in `src` called `counter`, which will contain a new app
that only renders the [Counter component](/counter/). Then make sure this new
directory contains:

- `Counter.re` from `src`
- `dune` file from `src`
- `Index.re` to render the app to the DOM

  <<< @/../src/counter/Index.re
- `Makefile` to serve the app using webpack dev server

  <<< @/../src/counter/Makefile{make}

After you've added those files, your `src/counter` directory should look like
this:

```
src/counter
├─ Counter.re
├─ dune
├─ Index.re
└─ Makefile
```

## Why Makefile

You might be wondering why we're using a `Makefile` instead of adding another
npm script. In OCaml projects, usage of [the Make build automation
tool](https://en.wikipedia.org/wiki/Make_(software)) is common, and this
includes Melange-related projects. So let's take a short detour to explain the
`Makefile` we just added.

## Anatomy of Makefile

In case you're not familiar with Make, here's a breakdown of
`src/counter/Makefile`:

<<< @/../src/counter/Makefile{make}

- There's only a single rule here, and its *target* is `serve`.
- `.PHONY: serve` is used to indicate that `serve` is a [phony
  target](https://makefiletutorial.com/#phony), meaning `serve` is not a file we
  want to generate but just a label for an action. In a simple `Makefile` like
  this one, declaring phony targets isn't strictly necessary.
- The line under `serve:` is the *recipe*, basically the command that will be
  executed when we run `make serve` at the command line.
  - `app=counter` sets the `app` environment variable to `counter`,
    which is the name of the directory this `Makefile` lives in
  - The [-C
    option](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html)
    tells Make to change the directory before reading the `Makefile`. In effect,
    it means we're running the `serve` rule in the root directory's `Makefile`.

## Root directory `Makefile`

Currently, the `serve` rule in root directory's `Makefile` can only run the app
in the `src` directory. We want it to be able to run the app in `src/counter`,
so we need to change it:

```make
.PHONY: serve
serve: ## Serve the application with a local HTTP server
	npx webpack serve --open --mode development --entry ./_build/default/src/$(app)/output/src/$(app)/Index.js
```

We basically copied the `serve` npm command from `package.json` to create the
recipe for this rule, but instead of a fixed value for the `--entry` option, we
use

```
./_build/default/src/$(app)/output/src/$(app)/Index.js
```

This means that the entry script served by webpack dev server depends on the
`app` environment variable, which is provided by `src/counter/Makefile`.

You're now ready to run the new Counter app you created! Go into the
`src/counter` directory and run

```
make serve
```

It should open a new tab in your default browser to <a
href="http://localhost:8080/" target="_blank" rel="noreferrer
noopener">http://localhost:8080/</a> and display your Counter component.

## Serving a default app

If you run `make serve` in the root directory, you'll get an error that looks
something like this:

```
ERROR in main
Module not found:
Error: Can't resolve './_build/default/src//output/src//Index.js' in '...'
```

That's because if `app` environment variable isn't provided, the value passed to
the `--entry` option is no longer a valid path. We can define a default value
for `app` by putting this near the top of `Makefile`:

```make
ifndef app
	app := counter
endif
```

Now if you run `make serve` in the root directory, it will serve the counter
app.

:::info
Feel to delete the `src/Index.re` and `src/dune` files. You won't need them
anymore for this or later chapters. You can also delete the `scripts` section of
your `package.json` file.
:::

Huzzah! You created a new `dune` file to build an app and a `Makefile` to
serve that app locally. In future chapters, we'll assume that you use the same
directory structure for each new app you build.

## Exercises

<b>1.</b> Repeat the steps we did for Counter and create a separate app for
Celsius Converter.

<b>2.</b> Delete `reason-react-ppx` from `src/counter/dune`'s `melange.emit`
stanza. What compiler errors do you get?

<b>3.</b> You might have noticed that every recipe in the Makefile is prefixed
by a tab. What happens if you replace the tab in front of the `serve` rule's
recipe with four spaces?

## Overview

- Dune is the build system we use to build Melange projects
- The `dune-project` file describes the metadata for your project, primarily:
  - The version of Dune you're using
  - The version of the Melange plugin for Dune you're using
  - The name of your project
- `dune` files describe things of interest to Dune, for example:
  - Which directories to include and which to exclude
  - Which directories contain code that should be transpiled to JavaScript,
    using the `melange.emit` stanza
- Make is a build automation tool heavily used in the OCaml world
  - `Makefile`s contain rules which can run commands based on the target you
    pass to the `make` command

## Solutions

<b>1.</b> Creating a separate app for Celsius Converter, with its own `dune`,
`Makefile`, and `Index.re` files, should look something like
[this](https://github.com/melange-re/melange-for-react-devs/tree/develop/src/celsius-converter-option).

<b>2.</b> Deleting `reason-react-ppx` from `src/counter/dune` will result in a
compilation error:

```
File "src/counter/Counter.re", line 5, characters 2-6:
5 |   <div
      ^^^^
Error: Unbound value div
```

That's because putting `reason-react-ppx` in the `preprocess/pps` field enables
JSX in `.re` files.

<b>3.</b> Replacing the tab in front of the `serve` rule's
recipe with four spaces will result in a `Makefile` error:

```
Makefile:42: *** missing separator.  Stop.
```

The line number where it expects the tab is 42. It is a very common mistake to
write spaces instead of tabs in a `Makefile`. Fortunately, most code editors
know that when you press the Tab key inside a `Makefile`, you mean to add a tab
character instead of space characters.

-----

[Source code for this
chapter](https://github.com/melange-re/melange-for-react-devs/tree/develop/src)
can be found in the [Melange for React Developers
repo](https://github.com/melange-re/melange-for-react-devs).


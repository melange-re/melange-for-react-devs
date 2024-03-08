# Sandwich Tests

One day over drinks at the local bar, Madame Jellobutter tearfully recounts to
you how her previous venture, Unicorn BBQ, failed in part because of a terribly
buggy and crashy website. It was so traumatizing that she still has nightmares
about that website to this day! After the conversation, you decide to write some
unit tests.

## Install `melange-fest` via opam

After asking around in the `#melange` channel of the [Reason Discord
chatroom](https://melange.re/v3.0.0/community.html#community), you get a
recommendation from user MonadicFanatic1984 to try out
[melange-fest](https://github.com/jchavarri/melange-fest), a library that allows
you to write tests in OCaml and run them in [Node test
runner](https://nodejs.org/api/test.html#test-runner). You decide to give it a
shot!

Run this from your project's root directory:

```shell
opam install melange-fest
```

Once that finishes, run `opam list` to see all the opam packages you've
installed in your *opam switch* (more on that later). Scroll down to the entry
for `melange-fest` and you should see something like this:

```
melange-fest    0.1.0   A minimal test framework for Melange using Node test runner
```

Open `melange-for-react-devs.opam` and add a corresponding entry in the
`depends` section for `melange-fest`:

```
depends: [
  "ocaml" {>= "5.1.1"}
  "reason" {>= "3.10.0"}
  "dune" {>= "3.8"}
  "melange" {>= "3.0.0-51"}
  "reason-react" {>= "0.14.0"}
  "reason-react-ppx" {>= "0.14.0"}
  "opam-check-npm-deps" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "ocaml-lsp-server" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "dot-merlin-reader" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "odoc" {with-doc}
  "melange-fest" {>= "0.1"} // [!code ++]
]
```

Note that the version number might not be `0.1` when you run `opam list`. If
it's different, just use that version number instead.

Now if we want to install this project on another computer, we don't need to
manually install `melange-fest`; it will be installed along with all the other
dependencies when we run `opam install . --deps-only`[^1] (this is already done for
you when you run `npm run init`).

::: warning

Currently, `melange-fest` isn't yet available in the [opam package
repository](https://opam.ocaml.org/packages/), but this will be fixed soon.

:::

## Opam switch

An [opam switch](https://ocaml.org/docs/opam-switch-introduction) is an isolated
OCaml environment, similar to Python's
[virtualenv](https://virtualenv.pypa.io/)[^2]. You can list all the opam switches on
your computer by running

```shell
opam switch
```

The output will look something like this:

```
#  switch                     compiler                   description
→  ~/melange-for-react-devs   ocaml-base-compiler.5.1.1  ~/melange-for-react-devs
   default                    ocaml.5.1.0                default

[NOTE] Current switch has been selected based on the current directory.
       The current global system switch is default.
```

As implied by `[NOTE]`, you don't need to manually set the opam switch for your
project, the switch is set based on your current working directory[^3].

## First test

Add a new file `src/order-confirmation/SandwichTests.re` and add a simple test
to it:

<<< SandwichTests.re#first-test

You should get this error:

```
File "docs/burger-tests/SandwichTests.re", line 2, characters 0-9:
2 | Fest.test("Item.Sandwich.toEmoji", () =>
    ^^^^^^^^^
Error: Unbound module Fest
```

In order to use a library, you must add it to the `libraries` field of the
`melange.emit` stanza in your `dune` file:

```clj{3}
(melange.emit
 (target output)
 (libraries reason-react melange-fest)
 (preprocess
  (pps melange.ppx reason-react-ppx))
 (module_systems es6)
 (runtime_deps
  (glob_files *.css)))
```

## Opening a module

We'll be adding several more tests to this file, and it'll quickly become
tiresome to have to write `Fest.test`, `Fest.expect`, and `Fest.equal` all the
time.

The easiest way to save typing is by using a module alias:

<<< SandwichTests.re#module-alias

However, because `SandwichTests` is a module for tests and `Fest` only contains
testing-related functions, it's reasonable to
[open](https://reasonml.github.io/docs/en/module#opening-modules) the `Fest`
module and make all its functions available inside the scope of the
`SandwichTests` module:

<<< SandwichTests.re#open-fest

::: warning

In most cases, `open` should not be used at the toplevel of a module. It usually
makes more sense to use a [local
open](https://reasonml.github.io/docs/en/module#local-opens) which makes
all the functions of the opened module available inside the scope of a function
or submodule.

:::

## Compile with `.mjs` extension

Now that your `SandwichTests.re` compiles, try running the outputted `.js` file
in Node:

```shell
node _build/default/src/order-confirmation/output/src/order-confirmation/SandwichTests.js
```

You'll probably see this error:

```
(node:68498) Warning: To load an ES module, set "type": "module" in the package.json or use the .mjs extension.
(Use `node --trace-warnings ...` to show where the warning was created)
/home/fhsu/work/melange-for-react-devs/_build/default/src/order-confirmation/output/src/order-confirmation/SandwichTests.js:3
import * as Assert from "assert";
^^^^^^

SyntaxError: Cannot use import statement outside a module
```

Recent versions of Node support [ECMAScript
modules](https://nodejs.org/api/esm.html#introduction), but Node uses [CommonJS
modules](https://nodejs.org/api/modules.html#modules-commonjs-modules) by
default. Node automatically treats `.mjs` files as ECMAScript modules, so you
can change the `module_systems` field of your `melange.emit` stanza to use the
`.mjs` extension:

```clj{6}
(melange.emit
 (target output)
 (libraries reason-react melange-fest)
 (preprocess
  (pps melange.ppx reason-react-ppx))
 (module_systems (es6 mjs))
 (runtime_deps
  (glob_files *.css)))
```

Now rebuild and run the test on the newly-generated `SandwichTests.mjs` file:

```shell
npm run build
node _build/default/src/order-confirmation/output/src/order-confirmation/SandwichTests.mjs
```

The tests should run successfully this time!

Since you've changed the extension of your generated JavaScript files to `.mjs`,
you must also change the reference in `src/order-confirmation/index.html`:

```html{7}
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Melange for React Devs</title>
    <script type="module" src="../../_build/default/src/order-confirmation/output/src/order-confirmation/Index.mjs"></script>
  </head>
  <body>
    <div id="root"></div>
  </body>
</html>
```

## Add `test` npm script

To save ourselves from having to repeatedly type the commands to rebuild the
project and run the test, add a new npm script to `package.json`:

```json
"test": "npm run build && node _build/default/src/order-confirmation/output/src/order-confirmation/SandwichTests.mjs"
```

## Test `Item.Sandwich.toPrice`

Let's add a test for `Item.Sandwich.toPrice`. However, in its current form, it's
not very easy to test since it can return different values depending on what the
date is. So first we must make the function pure, i.e. make it free from side
effects. The easiest way to do so is by adding a `date` argument:

<<< Item.re#to-price-with-date{1}

To quiet the compiler, you must also update `Item.toPrice` accordingly:

<<< Item.re#to-price{3}

Now you can use the
[Fest.deepEqual](https://ahrefs.github.io/melange-fest/reason/Fest/index.html#val-deepEqual)
function to write the test for `Item.Sandwich.toPrice`:

<<< SandwichTests.re#test-to-price

## Punning

There are two things to note about this line:

```reason
sandwiches |> Js.Array.map(~f=Item.Sandwich.toPrice(~date)),
```

1. We use [punning](https://reasonml.github.io/docs/en/jsx#punning) to shorten
`~date=date` to just `~date`.
1. We use [partial application](/celsius-converter-exception/#partial-application) to avoid explicitly creating an anonymous
   function for the callback to `Js.Array.map`.

If we didn't use punning or partial application, the line would look like this:

```reason
sandwiches
|> Js.Array.map(~f=item => Item.Sandwich.toPrice(~date=date, item)),
```

---

Sugoi! You now have a module for testing sandwich-related logic. In the next
chapter, we'll see how to integrate your tests with the Dune build system.

## Overview

- An opam switch is an isolated OCaml environment
- Use `opam install` to install third-party libraries into the current opam
  switch
- After you install a library, you should:
  - Add it to your `.opam` file so that it will be installed with all your other
    dependencies the next time you run `opam install . --deps-only`
  - Add it to the `libraries` field of your `melange.emit` stanza so your code
    can use it
- `melange-fest` is a testing library that allows you write tests in OCaml and
  run them in Node test runner
- You can `open` a module to make all its functions available in the current
  scope
- Set the value of `module_systems` to `(es6 mjs)` to make Melange generate
  `.mjs` files that are treated by Node as ECMASCript modules

## Exercises

<b>1.</b> Test all possible outputs for `Item.Sandwich.toEmoji` using
`Fest.deepStrictEqual` (see [`node:assert`
docs](https://nodejs.org/api/assert.html) if you're not familiar with this
function).

::: details Hint 1

Create an array of sandwiches:

<<< SandwichTests.re#array-of-sandwiches

:::

::: details Hint 2

Use `Js.Array.map` and pass two arrays of strings to `Fest.deepStrictEqual`.

:::

::: details Solution

A comprehensive test of `Item.Sandwich.toEmoji` might look like this:

<<< SandwichTests.re#test-to-emoji

:::

<b>2.</b> Add a new file for `BurgerTests.re` containing this code:

<<< @/../src/sandwich-tests/BurgerTests.re

Add a new `rule` stanza in your `dune` file so that this test can be run via
`npm run test`.

::: details Solution

The `rule` stanza for `BurgerTests.re` is very similar to the existing
rule we have for `SandwichTests.re`:

```clj
(rule
 (alias runtest)
 (deps
  Item.re
  (:input ./output/src/order-confirmation/BurgerTests.mjs))
 (action
  (run time node %{input})))
```

:::

<b>3.</b> Refactor `Item.Sandwich.toPrice` to accept a `Js.Date.t`:

<<< Item.re#to-price

Fix the resulting compilation error in `Item.toPrice` and add a unit test to
check that the date-dependent logic for Turducken sandwiches works as expected.

::: details Hint

Use
[Js.Date.makeWithYMD](https://melange.re/v3.0.0/api/re/melange/Js/Date/#val-makeWithYMD)
and `Js.Array.map` to generate a whole week's worth of dates. Here's a [relevant
playground
example](https://melange.re/v3.0.0/playground/?language=Reason&code=bGV0IGRhdGVzID0KICBbfDEuLCAyLiwgMy4sIDQuLCA1LiwgNi4sIDcufF0KICB8PiBKcy5BcnJheS5tYXAofmY9ZGF0ZSA9PgogICAgICAgSnMuRGF0ZS5tYWtlV2l0aFlNRCh%2BeWVhcj0yMDI0Liwgfm1vbnRoPTAuLCB%2BZGF0ZSkKICAgICApOwoKZGF0ZXMKfD4gSnMuQXJyYXkuZm9yRWFjaCh%2BZj1kYXRlID0%2BCiAgICAgSnMubG9nMihKcy5EYXRlLmdldERheShkYXRlKSwgSnMuRGF0ZS50b1N0cmluZyhkYXRlKSkKICAgKTsK&live=off).

:::

::: details Solution

A comprehensive test of `Item.Sandwich.toPrice` for Turducken
sandwiches might look like this:

<<< SandwichTests.re#test-to-price

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/sandwich-tests/)
and [demo](https://react-book.melange.re/demo/src/sandwich-tests/) for this chapter.

-----

[^1]: The `--deps-only` option tells `opam install` to only install the
    dependencies and not the package. When you're developing an application and
    not a library, there is no package associated with your project.

[^2]: In this book, we only use opam's [local
    switches](https://opam.ocaml.org/blog/opam-local-switches/), which are in
    practice similar to Node project directories containing a `package.json`
    file. But in general, an opam switch doesn't have to be associated with a
    project directory.

[^3]: A shell hook is responsible for setting the current opam switch depending
    on what directory you're in. The shell hook is typically installed when you
    run `opam init`. Just respond with `y` when it asks

    ```
    Do you want opam to modify ~/.profile?
    ```

    Note that it may ask to modify `~/.zshrc` or some other file; the name of
    the file is system-dependent.

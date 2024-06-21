# Sandwich Tests

One day over drinks at the local bar, Madame Jellobutter tearfully recounts to
you how her previous venture, Unicorn BBQ, failed in part because of a terribly
buggy and crashy website. It was so traumatizing that she still has nightmares
about that website to this day! After the conversation, you decide to write some
unit tests.

## Install `melange-fest` via opam

After asking around in the `#melange` channel of the [Reason Discord
chatroom](https://melange.re/v4.0.0/community.html#community), you get a
recommendation from user MonadicFanatic1984 to try out
[melange-fest](https://github.com/ahrefs/melange-fest), a library that allows
you to write tests in OCaml and run them in [Node test
runner](https://nodejs.org/api/test.html#test-runner). You decide to give it a
shot!

Run this from your project's root directory:

```shell
opam install melange-fest
```

Once that finishes, run

```shell
opam list
```

to see all the packages you've installed in your *opam switch* (more on that
later). Scroll down to the entry for `melange-fest` and you should see something
like this:

```
melange-fest    0.1.0   A minimal test framework for Melange
```

Open `melange-for-react-devs.opam` and add a corresponding entry in the
`depends` section for `melange-fest`:

```
depends: [
  "ocaml" {>= "5.1.1"}
  "reason" {>= "3.10.0"}
  "dune" {>= "3.8"}
  "melange" {>= "4.0.0-51"}
  "reason-react" {>= "0.14.0"}
  "reason-react-ppx" {>= "0.14.0"}
  "melange-fest" {>= "0.1.0"} // [!code ++]
  "opam-check-npm-deps" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "ocaml-lsp-server" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "dot-merlin-reader" {with-test} # todo: use with-dev-setup once opam 2.2 is out
  "odoc" {with-doc}
]
```

Note that the version number might not be `0.1.0` when you run `opam list`. If
it's different, just use that version number instead.

Now if we want to install this project on another computer, we don't need to
manually install `melange-fest`; it will be installed along with all the other
dependencies when we run `opam install . --deps-only`[^1] (this is already done
for you when you run `npm run init`).

## Opam switch

An [opam switch](https://ocaml.org/docs/opam-switch-introduction) is an isolated
OCaml environment. In this book, we only use [local
switches](https://opam.ocaml.org/blog/opam-local-switches/), which are similar
to Node project directories[^2]. You can list all the opam switches on your
computer by running

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
File "docs/order-confirmation/SandwichTests.re", line 2, characters 0-9:
2 | Fest.test("Item.Sandwich.toEmoji", () =>
    ^^^^^^^^^
Error: Unbound module Fest
```

In order to use a library, you must add it to the `libraries` field of the
`melange.emit` stanza in your `dune` file:

```dune
(melange.emit
 (target output)
 (libraries reason-react) // [!code --]
 (libraries reason-react melange-fest) // [!code ++]
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

```dune
(melange.emit
 (target output)
 (libraries reason-react melange-fest)
 (preprocess
  (pps melange.ppx reason-react-ppx))
 (module_systems es6)  // [!code --]
 (module_systems (es6 mjs))  // [!code ++]
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

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Melange for React Devs</title>
    <script type="module" src="../../_build/default/src/order-confirmation/output/src/order-confirmation/Index.js"></script> // [!code --]
    <script type="module" src="../../_build/default/src/order-confirmation/output/src/order-confirmation/Index.mjs"></script> // [!code ++]
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

In the next chapter, we'll see a way to shorten this script.

## Test `Item.Sandwich.toPrice`

Let's add a test for `Item.Sandwich.toPrice`. However, in its current form, it's
not testable since it's a nondeterministic function which can return different
values depending on what the date is. So first we must refactor it into a [pure
function](https://en.wikipedia.org/wiki/Pure_function), i.e. make it free from
side effects. The easiest way to do so is by adding a `date` argument:

<<< Item.re#to-price-with-date{1}

To quiet the compiler, you must also update `Item.toPrice` accordingly:

<<< Item.re#to-price{3}

Now you can use the
[Fest.deepEqual](https://ahrefs.github.io/melange-fest/reason/Fest/index.html#val-deepEqual)
function to write the test for `Item.Sandwich.toPrice`:

<<< SandwichTests.re#test-to-price

Here we create an array of `Item.Sandwich.t`, tranform it to an array of prices,
then compare that array with an array of expected prices.

## Punning for function arguments

Note that the following chunk of code uses
[punning](https://reasonml.github.io/docs/en/jsx#punning):

```reason{4}
let date = Js.Date.makeWithYMD(~year=2024., ~month=1., ~date=14.);

sandwiches
|> Js.Array.map(~f=item => Item.Sandwich.toPrice(~date, item))
```

Punning means that `~date=date` gets shortened to just `~date`.

## Type inference

It's actually not necessary to create a `sandwiches` variable, we can feed the
array directly to `Js.Array.map`:

<<< SandwichTests.re#test-to-price-type-inference

The OCaml compiler can infer that `[|Portabello, Ham, Unicorn, Turducken|]` is
of type `array(Item.Sandwich.t)` because

```reason
Js.Array.map(~f=item => Item.Sandwich.toPrice(~date, item))
```

is a function that only accepts an argument of the type
`array(Item.Sandwich.t)`.

---

Sugoi! You now have a module for testing sandwich-related logic. In the next
chapter, we'll see how to integrate your tests with the Dune build system.

## Overview

- An opam switch is an isolated OCaml environment
- Use `opam list` to see all the packages installed in your current opam switch
- Use `opam install` to install third-party packages into the current opam
  switch
- After you install a package, you should:
  - Add it to your `.opam` file so that it will be installed with all your other
    dependencies the next time you run `opam install . --deps-only`
  - Add the corresponding library to the `libraries` field of your
    `melange.emit` stanza so your code can use it
- [melange-fest](https://github.com/ahrefs/melange-fest) is a testing library
  that allows you write tests in OCaml and run them in Node test runner
- You can `open` a module to make all its functions available in the current
  scope, but you should do this sparingly
- To generate `.mjs` files that are treated by Node as ECMASCript modules, set
  the `melange.emit` stanza's `module_systems` field to `(es6 mjs)`
- Punning shortens function invocations by transforming `~foo=foo` into just
  `foo`.
- Sometimes you can use a value without any type annotation because the compiler
  can infer the type based on the type signature of the function it's fed into

## Exercises

<b>1.</b> Use
[Fest.deepEqual](https://ahrefs.github.io/melange-fest/reason/Fest/index.html#val-deepEqual)
to improve the existing test for `Item.Sandwich.toEmoji` by testing for all
possible outputs.

::: details Solution

<<< SandwichTests.re#test-to-emoji

Note the use of [partial
application](https://reasonml.github.io/docs/en/function#partial-application) in
the callback to `Js.Array.map`.

:::

<b>2.</b> Write a new unit test for `Item.Sandwich.toPrice` that checks the
date-dependent logic for Turducken sandwiches.

::: details Hint

Use
[Js.Date.makeWithYMD](https://melange.re/v4.0.0/api/re/melange/Js/Date/#val-makeWithYMD)
and `Js.Array.map` to generate a whole week's worth of dates. Here's a [relevant
playground
example](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGRhdGVzID0KICBbfDEuLCAyLiwgMy4sIDQuLCA1LiwgNi4sIDcufF0KICB8PiBKcy5BcnJheS5tYXAofmY9ZGF0ZSA9PgogICAgICAgSnMuRGF0ZS5tYWtlV2l0aFlNRCh%2BeWVhcj0yMDI0Liwgfm1vbnRoPTAuLCB%2BZGF0ZSkKICAgICApOwoKZGF0ZXMKfD4gSnMuQXJyYXkuZm9yRWFjaCh%2BZj1kYXRlID0%2BCiAgICAgSnMubG9nMihKcy5EYXRlLmdldERheShkYXRlKSwgSnMuRGF0ZS50b1N0cmluZyhkYXRlKSkKICAgKTsK&live=off).

:::

::: details Solution

<<< SandwichTests.re#test-to-price-turducken

:::

<b>3.</b> Refactor the `Item.Sandwich.toPrice` test using punning and partial
application:

```reason
test("Item.Sandwich.toPrice", () => {
  /* Write your code here; don't change the rest of the function */

  expect
  |> deepEqual(
       [|Portabello, Ham, Unicorn, Turducken|] |> Js.Array.map(~f),
       [|10., 10., 80., 20.|],
     );
});
```

::: details Hint

Add a new function `f` with the type signature `Item.Sandwich.t => float`.

:::

::: details Solution

<<< SandwichTests.re#test-to-price-exercise-solution

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/sandwich-tests/)
and [demo](https://react-book.melange.re/demo/src/sandwich-tests/) for this chapter.

-----

[^1]: The `--deps-only` option tells `opam install` to only install the
    dependencies and not the package. When you're developing an application and
    not a library, there is no package associated with your project.

[^2]: This is how Node project directories correspond to opam local switches:
    - `packages.json` -> `.opam` file
    - `node_modules` directory -> `_opam` directory

[^3]: A shell hook is responsible for setting the current opam switch based on
    the directory you are `cd`-ing into. The shell hook is typically installed
    when you run `opam init`. Just respond with `y` when it asks

    ```
    Do you want opam to modify ~/.profile?
    ```

    Note that it may ask to modify `~/.zshrc` or some other file; the name of
    the file is system-dependent.

# Cram Tests

Now that you have a set of unit tests for sandwich-related logic, you realize it
would be nice to have some unit tests for burger-related logic as well. But
adding a new command to your `test` npm script doesn't feel like The OCaml Way,
so you hop on the [Reason Discord
chatroom](https://melange.re/v3.0.0/community.html#community) to ask for advice.
MonadicFanatic1984 once again comes to your aid and points you toward *cram
tests*.

## Cram tests

A [cram test](https://dune.readthedocs.io/en/stable/reference/cram.html) is
essentially a test that runs a command inside a sandbox, then compares the
output with some expected output. Dune has built-in support for cram tests,
which means:

- You can specify dependencies for a cram test inside your `dune` file
- Cram tests are only run when their dependencies change
- When you update cram tests, causing their output to change, you can easily
  *promote* the latest output to become the new expected output
- You can run all tests using a single command that doesn't need to refer to
  individual tests

Before you can write your first cram test, you need to enable cram tests by
adding this to your `dune-project` file:

```dune
; Enable cram tests
(cram enable)
```

## `cram` stanza

To add dependencies for your cram tests, you need to add a [cram
stanza](https://dune.readthedocs.io/en/stable/dune-files.html#cram-stanza) to
`src/order-confirmation/dune`:

```dune
(cram
 (deps ./output/src/order-confirmation/SandwichTests.mjs))
```

This configuration tells Dune two things:

- Copy `./output/src/order-confirmation/SandwichTests.mjs` into the sandbox
  directory
- Whenever `SandwichTests.re` (the immediate dependency for
  `./output/src/order-confirmation/SandwichTests.mjs`) changes, rebuild and
  re-run all the cram tests defined in the same directory as this `dune` file

Note that we cannot use the same path to `SandwichTests.mjs` as we would if we
were running this from the root directory of our project, because cram tests are
run inside a sandbox directory (more on that later).

## Add `tests.t` file

Cram tests are specified inside `.t` files. Add a new file
`src/order-confirmation/tests.t`:

```cram
Sandwich tests
  $ node ./output/src/order-confirmation/SandwichTests.mjs
```

Note that this is the same path we specified inside the `cram/deps` field.

To run tests, execute

```shell
dune build @runtest
```

Here,
[@runtest](https://dune.readthedocs.io/en/stable/reference/aliases.html#runtest)
is a built-in *alias*. The above command builds whatever is needed by the tests
and then runs the tests.

## Aliases

A Dune [alias](https://dune.readthedocs.io/en/stable/reference/aliases.html) is
a virtual build target that other build targets can attach to. You can refer to
aliases by name in build commands (like the one above), to build a specific
subset of your build targets. In commands, we need to put `@` in front of the
alias name to distinguish it from file names. Therefore when we refer to an
alias, we put an `@` in front, e.g. `@runtest`, `@melange`, etc.

::: tip

Cram tests are automatically attached to the `@runtest` alias, and
`melange.emit` stanzas are automatically attached to the `@melange` alias.

:::

## Promotion

After running `dune build @runtest`, you should be able to see the
output from the cram test you just added[^1]. However, this doesn't count as a
passing cram test, because this output can't be compared to the expected output.
Dune has a nice feature that lets you
[promote](https://dune.readthedocs.io/en/stable/concepts/promotion.html#promotion)
the latest output of a cram test to become the expected output:

```shell
dune promote
```

The above command updates your `src/order-confirmation/tests.t` cram test file,
which now looks something like this:

<<< tests{cram}

The part underneath the command is the expected output of the command.

## Add `npm` scripts

Instead of typing `dune` commands over and over, let's add the relevant `npm`
scripts to your `package.json` file:

```json
"test": "npm run build -- @runtest",
"test:watch": "npm run build -- @runtest --watch",
"promote": "npm run dune -- promote"
```

Since JSON doesn't support comments, you can optionally add some lines to the
`scriptsComments` section of `package.json` to explain what each command does:

```json
"test": "# Run the tests",
"test:watch": "# Watch files and re-run tests",
"promote": "# Promote most recent output to expected output"
```

Run `npm run test` to check that it works.

## Sanitize cram test output

The cram test still doesn't succeed, but now it's because the most recent output
and the expected output don't match:

<<< diff{diff}

The problem is that the recorded durations for individual unit tests is differs
on each test run. Therefore we must
[sanitize](https://dune.readthedocs.io/en/stable/tests.html#test-output-sanitation)
the test output, i.e. remove the parts that are non-deterministic. Update your
cram test command:

```shell
$ node ./output/src/order-confirmation/SandwichTests.mjs | sed '/duration_ms/d'
```

Now the output of the command is piped to `sed '/duration_ms/d'`, which removes
all the lines containing the string `duration_ms`.

Follow these steps to achieve your first successful cram test run:

1. Run `npm run test`
1. It fails because the expected output still contains the `duration_ms` lines
1. Run `npm run promote` to promote the latest output to the expected output
1. Run `npm run test`
1. You should see no test output

No test output means it succeeded. Somewhat unintuitively, when cram tests
succeed, they remain silent![^2]

## Sandbox

All cram tests execute in a
[sandbox](https://dune.readthedocs.io/en/stable/tests.html#sandboxing). What
goes into the sandbox directory is determined by the value of the `cram/deps`
field in your `dune` file. Let's write an explorative cram test that allows us
to peek inside the sandbox. First, install the [tree-node-cli
package](https://www.npmjs.com/package/tree-node-cli):

```shell
npm install --save-dev tree-node-cli
```

Then add a new cram test to `src/order-confirmation/tests.t`:

```cram
See all the files inside the sandbox
  $ npx tree
```

Now when you run `npm run test`, you'll see some output like this:

```diff
@@ -24,3 +24,8 @@ Sandwich tests

 See all the files inside the sandbox
   $ npx tree
+  cram-tests
+  └── output
+      └── src
+          └── cram-tests
+              └── SandwichTests.mjs
```

The sandbox directory only contains a single file
`output/src/cram-tests/SandwichTests.mjs`. Add another explorative cram test:

```cram
Show detail of SandwichTests.mjs
  $ ls -la ./output/src/cram-tests/SandwichTests.mjs
```

The output should now look like this:

```diff
@@ -24,6 +24,12 @@ Sandwich tests

 See all the files inside the sandbox
   $ npx tree
+  cram-tests
+  └── output
+      └── src
+          └── cram-tests
+              └── SandwichTests.mjs

 Show detail of SandwichTests.mjs
   $ ls -la ./output/src/cram-tests/SandwichTests.mjs
+  lrwxr-xr-x  1 fhsu  staff  86 Mar 16 22:09 ./output/src/cram-tests/SandwichTests.mjs -> ../../../../../../../../default/src/cram-tests/output/src/cram-tests/SandwichTests.mjs
```

From this, you can see that `./output/src/cram-tests/SandwichTests.mjs` is
actually a symbolic link which links to the real file inside your build
directory, specifically this file:

```
_build/default/src/order-confirmation/output/src/order-confirmation/SandwichTests.mjs
```

`SandwichTests.mjs` being a symbolic link explains why you can run the test
successfully even when the rest of the `.mjs` files aren't present in the
sandbox directory.

## Better dependencies

There's a problem with the cram test dependencies. The cram test is re-run if
`SandwichTests.re` changes, but if `Item.re`, which `SandwichTests.re` depends
on, were to change, it wouldn't trigger a test re-run. To see this, make the
following change to `Item.Sandwich.toEmoji`:

```reason
let toEmoji = t =>
  Printf.sprintf(
    {js|🥪(%s)|js},
    switch (t) {
    | Portabello => {js|🍄|js}
    | Ham => {js|🐷|js}
    | Unicorn => {js|🦄|js}
    | Turducken => {js|🦃🦆🐓|js} // [!code --]
    | Turducken => {js|🦃🦆🐓💩|js} // [!code ++]
    },
  );
```

After running `npm run test`, you would expect a failing test, but that doesn't
happen. You have to add `(alias melange)` to `cram/deps`:

```dune
(cram
 (deps
  (alias melange)
  ./output/src/cram-tests/SandwichTests.mjs))
```

This adds the `Item.re` to the sandbox, but it doesn't cause the test to fail.
The proper fix is to instead add `(alias melange)` to `cram/deps`:

```dune
(cram
 (deps
  (alias melange)
  ./output/src/cram-tests/SandwichTests.mjs))
```

Now the test finally fails. The presence of `(alias melange)` means that all
build targets attached to the `@melange` alias are now dependencies for the cram
tests in this directory.

## `expand_aliases_in_sandbox` stanza

If you kept the `$ npx tree` cram test around, you'll observe that even though
all build targets attached to `@melange` are dependencies, they aren't copied
into the sandbox directory. You can change that behavior by adding the
[expand_aliases_in_sandbox
stanza](https://dune.readthedocs.io/en/stable/dune-files.html#stanza-expand_aliases_in_sandbox)
to your `dune-project` file:

```dune
; Copy all build targets for an alias into the sandbox

(expand_aliases_in_sandbox)
```

Run `npm run test` again and you'll see that a lot of files have been copied
over to the sandbox directory. Since `SandwichTests.mjs` is a `@melange` build
target too, it actually no longer needs to be explicitly included in
`cram/deps`:

```
(cram
 (deps
  (alias melange)))
```

Fix the bug in `Item.re` and re-run `npm run test`. Your cram test should pass
once more.

::: warning

Using
[expand_aliases_in_sandbox](https://dune.readthedocs.io/en/stable/dune-files.html#stanza-expand_aliases_in_sandbox)
is more convenient, but it may noticeably impact cram test performance. If you
feel your cram tests are too slow, you should remove it.

:::

---

Fantastico! You've set up Dune cram tests to streamline the testing process. In
the next chapter, we'll add logic for promotional discounts.

## Overview

- A cram test runs a command inside a sandbox and compares the output of that
  command with some expected output
- Dune has extensive support for cram tests, including the ability to promote
  the latest output to become the expected output
- A `cram` stanza is used to specify dependencies for your cram tests
- Cram test files have the `.t` extension
- Aliases are virtual build targets that other build targets can be attached to
- Cram tests that produce nondeterministic output must have that be sanitized
- Cram tests execute in a sandbox
- For Melange projects, `(alias melange)` is generally the best dependency for
  your cram tests
- The `expand_aliases_in_sandbox` allows you to avoid having to put generated
  `.mjs` files in your cram test dependencies, at the cost of having slower cram
  tests

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/cram-tests/)
and [demo](https://react-book.melange.re/demo/src/cram-tests/) for this chapter.

-----

[^1]: You might notice that the output from the Node test runner looks different
    when run inside a cram test. That's because Note test runner uses different
    [test reporters](https://nodejs.org/api/test.html#test-reporters) depending
    on whether you run it directly or through another process.

[^2]: A successful cram test won't print any output from the test, but you may
    see some logging that indicates that build targets were produced, e.g.

    ```shell
    refmt src/cram-tests/Item.re.ml
    ppx src/cram-tests/Item.re.pp.ml
    ocamldep src/cram-tests/.output.mobjs/melange__Item.impl.d
    melc src/cram-tests/.output.mobjs/melange/melange__Item.{cmi,cmj,cmt}
    melc src/cram-tests/output/src/cram-tests/Item.mjs
    melc src/cram-tests/.output.mobjs/melange/melange__SandwichTests.{cmi,cmj,cmt}
    melc src/cram-tests/.output.mobjs/melange/melange__Order.{cmi,cmj,cmt}
    melc src/cram-tests/.output.mobjs/melange/melange__Index.{cmi,cmj,cmt}
    ```

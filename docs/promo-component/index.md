# Promo Component

Madame Jellobutter has been pretty impressed with your work so far, so she sends
you on an all-expenses paid trip to an [OCaml
Hackathon](https://fun-ocaml.com/). You learn a lot, make several new friends,
and you even meet MonadicFanatic1984 in person![^1] You're extremely psyched to
start applying the things you learn during the hackathon.

## `RR` utility module

While having lunch during the hackathon, fellow attendee FunctorPunk[^2]
suggests that you create a dedicated module for your ReasonReact-related helper
functions. Add a new file `RR.re`:

<<< RR.re#initial-functions

These are all functions we used before, but now they're in one handy place, and
we added documentation comments to them. Note that you can now delete the
`Format` module, since its `currency` function is now in `RR`.

## Documentation comment markup language

In a couple of the documentation comments, we used square brackets (`[]`):

```text
/** Alias for [React.string] */
```

Whatever is inside the brackets gets rendered in a monospace source code font.
This is one example of the [markup
language](https://v2.ocaml.org/releases/5.0/manual/ocamldoc.html#ss:ocamldoc-formatting)
used inside documentation comments.

## `Promo` component

Add new file `Promo.re`:

<<< Promo.re#first-version

For now, it just shows an input that allows the user to enter a promo code.

## `React.useReducer`

Another neat trick you learn from FunctorPunk is that you can substitute
[React.useState](https://reasonml.github.io/reason-react/docs/en/usestate-hook)[^3]
with
[React.useReducer](https://reasonml.github.io/reason-react/docs/en/usereducer-hook):

<<< Promo.re#use-reducer{3-4,9}

The advantage of this is that you can update state by passing a value instead of
a callback. To make it even nicer, we can also rename `dispatch` to `setCode`
and use function chaining inside the `onChange` callback:

<<< Promo.re#set-code{3-4,9}

## `RR.useStateValue` helper function

We can add a new helper function `RR.useStateValue` that allows us to more
easily use this pattern:

<<< RR.re#use-state-value

The state setup code in `Promo` then becomes very simple:

```reason
let (code, setCode) = // [!code --]
    React.useReducer((_state, newState) => newState, ""); // [!code --]
let (code, setCode) = RR.useStateValue(""); // [!code ++]
```

## Add `Promo` to `Index`

The `Promo` component isn't finished yet, but let's add it to `Index` so that we
can see how it looks as we're building it:

<<< Index.re#make{4-5}

We know that, sooner or later, we must pass `items` and `date` to `Promo`, so we
just do it now. However, this breaks the build because `Promo` doesn't yet
define those props.

## Temporarily ignore props

We can quickfix it by adding the `~items` and `~date` props to `Promo.make` and
then immediately `ignore`ing them in the body of the function:

<<< Promo.re#ignore

A more concise way to temporarily ignore props is to use the `as` keyword in the
function argument list:

```reason
[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t) => {
```

Here we rename `~items` to `_`, effectively ignoring it, and also do the same
for `~date`. Note that this renaming only extends to the scope of the function
itself.

## `styled-ppx` package

During dinner, you meet a cool human named Dave who introduces you to
[styled-ppx](https://ocaml.org/p/styled-ppx), the package he authored to allow
writing type-safe CSS inside OCaml. It’s inspired by
[Emotion](https://emotion.sh/docs/introduction), and also uses Emotion under the
hood.

To install `styled-ppx`, open your terminal and run:

```bash
npm install @emotion/css
opam install styled-ppx
```

After those commands finish, see what version of `styled-ppx` was installed by
running `opam list`:

```text{4}
...
stdune                3.15.2      Dune's unstable standard library
stringext             1.6.0       Extra string functions for OCaml
styled-ppx            0.56.0      Type-safe styled components for ReScript and Melange
time_now              v0.16.0     Reports the current time
topkg                 1.0.7       The transitory OCaml software packager
...
```

Add `"styled-ppx" {>= "0.56.0"}` to the `depends` section of your `.opam` file.
(Adjust the version based on what you see  from `ocaml list`.)

To make sure that the version of `@emotion/css` you got from npm is compatible
with the version of `styled-ppx` you got from opam, run

```bash
npm run check-npm-deps
```

You should see some output like this:

```text
Ok: opam package "reason-react.0.14.0" requires npm package: "react-dom" with constraint "^18.0.0", version installed: "18.2.0"
Ok: opam package "reason-react.0.14.0" requires npm package: "react" with constraint "^18.0.0", version installed: "18.2.0"
Ok: opam package "styled-ppx.0.56.0" requires npm package: "@emotion/css" with constraint ">=11.0.0", version installed: "11.11.2"
```

::: warning

The output from `npm run check-npm-deps` might not show anything for
`styled-ppx` yet, that should be fixed soon.

:::

## Add styles to `Promo`

Try adding a style to `Promo`'s `form` element:

```reason
<form
  className=[%cx {|display: flex; flex-direction: column;|}] // [!code ++]
  onSubmit={evt => evt |> React.Event.Form.preventDefault}>
```

You'll get a compilation error:

```
File "src/order-confirmation/Promo.re", line 6, characters 16-18:
6 |     className=[%cx {|display: flex; flex-direction: column;|}]
                    ^^
Error: Uninterpreted extension 'cx'.
```

`styled-ppx` is installed, but it can't be used until the `libraries` and
`preprocess/pps` stanzas of your `dune` file have been updated:

```dune
(libraries reason-react melange-fest styled-ppx.emotion)
 (preprocess
  (pps melange.ppx reason-react-ppx styled-ppx))
```

For reference:

- Adding `styled-ppx` in `preprocess/pps` enables the `%cx` extension node
- Adding `styled-ppx.css` in `libraries` enables the `Css` and `CssJs` modules

If you run `npm run serve', you'll see that the styles are now applied.


## `Promo.Style` submodule

Although you can use the `%cx` extension node inline within your JSX, it's
usually a good idea to put all of them in a `Style` submodule within your
component module:

<<< Promo.re#style-submodule

Then update your JSX:

<<< Promo.re#updated-jsx{2,5}

Using a `Style` submodule encourages reuse of class names and tends to make your
JSX more readable.

## `styled-ppx` is type-safe

The strings we pass to the `%cx` extension must be valid CSS and each property
must have a valid value. To see this in action, change the value of the
`flex-direction` property:

```css
flex-direction: column; // [!code --]
flex-direction: col; // [!code ++]
```

You'll get this compilation error:

```text
File "src/order-confirmation/Promo.re", lines 3-4, characters 37-43:
2 | ...................
3 | .................;
4 |     flex-direction: col.
Error: Property 'flex-direction' has an invalid value: 'col'
```

Change some of the other properties to invalid values and see what happens.

## Install `vscode-styled-ppx` extension

Now that we're writing our CSS inside our OCaml code, the CSS is no longer
highlighted. Fortunately, it's not an issue if for VS Code users, just install
the [vscode-styled-ppx
extension](https://marketplace.visualstudio.com/items?itemName=davesnx.vscode-styled-ppx).

---

summary

## Overview

- tbd

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/promo-component/)
and [demo](https://react-book.melange.re/demo/src/promo-component/) for this chapter.

-----

[^1]: MonadicFanatic1984 in real life is a genetically-modified gorilla who
    escaped from a mad scientist's laboratory. He used to be in charge of
    developing the mad scientist's website, and he escaped because he wanted to
    migrate the website to Melange but the mad scientist didn't approve.

[^2]: In real life, FunctorPunk29 is a cybernetically-enhanced wombat who
    escaped from a mad scientist's laboratory (no, not the same one). He escaped
    because FREEDOM.

[^3]: `React.useReducer` is just the ReasonReact binding for React's [useReducer
    hook](https://react.dev/reference/react/useReducer).

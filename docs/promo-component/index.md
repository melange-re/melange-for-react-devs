# Promo Component

Madame Jellobutter has been pretty impressed with your work so far, so she sends
you on an all-expenses paid trip to an [OCaml
Hackathon](https://fun-ocaml.com/). You learn a lot, make several new friends,
and you even meet MonadicFanatic1984 in person![^1] You eagerly star to improve
your development experience based on the things you've been learning during the
hackathon.

## `styled-ppx` package

At the hackathon, you meet a cool human named Dave who introduces you to
[styled-ppx](https://ocaml.org/p/styled-ppx), the OCaml package he authored to
let you write type-safe CSS inside OCaml. It’s inspired by
[Emotion](https://emotion.sh/docs/introduction), also uses Emotion under the
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

## `RR` utility module

While having lunch during the hackathon, fellow hackathon attendee
FunctorPunk[^2] suggests that you create a dedicated module for your
ReasonReact-related helper functions. Add a new file `RR.re`:

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

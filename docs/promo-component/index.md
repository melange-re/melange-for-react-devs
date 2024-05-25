# Promo Component

Madame Jellobutter has been pretty impressed with your work so far, so she sends
you on an all-expenses paid trip to an [OCaml
Hackathon](https://fun-ocaml.com/). You learn a lot, make several new friends,
and you even meet MonadicFanatic1984 in person![^1] After you return from the
trip, you're excited to improve your development experience by upgrading your
tools.

## `styled-ppx` package

At the hackathon, you meet a cool human named Dave who introduces you to his
[styled-ppx](https://ocaml.org/p/styled-ppx) OCaml package. It lets you write
type-safe CSS inside OCaml! It’s inspired by
[Emotion](https://emotion.sh/docs/introduction) (and uses Emotion under the
hood).

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
we added documentation comments to them. Note the use of square brackets (`[]`):

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

## Rename arguments using `as` keyword

This is our first time seeing the `as` keyword, which is used to rename function
arguments (among other things). It's equivalent to doing this:

```reason
[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t) => {
  let _items = items;
  let _date = date;
```

The renamed variables are only visible within the function, the caller of the
function must still use the original names for the arguments.

If we don't rename `~items` and `~dates` arguments, the compiler will give us
the "unused variable items" error. So we temporarily rename them to names
starting with underscores so the compiler won't complain about their non-usage.

## `React.useReducer`

One of the neat things you learn from FunctorPunk is that

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

[2^]: In real life, FunctorPunk29 is a cybernetically-enhanced wombat who
    escaped from a mad scientist's laboratory (no, not the same one). He escaped
    because FREEDOM.

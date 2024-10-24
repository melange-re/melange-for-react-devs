# Melange for React Developers

::: warning

This is a work in progress.

We'd love to hear your feedback! If you find any errors or have suggestions for
the book, please [file an
issue](https://github.com/ahrefs/melange-for-react-devs/issues) or ping @feihong
on the [#melange channel in the Reason Discord](https://discord.gg/reasonml).

:::

## What is Melange?

Melange is a collection of tools that bring the robustness of a mature,
statically-typed multi-paradigm programming language to the JavaScript
ecosystem. That language is OCaml, a language invented in the 1990s which has
been battle-tested by industry stalwarts. The heart of Melange is a compiler
that translates OCaml to human-readable JavaScript and built-in language
constructs for zero-cost interoperation with JavaScript.

## Why OCaml?

OCaml codebases scale well both in terms of quantity of lines and number of
contributors. The sound type system helps to prevent ambiguous behavior in your
program---if it compiles, it runs. OCaml and React (via
[ReasonReact][reasonreact]) are an effective, FP-friendly combination for
building frontend UIs using JSX syntax. If the backend is also written in OCaml,
you can share types between the frontend and backend, ensuring that they stay in
sync[^1]. In the near future, it will be possible to write [universal React
components](https://github.com/ml-in-barcelona/server-reason-react) that are
performantly rendered on the server using OCaml.

All that said, how does OCaml compare against other prominent compile-to-JS
languages?

**TypeScript** is a gradually-typed superset of JavaScript. It is easier to
learn and adopt, but is less type-safe than OCaml and all other langues on this
list. TypeScript doesn't support pattern matching at the language level.

**ReScript** is very similar to OCaml, even at the syntax level. Its
RescriptReact library is roughly equivalent to our ReasonReact library. The main
differences are that ReScript's only compilation target is JS, and it lacks the
metaprogramming features of OCaml, so you lose the ability to, say, [embed CSS
styles directly into your code](https://styled-ppx.vercel.app/).

**Elm** is similar to OCaml but leans much more strongly towards pure functional
programming. It cannot use React but comes with its own framework. It has many
fewer options for JS interop. Even though Elm compiles to JS, it can't be used
for backend development, even on Node.

**F#** is a fullstack language similar to OCaml and is strongly aligned with the
.NET ecosystem. F# has good support for functional programming and zero-cost
interop with C#. Feliz is an F# wrapper library for React that doesn't support
JSX.

**Kotlin** is a type-safe fullstack language strongly associated with the
Compose Multiplatform UI framework. Kotlin-react is a Kotlin wrapper library for
React which doesn't support JSX. Kotlin has some features in common with OCaml,
but doesn't have pattern matching.

**Dart** is a type-safe fullstack language strongly associated with the Flutter
UI framework, which supports compiling UI apps to mobile, desktop, and browser
environments. There's no Dart wrapper library for modern, functional React
components.

**OCaml** has another JS transpiler called js_of_ocaml. Unlike Melange,
js_of_ocaml prioritizes OCaml compatibility over JavaScript interop.

As you can see, each compile-to-JS language has its own design goals, which
dictate its strengths and weaknesses relative to OCaml and Melange.

## This book

This is a project-based, guided introduction to Melange and its ecosystem,
including ReasonReact, the dune build system, and various useful libraries.
Because Melange uses both OCaml and JavaScript ecosystems, there are quite a few
tools and concepts to learn. Therefore we try to make each chapter small and
digestible, not introducing too many things at once. At the end of each chapter,
you'll find exercises with solutions.

## Audience

You should already know how to make frontend applications in JavaScript, in
particular with [React](https://react.dev/). You should be interested in
learning how to leverage your existing knowledge to build apps using
[ReasonReact][reasonreact]. You do not need to know OCaml[^2]---we'll slowly
introduce the basics of the language throughout the tutorial. That said, a good
complement to this guide is [OCaml Programming: Correct + Efficient +
Beautiful](https://cs3110.github.io/textbook/), which teaches the language from
the ground up and goes much deeper into its features.

## Chapters and topics

| Title  | Summary | Topics covered |
| ------ | ------- | -------------- |
| Counter | Number that can be incremented or decremented | module, Option, `React.string`, pipe last operator, function chaining, switch expression |
| Numeric Types | Use Melange Playground to explore OCaml’s numeric types | Int, Float, Playground, sharing snippets, comparison operators, arithmetic operators, widgets in Playground |
| Celsius Converter | Single input that converts from  Celsius to Fahrenheit | `Js.t` object, string concatenation (`++`), exception handling, ternary expression, if-else expression, labeled argument, partial application, `{js\|\|js}` quoted string literal |
| Celsius Converter Using Option | The same component from the last chapter but replacing exception handling with Option | Option, `Option.map`, `when` guard |
| Introduction to Dune | A introduction to the Dune build system | `dune-project` file, `dune` file, `melange.emit` stanza, monorepo structure |
| Order Confirmation | An order confirmation for a restaurant website | variant type, primary type of module (`t`), wildcard (`_`) in switch, `fun` syntax, `Js.Array` functions, `React.array`, type transformation functions |
| Styling with CSS | Styling the order confirmation using CSS | `mel.raw` extension node, `runtime_deps` field, `glob_files` term, `external`, `mel.module` attribute |
| Better Sandwiches | Support different kinds of sandwiches by adding constructor arguments | variant constructor argument, variant pattern matching, `{j\|\|j}` quoted string literal, `Printf.sprintf`, building bundles |
| Better Burgers | Support different toppings for burgers by using records | record type, record destructuring, record pattern matching, submodules, wildcard |
| Sandwich Tests | Add unit tests for sandwich-related logic | opam switch, `opam switch`, `opam install`, `opam list`, `.opam` file, `melange-fest`, `open` module, `module_systems` field, punning, type inference |
| Cram Tests | Set up cram tests to run your unit tests | cram tests, `cram` stanza, Dune alias, promotion, test output sanitization, sandbox, `expand_aliases_in_sandbox` stanza |
| Burger Discounts | Implement burger discount logic using arrays | limits of type inference, type annotation of function arguments, full name (asset path), `Stdlib`, record spread syntax, `ignore`, runtime representations of common data types, properties of arrays, override `Array.get` |
| Discounts Using Lists | Re-implement burger discount logic using lists | properties of lists, pattern matching on lists, list spread syntax, `List` module, `ListLabels` module, runtime representation of lists, documentation comments, placeholder operator |
| Promo Codes | Implement promo and discount logic using Result | built-in Result type, translating error message to Reason syntax, `List.iter`, for  loops, polymorphic variants, runtime representations of Result and polymorphic variants |
| Promo Component | Create a promo component that renders Results using polymorphic variant | Documentation comment markup language, `React.useReducer`, `as` keyword to ignore arguments, `opam-check-npm-deps`, `Result.map`, render mutually-exclusive states using polymorphic variant constructors |

...and much more to come!

[reasonreact]: https://reasonml.github.io/reason-react/

[^1]: If the backend language isn't OCaml, you can still share types between
    frontend and backend through a third-party tool like
    [atd](https://github.com/ahrefs/atd).

[^2]: Because of the focus on ReasonReact, we won't cover traditional OCaml
    syntax in this guide. Instead, we'll cover the [Reason
    syntax](https://reasonml.github.io/) which works great with ReasonReact
    because of its first-class support for JSX.

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
been battle-tested by industry stalwarts. The heart of Melange is (1) a compiler
that translates OCaml to human-readable JavaScript and (2) built-in language
constructs for zero-cost interoperation with JavaScript.

## Why OCaml?

OCaml codebases scale well both in terms of quantity of lines and number of
contributors. The sound type system helps to prevent ambiguous behavior in your
program---if it compiles, it runs without runtime errors. OCaml and React (via
[ReasonReact][reasonreact]) are an effective, FP-friendly combination for
building frontend UIs using JSX syntax. If the backend is also written in OCaml,
you can share types between the frontend and backend, ensuring that they stay in
sync[^1].  It's even possible to write [universal React
components](https://github.com/ml-in-barcelona/server-reason-react) that are
rendered on the server with the performance of native code[^2].

All that said, how does OCaml compare against other prominent compile-to-JS
languages?

**TypeScript** is a gradually-typed superset of JavaScript. It's easier to learn
and adopt because its design highly emphasizes compatibility with JavaScript.
This brings a few footguns, such as `any`, `as` casting, `ts-nocheck` etc,
making it less type-safe than OCaml (and all the other languages on this list)[^3].

- Zero-cost JS interop? Yes
- Can use with React? Yes
- Supports JSX? Yes
- Pattern matching? No
- Can use on server? Yes, via Node.

**ReScript** is very similar to OCaml, even at the syntax level. The main
difference is that ReScript's only compilation target is JS[^4].

- Zero-cost JS interop? Yes
- Can use with React? Yes, via [@rescript/react](https://github.com/rescript-lang/rescript-react), which is roughly equivalent to
  Melange's ReasonReact bindings.
- Supports JSX? Yes
- Pattern matching? Yes
- Can use on server? Yes, via Node.

**Elm** is similar to OCaml. It leans strongly towards pure functional
programming.

- Zero-cost JS interop? No. Elm treats JavaScript as unsafe, interacting with JS
  via ports (more like message passing than traditional FFI). Ergonomic
  integration with React and other libraries isn't possible.
- Can use with React? No. Elm has its own framework with some similar concepts.
- Supports JSX? No
- Pattern matching? Yes
- Can use on server? No

**F#** is a fullstack language similar to OCaml and is strongly aligned with the
.NET ecosystem. F# has good support for functional programming.

- Zero-cost JS interop? Probably
- Can use with React? Yes. [Feliz](https://github.com/Zaid-Ajaj/Feliz) is an F#
  wrapper library for React.
- Supports JSX? No. Feliz uses ordinary function call syntax for render logic.
- Pattern matching? Yes
- Can use on server? Yes, via .NET or Node.

**Kotlin** is a type-safe fullstack language strongly associated with the
Compose Multiplatform UI framework.

- Zero-cost JS interop? Unknown
- Can use with React? Yes.
  [Kotlin-react](https://github.com/JetBrains/kotlin-wrappers/blob/master/kotlin-react/README.md)
  is a Kotlin wrapper library for React.
- Supports JSX? No. Kotlin-react doesn't support JSX, but it does provide a
  declarative syntax that is somewhat similar.
- Pattern matching? No
- Can use on server? Yes, via JVM or compiled to native code.

**Dart** is a type-safe fullstack language strongly associated with the Flutter
UI framework, which supports compiling UI apps to mobile, desktop, and browser
environments. There's no Dart wrapper library for modern, functional React
components.

- Zero-cost JS interop? Unknown
- Can use with React? Not really. There's no Dart wrapper library for functional
  React components (only [class-based
  components](https://github.com/Workiva/react-dart)). Dart users are encouraged
  to use [Flutter Web](https://flutter.dev/multi-platform/web) for SPAs.
- Supports JSX? No
- Pattern matching? Yes
- Can use on server? Yes, via Dart VM

**OCaml** has another JS transpiler called
[js_of_ocaml](https://github.com/ocsigen/js_of_ocaml). Unlike Melange,
js_of_ocaml prioritizes OCaml compatibility over JavaScript interop[^5].

- Zero-cost JS interop? No. Interop with JavaScript involes some overhead.
- Can use with React? Yes, via
  [jsoo-react](https://github.com/ml-in-barcelona/jsoo-react), which is roughly
  equivalent to Melange's ReasonReact.
- Supports JSX? No
- Pattern matching? Yes
- Can use on server? Yes, via native compilation or Node

As you can see, each compile-to-JS language has its own design goals, which
dictate its strengths and weaknesses relative to OCaml and Melange.

## What's in this book

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
[ReasonReact][reasonreact]. You do not need to know OCaml[^6]---we'll slowly
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

[^2]: Note that
    [server-reason-react](https://github.com/ml-in-barcelona/server-reason-react)
    is not yet polished. However, the parts of its API that are more stable are
    already used in production by [Ahrefs](https://ahrefs.com/).

[^3]: For a more detailed comparison of Melange and TypeScript, see [Melange
    docs](https://melange.re/v4.0.0/melange-for-x-developers#for-typescript-developers).

[^4]: For a more detailed comparison of Melange and ReScript, see [Melange
    docs](https://melange.re/v4.0.0/melange-for-x-developers.html#for-rescript-developers).

[^5]: For a more detailed comparison of Melange and js_of_ocaml, see [Melange
    docs](https://melange.re/v4.0.0/melange-for-x-developers.html#for-js-of-ocaml-developers).

[^6]: Because of the focus on ReasonReact, we won't cover traditional OCaml
    syntax in this guide. Instead, we'll cover the [Reason
    syntax](https://reasonml.github.io/) which works great with ReasonReact
    because of its first-class support for JSX.

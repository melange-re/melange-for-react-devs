# Promo Component

Your work so far has left a deep impressin on Madame Jellobutter, so much so
that she even sends you on an all-expenses paid trip to an [OCaml
Hackathon](https://fun-ocaml.com/). You learn a lot, make several new friends,
and you even meet MonadicFanatic1984 in person![^1] You're extremely psyched to
start applying the things you're learn during the hackathon.

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
[React.useState](https://reasonml.github.io/reason-react/docs/en/usestate-hook)
with
[React.useReducer](https://reasonml.github.io/reason-react/docs/en/usereducer-hook)[^3]
when defining [state
variables](https://react.dev/learn/state-a-components-memory#adding-a-state-variable):

<<< Promo.re#use-reducer{3-4,9}

The advantage of this is that you can update state by passing a value instead of
a callback. To make it even nicer, we can also rename `dispatch` to `setCode`
and use function chaining inside the `onChange` callback:

<<< Promo.re#set-code{3-4,9}

## `RR.useStateValue` helper function

This state variable pattern can be made more convenient by adding new helper
function `RR.useStateValue`:

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

Introduce some other errors to your CSS code and see what happens.

## Install `vscode-styled-ppx` extension

Now that we're writing our CSS inside our OCaml code, the CSS is no longer
highlighted. Fortunately, it's not an issue if for VS Code users, just install
the [vscode-styled-ppx
extension](https://marketplace.visualstudio.com/items?itemName=davesnx.vscode-styled-ppx).

## Add `submittedCode` state variable

Add a new state variable named `submittedCode`:

<<< Promo.re#submitted-code{2,8,15}

Basically, `submittedCode` is:

- `None` if the user hasn't submitted a promo code
- `Some(code)` if the user has submitted a promo code

Pressing the `Enter` key while editing the input submits the promo code, and
editing the input cancels the submitted promo code (sets `submittedCode` back to
`None).

## Add `discountFunction` derived value

Add a `discountFunction` variable that derives its value from `submittedCode`
and `date`:

<<< Promo.re#discount-function

We have to use `Option.map` since `submittedCode`'s type is `option(string)`.
Also, we must stop ignoring the `date` prop in order to use its value:

```reason
[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t) => { // [!code --]
let make = (~items as _: list(Item.t), ~date: Js.Date.t) => { // [!code ++]
```

## Render `discountFunction`

Render `discountFunction` under the `input`:

<<< Promo.re#render-discount-function

We only render `discountFunction` when it's in the error state. We also need to
add the new `codeError` class name to the `Style` submodule:

<<< Promo.re#code-error-class-name

This will make promo code error messages be rendered in red.

## Add `discount` derived value

Add a `discount` variable that derives its value from `discountFunction` and
`items`:

<<< Promo.re#discount

To use the value of the `items` prop, we must stop ignoring it:

```reason
[@react.component]
let make = (~items as _: list(Item.t), ~date: Js.Date.t) => { // [!code --]
let make = (~items: list(Item.t), ~date: Js.Date.t) => { // [!code ++]
```

## Render `discount`

Render `discount` under where we render `discountFunction`:

<<< Promo.re#render-discount

When `discount` is of the form `Some(Ok(value))`, then the discount must exist
and we show its value (with a minus sign in front to indicate that the amount
that will be subtracted from the total value). When `discount` is of the form
`Some(Error(code))`, then an error occurred and we should render an error
message based on the value of `code` (this is left as an exercise at the end of
the chapter).

We also need to add `discountError` to `Style` submodule, which will cause the
discount error to be rendered in purple:

<<< Promo.re#discount-error-class-name

## Use one switch expression

The current rendering logic is unsatisfactory. On first glance, it looks as if
`discountFunction` is rendered first, and then `discount` is rendered. But in
reality, only one of the derived variables is ever rendered at a time. That
suggests that we could use a single switch expression for the render logic. It's
helpful to list the four mutually-exclusive states we need to render:

| State | What to render |
| ---- | ---- |
| No promo code submitted | Nothing |
| Promo code invalid or expired | Error message, e.g. "Invalid promo code" |
| Error applying discount | Error message, e.g. "Buy at least one more burger to enjoy this promotion" |
| Discount | The value of the discount with a minus sign in front |

One way to refactor to a single switch expression is to make the tuple
`(discountFunction, discount)` be the input to the switch expression:

<<< Promo.re#render-tuple

This works, but that is a lot of pattern matching to render just four states.

## Use a single `discount` derived variable

We cannot have better rendering logic without first refactoring the derived
variables. Replace `discountFunction` and `discount` with just a single derived
variable `discount`:

<<< Promo.re#discount-poly

Here we used three nested switch expressions to explicitly map the values from
all the inputs to the four mutually-exclusive states we want to render. The flow
of data goes like this:

1. If `submittedCode` is `None`, return `` `NoSubmittedCode``
1. If `submittedCode` is `Some(code)`, then invoke
   `Discount.getDiscountFunction(code, date)`
   1. If the result of `Discount.getDiscountFunction(code, date)` is
      `Error(error)`, then return `` `CodeError(error)``
   1. If the result of `Discount.getDiscountFunction(code, date)` is
      `Ok(discountFunction)` then invoke `discountFunction(items)`
      1. If the result of `discountFunction(items)` is `Error(error)`, then
         return `` `DiscountError(error)``
      1. If the result of `discountFunction(items)` is `Ok(value)`, then return
         `` `Discount(value)``

Because we use polymorphic variant constructors, we don't need to define a new
type. If you hover over the `discount` variable in your editor you'll see that
its inferred type is:

```reason
[> `CodeError(Discount.error)
 | `Discount(float)
 | `DiscountError([> `MissingSandwichTypes
                   | `NeedMegaBurger
                   | `NeedOneBurger
                   | `NeedTwoBurgers ])
 | `NoSubmittedCode ]
```

## Render `discount` polymorphic variant

Now change the switch expression in your render logic to take the `discount`
polymorphic variant as its only input:

<<< Promo.re#render-discount-poly

The pattern matching is now more concise and more readable, and it is still
type-safe. Try commenting out the `` `NoSubmittedCode`` branch and you'll get a
compilation error:

```text
File "src/order-confirmation/Promo.re", lines 52-68, characters 4-7:
52 | ....{switch (discount) {
53 |      //  | `NoSubmittedCode => React.null
54 |      | `Discount(discount) => discount |> Float.neg |> RR.currency
55 |      | `CodeError(error) =>
56 |        <div className=Style.codeError>
...
65 |        <div className=Style.discountError>
66 |          {RR.s("Todo: discount error message")}
67 |        </div>
68 |      }}
Error (warning 8 [partial-match]): this pattern-matching is not exhaustive.
Here is an example of a case that is not matched:
`NoSubmittedCode
```

---

Muy bueno! You've created a `Promo` component that can be used to submit promo
codes and see the discounts they produce when applied to an order, along with
any errors that might occur. In the next chapter, we'll integrate this `Promo`
component into your `Order` component.

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

[^2]: FunctorPunk is a cybernetically-enhanced wombat who
    escaped from a mad scientist's laboratory (no, not the same one). He escaped
    because FREEDOM.

[^3]: `React.useReducer` is the ReasonReact binding for React's [useReducer
    hook](https://react.dev/reference/react/useReducer).

# Counter

We're going build the classic frontend starter app, the counter, using
[ReasonReact](https://reasonml.github.io/reason-react/). If you've already
[installed the starter project](/installation/), you can run the project now:

1. Go to the root directory of your
[melange-for-react-devs-template](https://github.com/melange-re/melange-for-react-devs-template)
project
1. Run `make watch` to start the Melange compiler in watch mode.
1. In another terminal window, start the webpack dev server by running `make
serve`. As a side effect, it will open a browser tab pointed to
<a href="http://localhost:8080/" target="_blank" rel="noreferrer
noopener">http://localhost:8080/</a>.

Open `Index.re` and you'll see this:

<<< Snippets.re#app-v1

This is just about the simplest component you can make, but through it, we can
start to see some of the key differences of developing with ReasonReact:

- All components must be inside a module
- The `make` function renders the component and returns `React.element`
- We must decorate the `make` function with `[@react.component]`
- In JSX, we must wrap strings with calls to `React.string`

Let's go over these differences in more detail.

## Components are modules

In the example above, a new module named `App` is defined. OCaml's
[modules](https://cs3110.github.io/textbook/chapters/modules/modules.html) are
somewhat like JavaScript modules, with one notable difference being that there
can be multiples modules inside a single file. For now, you just need to know
that all components in ReasonReact are modules, and the name of the component
comes from the name of the module.

## The make function

The `make` function has the type `unit => React.element`, meaning it takes `()`
as the only argument and returns an object of type `React.element`. You'll need
to decorate `make` with the
[attribute](https://melange.re/v2.1.0/communicate-with-javascript/#attributes)
`@react.component`. We'll go into the details [later](/todo), but for now
let's just say that `@react.component` is there to reduce boilerplate and make
our code more readable and easier to maintain.

## Wrap strings with `React.string`

Unlike in normal React, we must wrap strings inside function calls to convert
them to the `React.element` type. This is exactly what the `React.string`
function does---if you hover over it, you'll see that it displays the type
`string => React.element`.

## Using the App component

A little bit further down, we make use of the `App` component:

<<< Snippets.re#use-app-component

`React.querySelector("#root")` returns an `option(Dom.element)`, meaning that if
it doesn't find the element, it returns `None`, and if it does find the element,
it returns `Some(Dom.element)`, i.e. the element wrapped in the `Some`
constructor. The *switch expression*[^1] allows you to succinctly express:

- If `node` is `Some(Dom.element)`, render the `App` component to the DOM
- Otherwise if `node` is `None`, log an error message

We'll talk more about `option` in the [Celsius converter chapter](/celsius-converter-option/).

## The Counter component

Let's create a counter component by creating a new file `Counter.re` with the
following contents:

<<< Snippets.re#counter-v1

This is a component with a single `useState` hook. It should look fairly
familiar if you know about [hooks in React](https://react.dev/reference/react).
Note that we didn't need to manually define a module for `Counter`, because all
source files in OCaml are automatically modules, with the name of the module
being the same as the name of the file.

Now let's modify `App` so that it uses our new `Counter` component:

<<< Snippets.re#app-v2

## The pipe last operator

To display the number of the counter, we wrote
`{React.string(Int.to_string(counter))}`, which converts an integer to a string,
and then converts that string to `React.element`. In OCaml, there's a way
to apply a sequence of operations over some data so that it can be
read from left to right:

```reason
{counter |> Int.to_string |> React.string}
```

This uses the [pipe last operator](https://melange.re/v2.1.0/communicate-with-javascript/#pipe-last),
which is useful for chaining function calls.

## Basic styling

Let's add a bit of styling to the root element of `Counter`:

<<< Snippets.re#render-with-styling{2-7}

Unlike in React, the `style` prop in ReasonReact doesn't take a generic object,
instead it takes an object of type `ReactDOMStyle.t` that is created by calling
`ReactDOMStyle.make`. This isn't a sustainable way to style our app---later,
we'll see how to [style using CSS classes](/todo).

----

Congratulations! You've created your first ReasonReact app and component. In
future chapters we'll create more complex and interesting components.

## Exercises

<b>1.</b> There aren't any runtime errors in our app right now. But what happens
if you try to remove the `| None` branch of the `switch (node)` expression?

<b>2.</b> What happens if you rename the `_evt` variable inside the button
callback to `evt`?

<b>3.</b> Comment out the `[@react.component]` attribute in `Counter.re`. What
happens?

## Overview

What we covered in this section:

- How to create and run a basic ReasonReact app
- ReasonReact components are also modules
- OCaml has an `option` type whose value can be either `None` or `Some(_)`
- The pipe last operator is an alternate way to invoke functions that enables
  easy chaining of function calls
- The `style` prop doesn't take generic objects

## Solutions

<b>1.</b> Removing the `| None` branch will result in a compilation error:

```
Error (warning 8 [partial-match]): this pattern-matching is not exhaustive.
Here is an example of a case that is not matched:
None
```

Basically, the compiler is telling you to handle the `None` case if you want
to ship your app. This is part of what makes OCaml such a type-safe language.

<b>2.</b> Renaming `_evt` to `evt` results in a compilation error:

```
Error (warning 27 [unused-var-strict]): unused variable evt.
```

OCaml wants you to use all the variables you declare, unless they begin with
`_` (underscore).

<b>3.</b> Commenting out `[@react.component]` in `Counter.re` will trigger a
compilation error in `Index.re`, at the place where `Counter` component is used:

```
File "Index.re", line 3, characters 19-27:
3 |   let make = () => <Counter />;
                       ^^^^^^^^^^^
Error: Unbound value Counter.makeProps
```

For now, don't worry about what `Counter.makeProps` is or where it came
from---just remember that you need to put the `[@react.component]` attribute
above your `make` function if you want your component to be usable in JSX. This
is a very common newbie mistake. See the [PPX chapter](/todo) for more details.

-----

[Source code for this
chapter](https://github.com/melange-re/melange-for-react-devs/blob/main/src/counter/)
can be found in the [Melange for React Developers
repo](https://github.com/melange-re/melange-for-react-devs).

[^1]: Despite the name, don't confuse OCaml's *switch expressions* with
    JavaScript's *switch statements*.

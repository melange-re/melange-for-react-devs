# Order Confirmation

The famed restauranteur Madame Jellobutter has opened a hot new pop-up
restaurant called Emoji Cafe, and you've been commissioned to build the order
confirmation widget on its website. Feeling adventurous, you decide to build it
using Melange.

Start by creating a new directory `src/order-confirmation` and give it the
same directory structure as we showed you in the previous chapter:

```
src/order-confirmation
├─ dune
├─ Index.re
├─ Makefile
└─ Item.re
```

The `dune` file can be copied from any of the existing projects. The `Makefile`
can also be copied over, but remember to update the value of the `app`
environment variable to `order-confirmation`. The `.re` files can be empty for
now.

## Variant type `Item.t`

For the time being, there are only two items you can order at Emoji Cafe, the
sandwich or the burger. In `Item.re`, add a new type:

<<< Snippets.re#type-t

This is a *variant type*[^1] named `t` with two *constructors*, `Sandwich` and
`Burger`. In OCaml, it is customary for the primary type of a module to be
called `t`. This convention makes sense because in other modules, this type will
be referred to as `Item.t`.

The `Item` module should contain helper functions that help us render an item,
i.e. we'll need functions that can return the price and the emoji[^2] for a
given item. First, add the `toPrice` function:

<<< Snippets.re#to-price

If you decide to add, say, a hotdog, to the menu, you would need to:

- Add a `Hotdog` constructor to `Order.t`
- Add a `| Hotdog` branch to the switch expression of `Order.toPrice`

Your OCaml code would fail to compile if you added `Hotdog` or removed
`Sandwich` from `Item.t` without also updating `Item.toPrice`. This is one of
the great advantages of variant types: changing the constructors will force you
to change the relevant parts of your code.

## Wildcard in switch expressions

Let's say that Madame Jellobutter decides to temporarily lower the price of
burgers so that they're the same price as sandwiches. You could then rewrite
`Item.toPrice` like this:

```reason
let toPrice = t =>
  switch (t) {
  | _ => 10.
  };
```

The underscore (`_`) here serves as a wildcard matching any constructor.
However, this would be a very bad idea! Now changing the constructors in
`Item.t` would not force you to change `Item.toPrice` accordingly. A superior
version would be:

```reason
let toPrice = t =>
  switch (t) {
  | Sandwich => 10.
  | Burger => 10.
  };
```

OCaml's pattern-matching syntax allows you to combine branches, which would
simplify it to:


```reason
let toPrice = t =>
  switch (t) {
  | Sandwich
  | Burger => 10.
  };
```

In any case, you should strive to avoid wildcards and explicitly match all
constructors in your switch expressions.

## `fun` sugar syntax

Incidentally, there's a sugar syntax for functions whose entire body is a switch
expression. It's called `fun`, and we can rewrite `Item.toPrice` to use it:

<<< Snippets.re#to-price-fun

We can also define `toEmoji` using the `fun` sugar syntax:

<<< Snippets.re#to-emoji

Using the `fun` syntax is completely equivalent to using a switch expression,
so it's up to your personal taste whether you want to use this sugar syntax.

## `Order.make`

Now we're ready to define the `make` function which will render the `Item`
component:

<<< Snippets.re#make

The `make` function has a single labeled argument, `~item`, of type `t`. This
effectively means the `Item` component has a single prop named `item`.

Note that this renders a single row of a table. We'll need another component
to render a table containing all items in an order.

## `Order` component

Create a new file `src/order-confirmation/Order.re` and add the following code:

<<< Snippets.re#order

There's a lot going on here:

- The primary type of `Order` is `array(Item.t)`, which is an array of
  variants.
- The `make` function has a single labeled argument, `~items`, of type `t`.
  This means the `Order` component has a single prop named `items`.
- We sum up the prices of all items using
  [Js.Array.reduce](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-reduce).
  This is identical to JavaScript's [Array.reduce
  method](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce),
  except that the Melange version requires the initial value to be passed in.
- For each order, we render an `Item` component via
  [Js.Array.map](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-map),
  which is equivalent to the [Array.map
  method](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map).
- The `React.array` function has the type signature `array(React.element) =>
  React.element`, meaning it converts an array of `React.element`s to
  `React.element`. All expressions in JSX must have the type `React.element`.

## `Index.re`

Render the `Order` component inside `Index.re`:

<<< Snippets.re#index

Run `make serve` inside `src/order-confirmation` to see your new app in action.
Open your browser's dev console, where you should see a warning:

```
Warning: Each child in a list should have a unique "key" prop.
```

Oops, we should've used
[Js.Array.mapi](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-mapi)[^3]
instead so we could set the `key` prop for each `Item` component:

<<< Snippets.re#mapi

You've got a basic component now, but it looks... not so great.[^4]

## Style with CSS

Let's add styling, but instead of using the `style` prop as we did in previous
chapters, let's use good old CSS. First, install `style-loader` and `css-loader`
webpack plugins:

```
npm install --save-dev css-loader style-loader
```

Add a new rule to `webpack.config.js` so that you can import .css files as
modules:

<<< @/../webpack.config.js{5-12}

Add a new file
`src/order-confirmation/item.css` and give it these styles:

<<< @/../src/order-confirmation/item.css

## `mel.raw` extension node

In OCaml, there is no syntax to import from files, because all modules within a
project are visible to all other modules[^5]. However, we can make use of
JavaScript's `import` syntax by using the [mel.raw extension
node](https://melange.re/v2.1.0/communicate-with-javascript/#generate-raw-javascript),
which allows us to embed raw JavaScript in our OCaml code. Add the following
code to the top of `Item.re`:

```reason
[%%mel.raw {|import "./item.css"|}];
```

The `{|` and `|}` are string delimiters which allow you to define a string that
doesn't require you to escape characters like double-quote and newline. They are
similar to the `{js|` and `|js}`
delimiters we first saw in the [Celsius
Converter](/celsius-converter-exception/#solutions) chapter, with the
difference that they aren't JavaScript strings so they won't handle Unicode correctly.

Unfortunately, in the terminal where we're running `make serve`, we see this webpack compilation error:

```
ERROR in ./_build/default/src/order-confirmation/output/src/order-confirmation/Item.js 5:0-6:1
Module not found: Error: Can't resolve './item.css' in '/home/fhsu/work/melange-for-react-devs/_build/default/src/order-confirmation/output/src/order-confirmation'
```

## `runtime_deps` field of `melange.emit`

The problem is that webpack is serving the app from the build directory at
`_build/default/src/order-confirmation/output/src/order-confirmation`, and the
`item.css` file isn't in the build directory.

To solve this, we can use the [runtime_deps
field](https://melange.re/v2.1.0/build-system/#handling-assets) inside
`src/order-confirmation/dune`:

```clj{7}
(melange.emit
 (target output)
 (libraries reason-react)
 (preprocess
  (pps melange.ppx reason-react-ppx))
 (module_systems es6)
 (runtime_deps item.css))
```

We also want to add styles for the `Order` component, so add a new file
`src/order-confirmation/order.css` with these styles:

<<< @/../src/order-confirmation/order.css

To ensure that `order.css` is also copied to the build directory, we can add
`order.css` to the value of `runtime_deps`:

```clj
(runtime_deps item.css order.css)
```

A better way is to just tell `runtime_deps` to copy all `.css` files over:

```clj
(runtime_deps (glob_files *.css))
```

## Add classes to JSX

Now we can add the appropriate classes to `Item.make`'s JSX:

<<< Snippets.re#make-with-classes

As well as `Order.make`'s JSX:

<<< Snippets.re#order-make-with-classes

Finally, add a `mel.raw` extension node at the top of `Order.re`:

```reason
[%%mel.raw {|import "./order.css"|}];
```

Excelsior! Madame Jellobutter is pleased with your progress on the order
confirmation widget. But she plans to add more options for her current menu
items, for example she'd like to more than one type of sandwich. We'll tackle
that in the next chapter.

## Exercises

<b>1.</b> We used the `%%mel.raw` extension node to import CSS files as modules,
but there is also a `%mel.raw` extension node. What happens if you use
`%mel.raw` instead?

<b>2.</b> Add another a `Hotdog` constructor to `Item.t` variant type. Update
the `Item` module's helper functions to get your program to compile again.

<b>3.</b> Instead of repeatedly using `value |>
Js.Float.toFixedWithPrecision(~digits=2)
|> React.string`, add a helper function `Format.currency` that does
the same thing.

## Overview

- By convention, the main type in a module is often named `t`
- A variant is a type that has one or more constructors
  - Adding or removing constructors forces you to change the relevant parts of
    your code
- The `fun` sugar syntax helps you save a little bit of typing when you have a
  function whose entire body is a switch expression
- Adding props to a component by adding labeled arguments to its `make` function
- Useful array helper functions can be found in the
  [Js.Array](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html)
  module
  - The `Js.Array.reduce` function is equivalent to JavaScript's `Array.reduce`
    method
  - The `Js.Array.map` and `Js.Array.mapi` functions can be used in place of
    JavaScripts `Array.map` method
- The `React.array` function is needed when you want to convert an array of
  `React.element`s to a single `React.element`
- `mel.raw` extension node is used to embed raw JavaScript in our OCaml code
- The `runtime_deps` field of the `melange.emit` stanza is used to copy assets
  like `.css` files into our build directory

## Solutions

<b>1.</b> If we use `%mel.raw` instead of `%%mel.raw` in `Item.re`, we'll get this webpack
compilation error:

```bash
ERROR in ./_build/default/src/order-confirmation/output/src/order-confirmation/Item.js 5:9
Module parse failed: Unexpected token (5:9)
You may need an appropriate loader to handle this file type, currently no loaders are configured to process this file. See https://webpack.js.org/concepts#loaders
| import * as JsxRuntime from "react/jsx-runtime";
|
> ((import "./item.css"));
```

So `%mel.raw` surrounds the JavaScript code you put into it because it wants to
treat it as an expression. Whereas `%%mel.raw` treats the JavaScript it's given
as a statement. If you change it back to `%%mel.raw`, the generated JavaScript
will look like this:

```javascript
import "./item.css"
;
```

<b>2.</b> After you add a `HotDog` constructor to `Item.t`, your `Item` module
should look something like this:

<<< Snippets.re#hotdog

Of course, you may have chosen a different price for the hotdog.

<b>3.</b> In order to create a helper function `Format.currency`, we must create
a new module file called `Format.re` and add a `currency` function:

<<< @/../src/order-confirmation/Format.re

Then we can use that function like this:

```reason
<td className="price"> {item |> toPrice |> Format.currency} </td>
```

-----

[Source code for this
chapter](https://github.com/melange-re/melange-for-react-devs/blob/main/src/order-confirmation/)
can be found in the [Melange for React Developers
repo](https://github.com/melange-re/melange-for-react-devs).

[^1]: Variant types have no equivalent in JavaScript, but they are similar to
    TypeScript's [union
    enums](https://www.typescriptlang.org/docs/handbook/enums.html#union-enums-and-enum-member-types)

[^2]: Recall that the name of this restaurant is Emoji Cafe, so everything from
  the menu to the order confirmation must use emojis.

[^3]: Note that unlike JavaScript's [Array.map
  method](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map),
  we have to use
  [Js.Array.mapi](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-mapi)
  instead of
  [Js.Array.map](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-map)
  if we want to use the index value.

[^4]: Madame Jellobutter was passing by and just happened to catch a glimpse of
  the unstyled component over your shoulder and puked in her mouth a little.

[^5]: Recall that in the `Index` modules you've written so far, you've never had
    to import any of the components you used that were defined in other files.

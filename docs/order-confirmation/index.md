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

The `Item` module should contain helper functions that return the price and the
emoji[^2] for a given item. First, add the `toPrice` function:

<<< Snippets.re#to-price

If Madame Jellobutter decides to add a hotdog to the menu, you would need to:

- Add a `Hotdog` constructor to `Order.t`
- Add a `| Hotdog` branch to the switch expression of `Order.toPrice`

Your OCaml code would fail to compile if you added `Hotdog` or removed
`Sandwich` from `Item.t` without also updating `Item.toPrice`. This is one of
the great advantages of variant types: changing the constructors will force you
to change the relevant parts of your code.

## Wildcard in switch expressions

If Madame Jellobutter decides to do a promotion that lowers the price of burgers
so that they're the same price as sandwiches, you could rewrite `Item.toPrice`
to:

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

Since OCaml's pattern-matching syntax allows you to combine branches, you can
simplify it to:


```reason
let toPrice = t =>
  switch (t) {
  | Sandwich
  | Burger => 10.
  };
```

In any case, you should strive to avoid wildcards. The OCaml Way is to
explicitly match all constructors in your switch expressions.

## A `fun` syntax for switch

There's an alternate, shorter syntax for functions whose entire body is a switch
expression. It's called `fun`, and we can rewrite `Item.toPrice` to use it:

<<< Snippets.re#to-price-fun

We can also define `toEmoji` using the `fun` syntax:

<<< Snippets.re#to-emoji

Using the `fun` syntax is completely equivalent to using a switch expression,
so it's up to your personal taste whether you want to use one or the other.

## `Item.make`

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
- The `Order.make` function has a single labeled argument, `~items`, of type
  `t`. This means the `Order` component has a single prop named `items`.
- We sum up the prices of all items using
  [Js.Array.reduce](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-reduce),
 which is the Melange binding to JavaScript's [Array.reduce
  method](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce).
  Note that `Js.Array.reduce` requires the initial value to be passed in.
- For each order, we render an `Item` component via
  [Js.Array.map](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-map),
  which is the Melange binding to the [Array.map
  method](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map).

## `React.array`

You might have noticed that we need a call to `React.array` after the call to
`Js.Array.map`:

```reason
{items |> Js.Array.map(item => <Item item />) |> React.array}
```

If we leave off the call to `React.array`, we'd get this error:

```
File "src/order-confirmation/Order.re", lines 12, characters 6-12:
12 |         {items |> Js.Array.map(item => <Item item />)}
             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Error: This expression has type React.element array
       but an expression was expected of type React.element
```

We get this error primarily because collections in OCaml can only contain
elements of the same type. The `tbody` element expects children of type
`React.element`[^3], but the call to `Js.Array.map` returns `array(React.element)`,
which creates a type mismatch. To make the actual type match the expected type,
we must add a call to `React.array` which turns `array(React.element)` to
`React.element`.

To better see what types are at play, it might make sense to refactor
`Order.make` like so:

<<< Snippets.re#order-make-item-rows{5,9}

This way you can hover over `itemRows` and see that it has type
`array(React.element)`.

## `Index.re`

Render the `Order` component inside `src/order-confirmation/Index.re`:

<<< Snippets.re#index

Run `make serve` inside `src/order-confirmation` to see your new app in action.

## `Js.Array.mapi`

Open your browser's dev console, where you should see a warning:

```
Warning: Each child in a list should have a unique "key" prop.
```

Oops, we forgot the set the `key` props! One way to fix this is to use
[Js.Array.mapi](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html#val-mapi)
instead[^4] so we can set `key` based on the index of the element:

<<< Snippets.re#mapi

The `Js.Array.mapi` function is also a binding to the `Array.map` method, but
unlike `Js.Array.map`, it passes the element *and the index* into the callback.
If you hover over it, you'll see that it has the type signature

```
(('a, int) => 'b, array('a)) => array('b)
```

In practice, it's common for Melange bindings do not match one-to-one to their
JavaScript counterparts.

Wunderbar! You've got a basic order confirmation component, but it looks... not
so great[^5]. In the next chapter, we'll see how ReasonReact components can be
styled with plain old CSS.

## Exercises

<b>1.</b> The `Item` component is only used inside the `Order` component and we
don't expect it to be used anywhere else (items rendered in a menu component
would look different). Rename it to `OrderItem` and move it inside the `Order`
module.

<b>2.</b> Add another constructor to `Item.t` variant type. Update the `Item`
module's helper functions to get your program to compile again.

<b>3.</b> Instead of repeatedly using `value |>
Js.Float.toFixedWithPrecision(~digits=2)
|> React.string`, add a helper function `Format.currency` that does
the same thing.

## Overview

- By convention, the main type in a module is often named `t`
- A variant is a type that has one or more constructors
  - Adding or removing constructors forces you to change the relevant parts of
    your code, *unless* you use wildcards when pattern-matching on a variant
  - Using wildcards in your switch expression makes your code less adaptable to
    change
- The `fun` syntax helps you save a little bit of typing when you have a
  function whose entire body is a switch expression
- Labeled arguments in a component's `make` function are treated as props by
  ReasonReact.
- The [Js.Array](https://melange.re/v2.1.0/api/re/melange/Js/Array/index.html)
  module contains useful array functions
  - The `Js.Array.reduce` function is the binding to JavaScript's `Array.reduce`
    method
  - The `Js.Array.map` and `Js.Array.mapi` functions are both bindings to
    JavaScript's `Array.map` method
- The `React.array` function is needed when you want to convert an array of
  `React.element`s to a single `React.element`, e.g. after a call to
  `Js.Array.map`

## Solutions

<b>1.</b> To move the `Item` component from the `Item` module to the
`Order` module, you'll have to move the `Item.make` function to a submodule
called `Order.OrderItem`. Then you'll have to prefix the references to `t`,
`toPrice`, and `toEmoji` with `Item.` since they're now being referenced outside
the `Item` module. After you're done, `src/order-confirmation/Order.re` should
look something like this:

<<< @/../src/order-confirmation/Order.re

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

[^3]: All lowercase elements like `div`, `span`, `table`, etc expect their
    children to be of type `React.element`. But React components (with uppercase
    names) can take children of any type.

[^4]: Using array indexes to set keys violates [React's rules of
    keys](https://react.dev/learn/rendering-lists#rules-of-keys), which states
    that you shouldn't generate keys while rendering. We'll see a better way to
    do this [later](/todo).

[^5]: Madame Jellobutter was passing by and just happened to catch a glimpse of
  the unstyled component over your shoulder and puked in her mouth a little.


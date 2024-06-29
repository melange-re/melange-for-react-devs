# Order with Promo

Now that you have a `Promo` component, you can add it to your `Order` component.
With that in place, customers can finally enter promo codes and enjoy discounts
on their orders.

## Add `discount` type

But first, let's see how to create an explicit type for the `discount` reactive
value inside `Promo`. We do so not because we have to, but because it will give
us some more insight into OCaml's type system.

When we hover over the `discount` variable, we see this:

```reason
[> `CodeError(Discount.error)
 | `Discount(float)
 | `DiscountError([> `MissingSandwichTypes
                   | `NeedMegaBurger
                   | `NeedOneBurger
                   | `NeedTwoBurgers ])
 | `NoSubmittedCode ]
```

The easiest thing to do is to create a new `discount` type and assign it to that
type expression:

<<< Types.re#bad-discount-type

However, this results in a compilation error:

```text
Error: A type variable is unbound in this type declaration.
       In type
         [> `CodeError of Melange.Discount.error
          | `Discount of float
          | `DiscountError of
              [> `MissingSandwichTypes
               | `NeedMegaBurger
               | `NeedOneBurger
               | `NeedTwoBurgers ]
          | `NoSubmittedCode ]
         as 'a the variable 'a is unbound
```

We'll come back to this error message later. For now, observe that the error
disappears if we simply delete all the instances of `>`:

<<< Types.re#delete-refinement

This fixes the syntax error so that we now have a correctly-defined polymorphic
variant type.

## Type annotate `discount` variable

We just defined a new type, but our `discount` variable doesn't know anything
about it. Since polymorphic variants can be used without explicitly defining
them, type inference works differently for them. In particular, our `discount`
variable continues to use its own inferred type, despite there being a perfectly
good type within scope that uses the same variant tags.

Type annotate the `discount` variable with the newly-created `discount` type:

```reason
let discount = // [!code --]
let discount: discount = // [!code ++]
```

Now when you hover over the `discount` variable, you see that its type is just
`discount`.

## Type constructor and type variable

Change the `discount` type to this:

<<< Types.re#type-variable

Now `discount` is a *type constructor* that takes a *type variable* named `'a`.
A type constructor is not a fixed type---you can think of it as function that
takes a type and outputs a new type. This is reinforced by the compilation error
we get:

```text
31 |   let discount: discount =
                     ^^^^^^^^
Error: The type constructor discount expects 1 argument(s),
       but is here applied to 0 argument(s)
```

You can fix it like so:

```reason
let discount: discount = // [!code --]
let discount: discount(_) = // [!code ++]
```

We use `discount(_)` to tell the compiler that it should use the `discount` type
constructor, but the value of its argument should be inferred. Now if you hover
over the `discount` variable, you see that its type is:

```reason
discount([> `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ])
```

## `>` = "allow more than"

Once again, we see `>`. In polymorphic variant type expressions, it means "allow
more than". In this case, it means that tags other than the four that are listed
are allowed. For example, this type would be allowed:

```reason{5-6}
discount([| `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
          | `HoneyButter
          | `KewpieMayo ])
```

Generally, you won't need to use `>` in your own type definitions, but it often
appears when the compiler is allowed to infer the type of a variable or function
that uses polymorphic variants.

## Implicit type variable

Let's come back to the question of why the original attempt at a type definition
is syntactically invalid:

<<< Types.re#bad-discount-type

The reason is that whenever you have `>`, you implicitly have a type variable.
So the above code is equivalent to this:

<<< Types.re#explicit-type-var{14}

Now the error message makes a bit more sense:

> Error: A type variable is unbound in this type declaration.

The type variable exists, but it doesn't appear as an argument of the `discount`
type constructor. Once it's added, it compiles:

<<< Types.re#add-type-arg{1}

This is somewhat like accidentally using a variable in a function but forgetting
to add that variable to the function's argument list.

## Do you need to make a type for `discount`?

When using a polymorphic variant, you don't generally need to explicitly define
a type. But it doesn't hurt to do so, and can serve as a sort of "documentation"
by showing all your variant tags in one place. [Later](/todo), we'll show you an
example of when you must define types to take advantage of the more advanced
features of polymorphic variants.

## Add `DateInput` component

To see different promotions in action, we want to be able to easily change the
date in our demo, so add a new file `DateInput.re`:

<<< DateInput.re

Note how the `type` prop of `input` has been renamed to `type_`, because in
OCaml, `type` is a reserved keyword and can't be used as an argument name.
But don't worry, it will still say `type` in the generated JS output.

## Add `Demo` component

Move the contents of `Index.App` into a new file called `Demo.re`. In the
process, add our newly-created `DateInput` component:

<<< Demo.re#initial{16-17,21}

Change `Index` to use the new `Demo` component:

<<< Index.re{7}

## Add `Promo` to `Order`

Add the `Promo` component to the `Order` component:

<<< Order.re#make{3,22-25,28}

A breakdown:

- Create a new state variable called `discount`
- Set the value of `discount` through `Promo`'s `onApply` callback prop (we'll
  add this prop in the next step)
- Subtract `discount` from `subtotal` when rendering the total price of the
  order

## Add `onApply` prop to `Promo`

Add an `onApply` prop to `Promo`, which will be invoked when a promo code is
successfully submitted and results in a discount:

<<< Promo.re#add-on-apply{3}

## Add `React.useEffect1`

To invoke `onApply`, we can add a `useEffect` hook that invokes `onApply` when
`discount` has a value of the form `` `Discount(value) ``:

<<< Promo.re#use-effect

## `useEffect*` functions

`React.useEffect1` is a one of the binding functions for React's [useEffect
hook](https://react.dev/reference/react/useEffect). The number `1` at the end of
the function indicates how many dependencies this function is supposed to take.
Accordingly, there are also `React.useEffect0`, `React.useEffect2`, etc, all the
way up to `React.useEffect7`[^1].

All `React.useEffect*` functions accept a [setup
callback](https://react.dev/reference/react/useEffect#reference) as their first
argument, the type of which is:

```reason
unit => option(unit => unit)
```

The setup callback's return type is `option(unit => unit)`, which allows you to
return a cleanup function encased in `Some`. When you don't need to return a
cleanup function, you can just return `None`.

## `useEffect*` dependencies

The second argument for all `React.useEffect*` functions except
`React.useEffect0` is for the dependencies. For example, the type of
`React.useEffect2` is:

```reason
(unit => option(unit => unit), ('a, 'b)) => unit
```

And the type of `React.useEffect3` is:

```reason
(unit => option(unit => unit), ('a, 'b, 'c)) => unit
```

::: tip

Every time you add or remove a dependency from your `useEffect` hook, you'll
need to use a different `React.useEffect*` function (the one that corresponds to
how many dependencies you now have).

:::

## Tuples vs arrays

Both functions take their dependencies as a tuple instead of an array (as would
be the case in ReactJS). To understand why, we need to understand the type
properties of tuples and arrays:

- The elements of tuples can have different types, e.g. `(1, "a", 23.5)`
- The elements of arrays must all be of the same type, e.g. `[|1, 2, 3|]`,
  `[|"a", "b", "c"|]`

Therefore, we must use tuples to express the dependencies of `useEffect` hooks,
otherwise our dependencies would all have to be of the same type.

## Tuples become arrays in JS

Even though we use tuples for dependencies in our OCaml code, they are turned
into JS arrays in the runtime. So the generated code will run the same as in any
ReactJS app.

## Why does `useEffect1` accept an array?

As you've seen, `React.useEffect2`, `React.useEffect3`, etc all accept a tuple
argument for their dependencies. But `React.useEffect1` is the odd man out,
because it accepts an array. The reason is that one-element OCaml tuples don't
become arrays in the JS runtime, they instead take on the value of their single
element. So in this case, `React.useEffect1` must take an array so that the
generated JS code does the right thing.

## `RR.useEffect1` helper function

`React.useEffect1` taking an array means that you could accidentally pass in an
empty array as the dependency for `React.useEffect1`. You can sidestep this
possibility by adding a helper function to your `RR` module:

<<< RR.re#use-effect-1

Refactor `Promo` to use your new helper function:

<<< Promo.re#use-effect-helper{1,11}

You may be wondering why ReasonReact doesn't provide this helper function for
you. The reason is that its bindings to React functions are supposed to be
zero-cost, without any additional abstractions on top. This is the same reason
that something like our `RR.useStateValue` helper function is also not included
with ReasonReact.

## Add styling for promo code row

Execute `npm run serve` to see your app in action. Verify that it behaves as
expected:

- Type "FREE" into the input and press Enter. It should deduct the price of
  every other burger (ordered by price descending).
- Type "HALF" into the input and press Enter. It should deduct half of the
  entire order.
- Change the date to something other than May 28. It should an error saying
  "Expired promo code"

However, the styling is a little bit off. Add the following class variable to
`Order.Style`:

<<< Order.re#promo-class

Then set the `className` of the promo code row to `Style.promo`:

<<< Order.re#set-promo-class{1}

## How often does the Effect run?

Everything seems to be working correctly, but let's see how often our
`useEffect` hook fires by adding a little logging:

<<< Promo.re#log{8}

You see that every time a promo code is successfully applied, it logs twice to
the console. That doesn't seem right, because the value of `discount` only
changes once when you submit a new promo code.

The reason lies in the runtime representation of `discount`---recall that
polymorphic tags with arguments are turned into objects in the JS runtime.
Because `discount` is a reactive value, it gets recreated on every render, and
even though its contents didn't necessary change, the [hook treats it as having
changed because the object is no longer the same one as
before](https://react.dev/reference/react/useEffect#removing-unnecessary-object-dependencies).

## Use `submittedCode` as dependency

The easiest fix is to simply change the dependency to `submittedCode` instead of
`discount`:

<<< Promo.re#submitted-code-dep{13}

This seems to do the trick---the Effect only runs once every time you submit a
new promo code. But wait! Why does it behave differently when `submittedCode` is
a `option`, and `option` is just another variant type?[^2]

Although `option` is a variant type, its [runtime representation is a special
case](../burger-discounts/#runtime-representation-of-option):

- `None` becomes `undefined`
- `Some(value)` becomes `value`

Therefore, an `option` value is never an object, and can be safely used as a
dependency for React hooks.

## You don't need an Effect

The above discussion was academic, because we [don't actually need an Effect to
handle user
events](https://react.dev/learn/you-might-not-need-an-effect#how-to-remove-unnecessary-effects).
Let's delete the call to `RR.useEffect1` and start over.

A better place to call `onApply` is from within the `form`'s `onSubmit`
callback. Replace the `discount` reactive value with a `getDiscount` function:

<<< Promo.re#get-discount

Call `getDiscount` within the `onSubmit` callback function:

<<< Promo.re#on-submit

Inside the render logic, change the input of the switch expression from
`discount` to `getDiscount(submittedCode)`:

```reason
{switch (discount) { // [!code --]
{switch (getDiscount(submittedCode)) { // [!code ++]
```

## Add `datasets` to `Demo`

To make it easier to see the different promo-related error messages, you can
create different collections of items. Add a `datasets` variable to `Demo`:

<<< Demo.re#datasets

Since the `burgers` value is only used in a single expression, we can move it
inside that expression:

<<< Demo.re#burger-expression

::: tip

OCaml makes it easy to  move variables closer to where their used. Unlike in
`JavaScript`, you can use `let` anywhere, even inside an expression.

:::

## Refactor `Demo`

Refactor `Demo` to render a different `Order` for each collection of items:

<<< Demo.re#refactor

You can delete the unused `items` value in `Order`.

---

Hot diggity! You've added the promo codes to your order confirmation widget,
just in time for Madame Jellobutter's International Burger Day promotions. In
the next chapter, we'll further polish the sandwich promotion logic.

## Overview

- tbd

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/order-with-promo/)
and [demo](https://react-book.melange.re/demo/src/order-with-promo/) for this chapter.

-----

[^1]: If you happen to need more than 7 dependencies, you can define your own
   binding function based on the [current binding
   functions](https://github.com/reasonml/reason-react/blob/713ab6cdb1644fb44e2c0c8fdcbef31007b37b8d/src/React.rei#L248-L255).
   We'll cover bindings in more detail [later](/todo).

[^2]: Recall that variant constructors with arguments also get turned into
    objects in the JS runtime.

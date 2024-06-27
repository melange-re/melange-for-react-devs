# Order with Promo

Now that you have a `Promo` component, you can add it to your `Order` component.
With that in place, customers can finally enter promo codes and enjoy discounts
on their orders.

## Add `discount` type

But first, let's see how to create an explicit type for the `discount` derived
variable inside `Promo`. We do so not because we have to, but because it will
give us some more insight into OCaml's type system.

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

<<< Demo.re{16-17,21}

Change `Index` to use the new `Demo` component:

<<< Index.re{7}

## Add `onApply` prop to `Promo`

Add an `onApply` prop to `Promo`, which will be invoked when a promo code is
successfully submitted and results in a discount:

<<< Promo.re#add-on-apply{3}

This causes a compilation error because the `Order` component doesn't provide a
value for that prop yet. We can temporarily silence the error by giving
`onApply` a default value:

<<< Promo.re#default-value{6}

The default value we've provided, `_ => ()`, is basically a no-op. We can remove
this default value once we're ready to use `onApply` inside `Order`.

## Add `React.useEffect1`

Your first instinct might be to add a `useEffect` hook that invokes `onApply`
when `discount` has a value of the form `` `Discount(value) ``:

<<< Promo.re#use-effect

## `useEffect*` functions

`React.useEffect1` is a one of the binding functions for React's [useEffect
hook](https://react.dev/reference/react/useEffect). The number `1` at the end of
the function indicates how many dependencies this function is supposed to take.
Accordingly, there are also `React.useEffect0`, `React.useEffect2`, etc, all the
way up to `React.useEffect7`[^1].

All `React.useEffect*` functions accept a callback as their first argument, the
type of which is:

```reason
unit => option(unit => unit)
```

The callback's return type is `option(unit => unit)` so that you can return a
cleanup function encased in `Some`. When you don't need to return a cleanup
function, you can just return `None`.

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

You see that they both take a tuple as their dependencies argument. That's
because the elements of OCaml tuples can be of different types (unlike OCaml
arrays, whose elements must all be of the same type). In the JS runtime, though,
tuples just get turned into JS arrays.

::: tip

Every time you add or remove a dependency from your `useEffect` hook, you'll
need to use a different `React.useEffect*` function (the one that corresponds to
how many dependencies you now have).

:::

## Why does `useEffect1` accept an array?

As you've seen, `React.useEffect2`, `React.useEffect3`, etc all accept a tuple
argument for their dependencies. However, `React.useEffect1` is the odd man out,
because it accepts an array! The reason is that one-element OCaml tuples don't
become arrays in the JS runtime, they instead take on the value of their single
element. So in this case, `React.useEffect1` must take an array so that the
generated JS code does the right thing. However, this creates the unfortunate
possibility that you accidentally pass in an empty array as the dependency for
`React.useEffect1`.

## `RR.useEffect1` helper function

You can sidestep this possibility by adding a helper function to your `RR`
module:

<<< RR.re#use-effect-1

You can refactor `Promo` to use your new helper function:

<<< Promo.re#use-effect-helper{1,11}


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
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/order-with-promo/)
and [demo](https://react-book.melange.re/demo/src/order-with-promo/) for this chapter.

-----

[^1]: If you happen to need more than 7 dependencies, you can define your own
   binding function based on the [current binding
   functions](https://github.com/reasonml/reason-react/blob/713ab6cdb1644fb44e2c0c8fdcbef31007b37b8d/src/React.rei#L248-L255).
   We'll cover bindings in more detail [later](/todo).

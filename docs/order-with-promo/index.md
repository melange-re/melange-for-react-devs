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

```reason
type discount = [>
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
];
```

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

```reason
type discount = [
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
];
```

This fixes the syntax error so that we now have a correctly-defined polymorphic
variant type.

## Type annotate `discount` variable

We just defined a new type, but our `discount` variable doesn't know anything
about it. Since polymorphic variants can be used without explicitly defining
them, type inference works differently for them. In particular, our `discount`
variable continues to use its own inferred type, despite there being a perfectly
good type within scope that uses the same tags.

Type annotate the `discount` variable with the newly-created `discount` type:

```reason
let discount = // [!code --]
let discount: discount = // [!code ++]
```

Now when you hover over the `discount` variable, you'll that its type is just
`discount`.

## Type constructor and type variable

Change the `discount` type to this:

```reason
type discount('a) = [
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError('a)
  | `NoSubmittedCode
];
```

Now `discount` is a *type constructor* that takes a *type variable* named `'a`.
A type constructor is not a fixed type---it's more useful to think of it as
function that takes a type and outputs a new type. This is reinforced by the
compilation error we get:

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
constructor, but the value of its argument should be inferred. Now if we hover
over the `discount` variable, we see that its type is:

```reason
discount([> `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ])
```

## The meaning of `>`

Once again, we see `>`. In polymorphic variant type expressions, it means "allow
more than". It means that tags other than the four that are listed are allowed.
For example, this type would be allowed:

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

```reason
type discount = [>
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
];
```

The reason is that whenever you have `>`, you implicitly have a type variable.
So the above code is equivalent to this:

```reason{13}
type discount = [>
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
] as 'a;
```

Now the error message makes a bit more sense:

> Error: A type variable is unbound in this type declaration.

The type variable exists, but it doesn't appear as an argument of the `discount`
type constructor. Once it's added, it compiles:

```reason{1}
type discount('a) =
  [>
    | `CodeError(Discount.error)
    | `Discount(float)
    | `DiscountError(
        [>
          | `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
        ],
      )
    | `NoSubmittedCode
  ] as 'a;
```

This is somewhat like accidentally using a variable in a function but forgetting
to add that variable to the function's argument list.



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

footnotes

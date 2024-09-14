# From Polymorphic to Normal Variant

The `Promo` component works fine now, but let's take a moment to explore how to
implement it with a normal variant instead of a polymorphic variant. This small
refactor will give us more insight into OCaml's type system. Additionally,
normal variants are better than polymorphic variants at "documenting" the types
that will be used in your program, since they must always be explicitly defined
before you can use them.

## Add `discount` type

Make a copy of your `Promo.re` file and then rename the copy to `PolyPromo.re`.
We'll make use of `PolyPromo.re` later, but for most of this chapter, we'll
focus our attention on `Promo.re`.

When we hover over the `discount` variable in `Promo.make`, we see this type
expression:

<<< Types.re#inferred-type

The easiest thing to do is to create a new `discount` type and assign it to the
above type expression, then delete the top-level backticks (`` ` ``) and
brackets (`[]`) to turn them into variant constructors:

<<< Types.re#bad-discount-type

::: tip

The alternative forms of a normal variant are called *variant constructors*,
while the forms of a polymorphic variant are called *variant tags*. They are
easy to tell apart because variant tags always start with a backtick (`` ` ``).

:::

However, this results in a compilation error:

<<< type-error.txt

We'll come back to this error message later. For now, observe that the error
disappears if we simply delete `>`:

<<< Types.re#delete-refinement

This fixes the syntax error so that we now have a correctly-defined normal
variant type.

## Refactor `discount` variable to use normal variant

Refactor the `discount` derived variable inside `Promo.make` to use our new
variant type by deleting all backticks in the switch expression:

<<< Promo.re#discount-variant{3,6,9-10}

Likewise, the render logic can be fixed by removing all backticks:

<<< Promo.re#discount-render{2-4,13}

## Type constructor and type variable

The current definition of the `discount` type works fine, but it is more verbose
than necessary. In particular, the `DiscountError` constructor's polymorphic
tags don't need to be fully specified:

<<< Types.re#delete-refinement{4-11}

The variant tags inside `DiscountError` originate from our `Discount` module, so
they shouldn't be needlessly constrained by the `discount` type defined inside
the `Promo` module.

Change the `discount` type to this:

<<< Types.re#type-variable

Now `discount` is a *type constructor* that takes a *type variable* named `'a`.
A type constructor is not a fixed type---you can think of it as a function that
takes a type and outputs a new type.

Using a type variable does not sacrifice type safety, if you hover over the
`discount` variable, you see that its type is now:

```reason
discount([> `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ])
```

This is essentially the same type as before (other than the `>` symbol,
explained in the next section), just written differently. By looking at the
usage of the `discount` variable, OCaml can infer how to fill in the type
variable and produce the type shown above.

The advantage of using a type variable for the definition of the
`Promo.discount` type is that when you add, rename, or delete variant tags in
`Discount`, you won't have to make corresponding edits to the `Promo.discount`
type[^1].

## `>` means open variant type

In the type expression above, we once again see `>`, which means that it's an
*open variant type*. In polymorphic variant type expressions, it means "allow
more tags than the ones listed here". In this case, it means that tags other
than the four that are listed are allowed. To illustrate, this type (which has
two extra tags) is compatible:

```reason{5-6}
discount([| `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
          | `HoneyButter
          | `KewpieMayo ])
```

This type (which is missing one of the tags) is not compatible:

```reason{5-6}
discount([| `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger ])
```

For reference, there are also *closed variant types*, denoted by `<`, which
define all possible tags, but compatible types don't need to have all of those
tags. An example:

```reason{1,9}
let getNoise = (animal: [< | `cat | `dog | `fox | `snake]) =>
  switch (animal) {
  | `cat => "meow"
  | `dog => "woof"
  | `snake => "sss"
  | `fox => "wowza"
  };

let fox: [ | `fox] = `fox;
Js.log(getNoise(fox));
```

The `getNoise` function accepts a type with as many as four tags, but it will
happily accept types that only have one tag (as long as that one tag is one of
the four tags).

When defining your own polymorphic variant types, you will most often use *exact
variant types*, which define all their tags, with no flexibility to add or
remove tags. Exact variant types don't have `>` or `<` in their type
expressions.

Now that you know about open and closed variants, you might have intuited that
polymorphic variants are quite complex. The OCaml compiler agrees with you! Code
containing polymorphic variants takes a little longer to compile and run,
although for most programs the difference is not noticeable.

## Implicit type variable

Let's come back to the question of why the original attempt at a variant type
definition was semantically invalid:

<<< Types.re#bad-discount-type

The reason is that there's an implicit type variable around the `>`. The
above type expression is equivalent to:

<<< Types.re#explicit-type-var{14}

Now the error message makes a bit more sense:

<<< type-error.txt{7}

The type variable exists, but it's pointless unless it appears as an argument of
the `discount` type constructor. Once it's added, it compiles:

<<< Types.re#add-type-arg{1}

This is somewhat like accidentally using a variable in a function but forgetting
to add that variable to the function's argument list.

## Force `DiscountError` argument to be polymorphic variant

Right now the argument of the `DiscountError` constructor can be any type at
all, but to be explicit, we can force it to be a polymorphic variant:

<<< Types.re#must-be-poly{4}

The `[> ]` type expression denotes an open variant type with no tags. Since open
variant types allow more tags, this expression is compatible with any
polymorphic variant. Adding this small restriction is just a way to make it
clear that `DiscountError`'s argument must be a polymorphic variant.

## Type-related error messages

Let's introduce some type errors and compare the error messages between the
normal variant and polymorphic variant versions of the code.

In `Promo.re`, change the `NoSubmittedCode` branch of the
`getDiscount(submittedCode)` switch expression to introduce a misspelling:

```reason{2}
{switch (getDiscount(submittedCode)) {
 | NoSubmittedCo => React.null
 | Discount(discount) => discount |> Float.neg |> RR.currency
```

Change the equivalent line in `PolyPromo.re` in the same way. You should see
this error message:

```text{25-26}
File "src/order-confirmation/PolyPromo.re", lines 52-75, characters 4-7:
52 | ....{switch (discount) {
53 |      | `NoSubmittedCo => React.null
54 |      | `Discount(discount) => discount |> Float.neg |> RR.currency
55 |      | `CodeError(error) =>
56 |        <div className=Style.codeError>
...
72 |        <div className=Style.discountError>
73 |          {RR.s({j|Buy $buyWhat to enjoy this promotion|j})}
74 |        </div>;
75 |      }}
Error (warning 8 [partial-match]): this pattern-matching is not exhaustive.
Here is an example of a case that is not matched:
`NoSubmittedCode

File "src/order-confirmation/Promo.re", line 66, characters 7-20:
66 |      | NoSubmittedCo => React.null
            ^^^^^^^^^^^^^
Error: This variant pattern is expected to have type
         [> `MissingSandwichTypes of string list
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ]
         discount
       There is no constructor NoSubmittedCo within type discount
Hint: Did you mean NoSubmittedCode?
Had 2 errors, waiting for filesystem changes...
```

The two error messages are very similar, but the normal variant error is a
little nicer. It points out that there's no `NoSubmittedCo` constructor in type
`discount`, and it gives you a friendly hint that maybe you meant to use
`NoSubmittedCode` instead (which is the case).

Restore your code to its previous state so that there are no compilation errors.
Then add a new `FooBar` branch to the `getDiscount(submittedCode)` switch
expression:

```reason{2}
{switch (getDiscount(submittedCode)) {
 | FooBar => React.null
 | NoSubmittedCode => React.null
```

Make the equivalent change in `PolyPromo.re`, and you should see a single
compilation error:

```text{10}
File "src/order-confirmation/Promo.re", line 66, characters 7-13:
66 |      | FooBar => React.null
            ^^^^^^
Error: This variant pattern is expected to have type
         [> `MissingSandwichTypes of string list
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ]
         discount
       There is no constructor FooBar within type discount
Had 1 error, waiting for filesystem changes...
```

OCaml knows that the `Promo.discount` type doesn't have a `FooBar` constructor,
but it doesn't know that the polymorphic variant used in `PolyPromo` doesn't
have a `` `FooBar`` tag.

In general, error messages involving polymorphic variants are a bit less
helpful, and some type-related mistakes won't lead to compilation errors. For
simple use cases, it's often better to use normal variants, as they force you to
be more disciplined.

## `discount` type inside a module

Let's refactor the `discount` type so that it's inside a module:

<<< Promo.re#model-module

This will cause the following compilation error:

```text
File "src/order-with-promo/Promo.re", line 34, characters 14-29:
34 |     | None => NoSubmittedCode
                   ^^^^^^^^^^^^^^^
Error: Unbound constructor NoSubmittedCode
```

OCaml no longer knows where to find the `NoSubmittedCode` constructor, because
it's in the scope of the `Model` submodule.

---

Phew! You refactored the `discount` derived variable to use a normal variant
instead of a polymorphic variant. The code changes were fairly minimal, but to
understand what was happening, it was necessary to learn the basics of type
constructors and type variables. In the next sections, we'll set types and other
theoretical considerations aside and get into the nitty-gritty of the UI changes
you must make to add promo support to the `Order` component.

## Overview

- A type constructor takes a type and outputs another type
- A type variable is a variable that stands in a for a type and often appears in
  type constructors or type signatures
- In polymorphic variant type expressions,
  - `>` means it's an open variant type that can accept more than the variant
    tags that are listed
  - `<` means it's a closed variant type that can accept fewer than the variant
    tags that are listed
- You rarely need to use `>` or `<` in your own type definitions, but they
  appear in inferred type definitions which you'll see in compiler error
  messages or when you hover over variables/functions
- Inferred type definitions that contain `>` or `<` also have an implicit type
  variable

## Exercises

<b>1.</b> The following code
([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGdldE5hbWUgPSAoYW5pbWFsOiBbfCBgQ2F0IHwgYERvZyhpbnQpIHwgYFVuaWNvcm4oc3RyaW5nKV0pID0%2BCiAgc3dpdGNoIChhbmltYWwpIHsKICB8IGBDYXQgPT4gIk1yIFdoaXNrZXJzIgogIHwgYERvZyhuKSA9PiAiQmFuZGl0ICIgKysgc3RyaW5nX29mX2ludChuKQogIHwgYFVuaWNvcm4obmFtZSkgPT4gIlNpciAiICsrIG5hbWUKICB9Ow%3D%3D&live=off))
doesn’t compile. Fix it by adding a single character.

```reason
let getName = (animal: [| `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  };
```

::: details Hint

Find a place where you can insert a space.

:::

::: details Solution

```reason{1}
let getName = (animal: [ | `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  };
```

A common mistake when writing polymorphic variant type definitions is forgetting
to put a space between the `[` and the `|` characters. Note that you don't need
to add the implicit type variable in type annotations.

::: warning

In the next version of [Reason](https://reasonml.github.io/), polymorphic
variant definitions no longer require a space between `[` and `|`.

:::

<b>2.</b> The following code
([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGdldE5hbWUgPSAoYW5pbWFsOiBbIHwgYENhdCB8IGBEb2coaW50KSB8IGBVbmljb3JuKHN0cmluZyldKSA9PgogIHN3aXRjaCAoYW5pbWFsKSB7CiAgfCBgQ2F0ID0%2BICJNciBXaGlza2VycyIKICB8IGBEb2cobikgPT4gIkJhbmRpdCAiICsrIHN0cmluZ19vZl9pbnQobikKICB8IGBVbmljb3JuKG5hbWUpID0%2BICJTaXIgIiArKyBuYW1lCiAgfCBgRHJhZ29uID0%2BICJQdWZmIHRoZSBNYWdpYyIKICB9Ow%3D%3D&live=off))
doesn’t compile. Fix it by adding a single character.

```reason
let getName = (animal: [ | `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  | `Dragon => "Puff the Magic"
  };
```

::: details Solution

```reason{1}
let getName = (animal: [> | `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  | `Dragon => "Puff the Magic"
  };
```

Adding a `>` to the polymorphic variant type definition allows it to accept more
than the listed variant tags.

:::

<b>3.</b> Fix the following code ([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=LyoqIE9ubHkgaW52b2tlIFtmXSB3aGVuIFtvMV0gYW5kIFtvMl0gYXJlIFtTb21lXSAqLwpsZXQgbWFwMjogKG9wdGlvbignYSksIG9wdGlvbignYSksICgnYSwgJ2EpID0%2BICdhKSA9PiBvcHRpb24oJ2EpID0KICAobzEsIG8yLCBmKSA9PgogICAgc3dpdGNoIChvMSwgbzIpIHsKICAgIHwgKE5vbmUsIE5vbmUpCiAgICB8IChOb25lLCBTb21lKF8pKQogICAgfCAoU29tZShfKSwgTm9uZSkgPT4gTm9uZQogICAgfCAoU29tZSh2MSksIFNvbWUodjIpKSA9PiBTb21lKGYodjEsIHYyKSkKICAgIH07CgpKcy5sb2cobWFwMihTb21lKDExKSwgU29tZSgzMyksICgrKSkpOwpKcy5sb2cobWFwMihTb21lKCJBQkMiKSwgU29tZSgxMjMpLCAoYSwgYikgPT4gKGEsIGIpKSk7&live=off)) which fails to compile:

```reason
/** Only invoke [f] when [o1] and [o2] are [Some] */
let map2: (option('a), option('a), ('a, 'a) => 'a) => option('a) =
  (o1, o2, f) =>
    switch (o1, o2) {
    | (None, None)
    | (None, Some(_))
    | (Some(_), None) => None
    | (Some(v1), Some(v2)) => Some(f(v1, v2))
    };

Js.log(map2(Some(11), Some(33), (+)));
Js.log(map2(Some("ABC"), Some(123), (a, b) => (a, b)));
```

::: details Hint 1

Fix the type annotation.

:::

::: details Hint 2

Delete the type annotation.

:::

::: details Solution

```reason{2}
/** Only invoke [f] when [o1] and [o2] are [Some] */
let map2: (option('a), option('b), ('a, 'b) => 'c) => option('c) =
  (o1, o2, f) =>
    switch (o1, o2) {
    | (None, None)
    | (None, Some(_))
    | (Some(_), None) => None
    | (Some(v1), Some(v2)) => Some(f(v1, v2))
    };

Js.log(map2(Some(11), Some(33), (+)));
Js.log(map2(Some("ABC"), Some(123), (a, b) => (a, b)));
```

We have to use different type variables if we expect that the types might be
different. Note that we could have deleted the type annotation and then OCaml's
inferred type would be the same as the type annotation above.

:::

<b>4.</b> When you enter a promo code that is blank (empty or consists of only
spaces) and hit Enter, it will still show the “Invalid promo code” error. Fix it
so that it shows no error in this case.

::: details Hint

Use
[Js.String.trim](https://melange.re/v4.0.0/api/re/melange/Js/String/#val-trim)
to help you check whether a string is blank.

:::

::: details Solution

One way to do it is to add a new branch with a `when` guard in `Promo.make`'s
first switch expression:

<<< Promo.re#when-solution{4}

A better solution is to modify the `form` element's `onSubmit` callback:

<<< Promo.re#onsubmit-solution{4}

The second solution is simpler and more direct, but both work.

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/poly-to-normal-variant/)
and [demo](https://react-book.melange.re/demo/src/poly-to-normal-variant/) for
this chapter.

-----

[^1]: Admittedly, this is a small advantage, because you still must handle any
    changes to `Discount`'s variant tags inside the `Promo.make` function's
    switch expression.

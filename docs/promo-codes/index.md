# Promo Codes

As International Burger Day looms ever closer, Madame Jellobutter is eager to
start her burger-related promotions. She bought ads on local billboards and she
even wore a giant burger costume to pass flyers at a local music festival[^1].
Depending on the ad, potential customers will either see the promo code FREE,
which corresponds to the "buy n burgers, get n/2 burgers free" discount, or
HALF, which corresponds to the "buy a burger with everything on it and get half
off the entire order" discount.

## `getDiscountFunction` function

Add a new function that maps a given promo code to its corresponding discount
function:

<<< Discount.re#simple-get-discount

Instead of
[Js.String.toUpperCase](https://melange.re/v4.0.0/api/re/melange/Js/String/#val-toLowerCase),
we could've used
[String.uppercase_ascii](https://melange.re/v4.0.0/api/re/melange/Stdlib/String/#val-uppercase_ascii),
but as its name implies, `String.uppercase_ascii` can only handle strings
containing ASCII characters. This is a common restriction for functions in the
[Stdlib.String](https://melange.re/v4.0.0/api/re/melange/Stdlib/String/) module,
so for most string operations, you should prefer the functions in
[Js.String](https://melange.re/v4.0.0/api/re/melange/Js/String/).

Madame Jellobutter informs you that the FREE promotion is only active during the
month of May, so you change `Discount.getDiscountFunction` accordingly:

<<< Discount.re#get-discount-with-date{1,5}

## `result` type

The function now takes a `date` argument and returns `None` if the promo code is
FREE and the given date isn't within the month of May. This logic is correct,
but there's still a flaw: when an error occurs, there's no way to inform the
user of the reason for error. Whether the user misspells the promo code or
enters it on June 1, the only feedback they'd get is that it doesn't work, but
they wouldn't know why. We can remedy this by returning `result` instead of
`option`:

<<< Discount.re#get-discount-return-result{6,8}

## `option` vs `result`

Other than the name, the `Ok` constructor of `result` is semantically equivalent
to the `Some` constructor of `option`. So the main difference is that the
`Error` constructor of `result` takes an argument, while the `None` constructor
of `option` cannot take an argument. In short, `None` can only signal that
there's an error, but `Error` can signal **what the error is**.

::: tip

We don't need to type annotate anything when using the `Ok` and `Error`
constructors because they are always in scope, due to the [result
type](https://melange.re/v4.0.0/api/re/melange/Stdlib/#type-result) being
defined in `Stdlib` (which is open by default).

:::

## Test for invalid promo codes

Add tests for `Discount.getDiscountFunction` in a new submodule called
`DiscountTests.GetDiscount`:

```reason
module GetDiscount = {
  test("Invalid promo code return Error", () => {
    let date = Js.Date.make();
    ["", "FREEDOM", "UNICORN", "POO"]
    |> List.map(code =>
         expect
         |> equal(
              Discount.getDiscountFunction(code, date),
              Error("Invalid code"),
            )
       );
  });
};
```

Here we iterate over a few invalid promo codes using `List.map` and check that
`getDiscountFunction` returns `Error("Invalid code")` for all of them.

## Native syntax in error messages

Regrettably, this code doesn't compile:

```text
File "docs/order-confirmation/DiscountTests.re", lines 7-14, characters 4-9:
 7 | ....["", "FREEDOM", "UNICORN", "POO"]
 8 |     |> List.map(code =>
 9 |          expect
10 |          |> equal(
11 |               Discount.getDiscountFunction(code, date),
12 |               Error("Invalid code"),
13 |             )
14 |        );
Error: This expression has type unit list
       but an expression was expected of type unit
```

This error message might seem a little cryptic because it's using native OCaml
syntax and not the Reason syntax that we cover in this book. For reference, this
is how the type notations map between native and Reason syntaxes:

| Native syntax | Reason syntax |
|---------------|---------------|
| `unit list` | `list(unit)` |
| `float option` | `option(float)` |
| `int option list` | `list(option(int))` |

Basically, when you see nested types in error messages, reverse the order of the
types and add parentheses to translate it from native to Reason syntax.

## `List.iter` function

Returning to the error message, if we translate it to Reason syntax, it's
saying:

> Error: This expression has type `list(unit)` but an expression was expected of
> type `unit`

Recall that OCaml considers it an error if we generate a value but don't do
anything with it. So the easiest way to fix this is by purposefully discarding
the value using `ignore`:

<<< DiscountTests.re#ignore-list

An even better solution would be to use
[List.iter](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-iter):

<<< DiscountTests.re#list-iter{2}

`List.iter` runs a side-effect function on each element of a list, and it
returns `unit`, which doesn't need to be ignored.

## Runtime representation of `result`

Run the tests (`npm run test`) and you'll get an error (truncated for clarity):

```diff
+      Values have same structure but are not reference-equal:
+
+      {
+        TAG: 1,
+        _0: 'Invalid code'
+      }
+
+    code: 'ERR_ASSERTION'
+    name: 'AssertionError'
+    expected:
+      TAG: 1
+      _0: 'Invalid code'
+    actual:
+      TAG: 1
+      _0: 'Invalid code'
```

The `result` type is a variant type with two constructors `Ok` and `Error`, and
both of these constructors have an argument. This means that both of them are
represented as objects in the JavaScript runtime, e.g.:

| OCaml source | JavaScript runtime |
|--------------|--------------------|
| `Ok(4567)` | `{TAG: 0, _0: 4567}` |
| `Error("Expired code")` | `{TAG: 1, _0: "Expired code"}` |

Since we are comparing objects and not values, we must use
[Fest.deepEqual](https://ahrefs.github.io/melange-fest/reason/Fest/index.html#val-deepEqual)
instead of
[Fest.equal](https://ahrefs.github.io/melange-fest/reason/Fest/index.html#val-equal):

```reason
expect
|> equal( // [!code --]
|> deepEqual( // [!code ++]
    Discount.getDiscountFunction(code, date),
    Error("Invalid code"),
  )
```

## Test for expired promo codes

Add a new test in `DiscountTests.GetDiscount` that checks the temporal behavior
of `getDiscountFunction`:

<<< DiscountTests.re#free-promo-may

This test loops over every month of the year:

- If the month is May, then `Discount.getDiscountFunction("FREE", date)` should
  return `Ok(Discount.getFreeBurgers)`
- If the month isn't May, then it should return `Error("Expired code")`

## For loop

This is the first for loop we've seen so far! This is one of the few scenarios
in OCaml where a for loop makes sense: iterating over a sequence of numbers and
calling a side-effect function on each number.

## Add `error` type

Right now, `Discount.getDiscountFunction` returns two types of errors:
`Error("Expired code")` and `Error("Invalid code")`, which have the same type.
Our code will be less brittle if these two different kinds of errors also have
different types. Add a new type `error` to `Discount`:

<<< Discount.re#error-type

After which you can update `Discount.getDiscountFunction` to use the new type
inside the `Error` constructor:

<<< Discount.re#use-error-type{6,8}

Returning different error constructors encased by `Error` is the safe
alternative to raising different exceptions.

## Refactor discount functions

Update `Discount.getFreeBurgers` to also use `result` instead of `option`:

<<< Discount.re#get-free-burgers{13-14,21}

This is a straightforward refactor, but to fix your tests, you would need to
reproduce the error messages verbatim in `DiscountTests`, e.g.:

```reason
test("0 burgers, no discount", () =>
  expect
  |> equal(
        Discount.getFreeBurgers([
          Hotdog,
          Sandwich(Ham),
          Sandwich(Turducken),
        ]),
        Error("To enjoy this promo, buy at least 2 burgers"),
      )
);
```

You would need to change your tests every time the error message changes, which
is annoying. But error messages don't necessarily need to be inside
`Discount.getFreeBurgers`, there should be a way to indicate what an error is
without using the actual error message.

## Polymorphic variants

Instead of using a string as the argument for the `Error` constructor, you can
instead use a *polymorphic variant*:

<<< Discount.re#get-free-burgers-poly{13-14}

Polymorphic variants are similar to the variants you've seen before, the main
difference being that you don't need to declare the constructors beforehand.
Instead, you can freely use polymorphic variant constructors inside a function,
and the type of the function is inferred from the usage. For example, the type
of `Discount.getFreeBurgers` is now

```reason
list(Item.t) => result(float, [> `NeedOneBurger | `NeedTwoBurgers ])
```

::: tip

The name of a polymorphic variant constructor must start with the backtick (`` `
``) character.

:::

Refactor `Discount.getHalfOff` to return `result` as well[^2]:

```reason
switch (meetsCondition) {
| false => None // [!code --]
| false => Error(`NeedMegaBurger) // [!code ++]
| true =>
  let total =
    items
    |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
          total +. Item.toPrice(item)
        );
  Some(total /. 2.0); // [!code --]
  Ok(total /. 2.0); // [!code ++]
```

The type of `Discount.getHalfOff` is now

```reason
list(Item.t) => result(float, [> `NeedMegaBurger ])
```

## Fixing the tests

Fixing the tests is mostly a mechanical and consists of these steps:

- Replace `Some` with `Ok`
- Replace `None` with ``Error(`SomeConstructor)``
- Replace `equal` with `deepEqual`

However, there is a little wrinkle. What if you misspell one of the polymorphic
variant constructors?

```reason{5}
test("1 burger, no discount", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurgers([Hotdog, Sandwich(Ham), Burger(burger)]),
       Error(`NeedOneBurgers),
     );
```

The constructor should be called `` `NeedOneBurger`` but here it's been
misspelled as `` `NeedOneBurgers``, yet it still compiles! This is due to the
open-ended nature of polymorphic variants. The compiler knows that
`Discount.getFreeBurgers` currently can't return ``Error(`NeedOneBurgers)``, but
because the constructors for polymorphic variants don't need to be defined
up-front, it has no way to know that the function will **never** return
``Error(`NeedOneBurgers)`` in the future.

## Runtime representation of polymorphic variants

If you run the test with the misspelling, you'll get this error (shortened for
clarity):

```diff
+    expected:
+      TAG: 1
+      _0: 'NeedOneBurgers'
+    actual:
+      TAG: 1
+      _0: 'NeedOneBurger'
```

You can see that polymorphic variant constructors without arguments are just
strings in the JS runtime.

For reference, the runtime representation for polymorphic variant
constructors with arguments is an object with `NAME` and `VAL` fields. For
example, `` `foo(42)`` becomes `{"NAME": "foo", "VAL": 42}`.

## Variants vs polymorphic variants

Variants are always going to be more reliable than polymorphic variants because
you must define them using `type` before you can use them, meaning if you ever
misspell a constructor, you'll get an error. Polymorphic variants are more
flexible and convenient but allow you to make silly mistakes. However, we will
see in the next chapter that polymorphic variants are just as type-safe as
variants.

---

Espectáculo! You've made your discount logic more expressive by converting it to
use `result` instead of `option`. You've also started to experience the power of
polymorphic variants. In the next chapter, we'll build a new ReasonReact
component that uses our discount functions.

## Overview

- The `result` type is similar to `option`, except that the "failure"
  constructor takes an argument
  - The `Ok` constructor becomes an object in the JS runtime
  - The `Error` constructor becomes an object in the JS runtime
  - Inside tests, `result` values must be compared using `Fest.deepEqual`
- Functions in the `Stdlib.String` module won't always handle Unicode characters
correctly, so prefer to use the functions in `Js.String`
- Compilation error messages use OCaml's native syntax when describing types
  - Convert from native to Reason syntax by reversing the order of the types and
    inserting parentheses between them
- `List.iter` is useful for running a side-effect function on every element of a
  list. It's better than `List.map` for this use case because the return value
  doesn't need to be `ignore`d.
- For loops are useful for the narrow use case of running a side-effect function
  on a sequence of numbers
- Polymorphic variants are like the variants we've already seen but with some
  differences:
  - The constructors don't need to defined before they are used
  - The constructors must start with the backtick (`` ` ``) character
  - A polymorphic variant constructor without arguments becomes a string in the
    JS runtime
  - A polymorphic variant constructor with argument(s) becomes an object in the
    JS runtime. The keys in the object are `NAME` and `VAL`.

## Exercises

<b>1.</b> Rewrite the “FREE promo code works in May but not other months” to use
`List.iter` instead of a for loop.

::: details Hint

Use
[List.init](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-init).

:::

::: details Solution

<<< DiscountTests.re#free-promo-may-list-iter

:::

<b>2.</b> Modify `Discount.getDiscountFunction` so that it returns
`Ok(getHalfOff)` only if the promo code is "HALF" and the date is May 28,
International Burger Day. Make sure it passes the following test:

<<< DiscountTests.re#half-promo-may-28

::: details Solution

<<< Discount.re#half-promo-may-28

:::

<b>3.</b> Add a new function `Discount.getSandwichHalfOff` that takes half off
the entire order if you order at least one of every type of sandwich. Make sure
it passes the following tests:

<<< DiscountTests.re#get-sandwich-half-off

::: details Hint 1

Add a new record type that keeps track of which sandwich types have been
encountered in the order.

:::

::: details Hint 2

Use
[List.filter_map](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-filter_map)
and
[ListLabels.fold_left](https://melange.re/v4.0.0/api/re/melange/Stdlib/ListLabels/#val-fold_left).

:::

::: details Solution

<<< Discount.re#get-sandwich-half-off

:::

<b>4.</b> Modify `Discount.getDiscountFunction` so that it returns
`Ok(getSandwichHalfOff)` only if the promo code is "HALF" and the date is
November 3, World Sandwich Day. Make sure it passes the following test:

<<< DiscountTests.re#half-promo-nov-3

::: details Solution

<<< Discount.re#half-promo-nov-3

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/promo-codes/)
and [demo](https://react-book.melange.re/demo/src/promo-codes/) for this chapter.

-----

[^1]: It was quite a sight to see a giant burger zipping around the fairgrounds
    on a Segway while being chased by a small army of juggalos.

[^2]: Instead of creating a polymorphic variant constructor out of the phrase
    "burger that has every topping", we save ourselves some typing by using the
    much shorter "megaburger".

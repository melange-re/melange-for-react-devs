# Promo Codes

As International Burger Day looms ever closer, Madame Jellobutter is eager to
start her burger-related promotions. She bought ads on local billboards and she
even put on a giant burger costume to pass flyers at a local music festival[^1].
Depending on the ad, potential customers will either see the promo code FREE,
which corresponds to the "buy n burgers, get n/2 burgers free" discount, or
HALF, which corresponds to the "buy a burger with everything on it and get half
off the entire order" discount.

## `getDiscountFunction` function

Let's start by adding a function that maps a given promo code to its
corresponding discount function:

<<< Discount.re#simple-get-discount

Instead of
[Js.String.toUpperCase](https://melange.re/v3.0.0/api/re/melange/Js/String/#val-toLowerCase),
we could've used
[String.uppercase_ascii](https://melange.re/v3.0.0/api/re/melange/Stdlib/String/#val-uppercase_ascii),
but its name implies, `String.uppercase_ascii` can only handle strings
containing ASCII characters. This is a common restriction for functions in the
[Stdlib.String](https://melange.re/v3.0.0/api/re/melange/Stdlib/String/) module,
so for most string operations you should prefer the functions in
[Js.String](https://melange.re/v3.0.0/api/re/melange/Js/String/).

Madame Jellobutter informs you that the FREE promotion is only active during the
month of May, so you change `Discount.getDiscountFunction` accordingly:

<<< Discount.re#get-discount-with-date{1,5-6}

## `result` type

The function now takes a `date` argument and returns `None` if the promo code is
FREE and the given date isn't within the month of May. This logic is technically
correct, but there's a slight problem: it leaves no way to inform the user of
the reason for the promo code not working. Whether the user misspelled the promo
code or entered it on June 1, the only feedback they'd get is that it didn't
work, but they wouldn't know why. We can remedy this by returning `result`
instead of `option`:

<<< Discount.re#get-discount-return-result{6,8}

Basically, we replaced `Some` with `Ok` and `None` with `Error`. But when we
return the `Error` constructor, we also supply a specific error message that can
be shown in the UI. We didn't need to type annotate anything in this function
because both the `Ok` and `Error` constructors of the
[Stdlib.result](https://melange.re/v3.0.0/api/re/melange/Stdlib/#type-result)
type are always in scope (since `Stdlib` is opened by default).

## Test for invalid promo codes

Let's add tests for `Discount.getDiscountFunction` in a new submodule called
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

But this code won't compile:

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

The error message might seem slightly cryptic because it's using native OCaml
syntax and not the Reason syntax that we cover in this book. For reference, this
is how the type notations map between native and Reason syntaxes:

| Native syntax | Reason syntax |
|---------------|---------------|
| `unit list` | `list(unit)` |
| `float option` | `option(float)` |
| `int option list` | `list(option(int))` |

Basically, when you see nested types in error messages, reverse the order of the
types and add parentheses.

## `List.iter` function

Returning to the error message, if we translate it to Reason syntax, it says:

> Error: This expression has type `list(unit)` but an expression was expected of
> type `unit`

Recall that OCaml considers it an error if we generate a value but don't do
anything with it. So the easiest way to fix this is by purposefully discarding
the value using `ignore`:

<<< DiscountTests.re#ignore-list

An even better solution would be to use
[List.iter](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-iter):

<<< DiscountTests.re#list-iter{2}

`List.iter` runs a side-effect function on each element of a list, and it
returns `unit`, which doesn't need to be ignored.

## Runtime representation of `result`

Run the tests (`npm run test`) and you'll get an error:

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
represented as objects in the JavaScript runtime:

| OCaml source | JavaScript runtime |
|--------------|--------------------|
| `Ok(4567)` | `{TAG: 0, _0: 4567}` |
| `Error("Expired code")` | `{TAG: 1, _0: "Expired code"}` |

Since we are comparing objects, we must rely on
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
But our code will be less brittle if these two different kinds of errors also
have different types. Add a new type `error` to `Discount`:

<<< Discount.re#error-type

After which you can update `Discount.getDiscountFunction` to use the new type
inside the `Error` constructor:

<<< Discount.re#use-error-type{6,8}

## Refactor discount functions

Update `Discount.getFreeBurgers` to use result instead of option:

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

You would need to change your tests every time the error messages change, but
that's a presentation issue, not directly related to a function whose primary
job is to calculate and return a number.

## Polymorphic variants

Instead of using a string as the argument for the `Error` constructor, you can
instead use a *polymorphic variant*:

<<< Discount.re#get-free-burgers-poly{13-14}

Polymorphic variants are similar to the variants you've seen before, the main
difference being that you don't need to declare the constructors beforehand.
Instead, you can just freely use polymorphic variant constructors inside a
function, and the type of the function is inferred from the usage. For example,
the type of `Discount.getFreeBurgers` is now

```reason
list(Item.t) => result(float, [> `NeedOneBurger | `NeedTwoBurgers ])
```

::: tip

The name of a polymorphic variant constructor must start with the backtick (`` `
``) character.

:::

Of course, we could have used normal variants here, but we would've needed to
define a new type that contains one constructor for every error message we'd
need, something like

```reason
type discountError =
  | NeedOneBurger
  | NeedTwoBurgers;
```

---

Summary

## Overview

tbd

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/promo-codes/)
and [demo](https://react-book.melange.re/demo/src/promo-codes/) for this chapter.

-----

[^1]: It was quite a sight to see a giant burger running around a music
    festival, being chased by juggalos!

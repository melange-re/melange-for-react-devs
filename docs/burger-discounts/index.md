# Burger Discounts

International Burger Day falls on Tuesday of next week, so Madame Jellobutter
decides to celebrate it by running the classic "buy 2 burgers get 1 free"
promotion. Add a new file `src/order-confirmation/Discount.re`:

```reason
// Buy 2 burgers, get 1 free
let getFreeBurger = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.sortInPlaceWith(~f=(item1, item2) =>
        - compare(Item.toPrice(item1), Item.toPrice(item2))
      )
    |> Js.Array.filter(~f=item =>
        switch (item) {
        | Burger(_) => true
        | Sandwich(_)
        | Hotdog => false
        }
      );

  switch (burgers) {
  | [|Burger(_), Burger(cheaperBurger)|] =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
};
```

## `Discount.getFreeBurger` function

The new function `Discount.getFreeBurger` takes an array of items, finds the
second-most-expensive burger, and returns its price encased in `Some`. If there
is no second burger, it returns `None`.

The functions used in `Discount.getFreeBurger` are:

- [Js.Array.sortInPlaceWith](https://melange.re/v3.0.0/api/re/melange/Js/Array/index.html#val-sortInPlaceWith)
  takes a callback function `~f` with type signature `('a, 'a) => int` (accept
  two arguments of the same type and `int`). It's used to used to sort the items
  by price (highest to lowest).
- [Stdlib.compare](https://melange.re/v3.0.0/api/re/melange/Stdlib/#val-compare)
  has type signature `('a, 'a) => int`. It's a polymorphic function capable of
  comparing many types, including `bool`, `int`, `string`, etc. Note that you
  can always just write `compare` instead of `Stdlib.compare`, because the
  [`Stdlib` module is always opened by
  default](https://melange.re/v3.0.0/api/re/melange/Stdlib/).
- [Js.Array.filter](https://melange.re/v3.0.0/api/re/melange/Js/Array/#val-filter)
  takes a callback function `~f` with type signature `'a => bool`. It's used to
  make sure all items in the `burgers` array are all burgers.

At the moment, this code doesn't compile, and that's not the only thing wrong
with it, but we'll address each issue in due course.

## Limitation of type inference

You should see this compilation error:

```
File "src/order-confirmation/Discount.re", line 9, characters 11-17:
9 |          | Burger(_) => true
               ^^^^^^
Error: Unbound constructor Burger
```

OCaml's type inference isn't able to figure out that the callback function
passed to `Js.Array.filter` receives an argument of `Item.t`, so it doesn't know
where the `Burger` constructor is coming from. But why does type inference work
in the callback to `Js.Array.sortInPlaceWith`?

```reason
|> Js.Array.sortInPlaceWith(~f=(item1, item2) =>
    - compare(Item.toPrice(item1), Item.toPrice(item2))
  )
```

The reason is that `Item.toPrice` is invoked inside this callback, and its type
signature is already known to be `Item.t => float`. So type inference can figure
out that `item1` and `item2` must both be of type `Item.t`, because
`Item.toPrice` can only accept an argument of type `Item.t`.

## Type annotate callback argument

There aren't any function invocations inside the callback to `Js.Array.filter`,
so we can help the compiler out by type annotating the `item` argument:

<<< Discount.re#type-annotate-argument{1}

Explicit type annotation always works, but it's sometimes enough to just give
the compiler a hint.

## Use full name

A less direct way to indicate the type of `item` is to use the full name[^1] for the
constructors used in the switch expression:

<<< Discount.re#full-name-constructors{3-5}

Because `Item.Burger` is a constructor of the `Item.t` variant type, `item` must
have type `Item.t`. For the sake of convenience, you leave off `Item.` the
second and third branches of the switch expression---OCaml is smart to know what
module the following constructors come from.

<<< Discount.re#full-name-constructor{3}

By using the full name of the `Burger` constructor, we can also refactor the
callback function to use the `fun` syntax:

<<< Discount.re#full-name-fun

## Add new tests

Add some tests in new file `src/order-confirmation/DiscountTests.re`:

<<< DiscountTests.re#first-three

To run these tests, add a new cram test to `src/order-confirmation/tests.t`:

```cram
Discount tests
  $ node ./output/src/order-confirmation/DiscountTests.mjs | sed '/duration_ms/d'
```

Run `npm run test:watch` to see that the unit tests pass, then run `npm promote`
to make the cram test pass.

## Records are immutable

It's unnecessary to fully write out every burger record in the tests. Define a
`burger` record at the very top of `DiscountTests`:

<<< DiscountTests.re#burger-record

Then refactor the second and third tests to use this record:

<<< DiscountTests.re#refactor-use-burger-record{4,14,16}

It's safe to reuse a single record this way because records are immutable. You
can pass a record to any function and never worry that its fields might be
changed by that function.

## Record copy syntax

Add a new test to `DiscountTests`:

<<< DiscountTests.re#different-price-test

Again, we're reusing the `burger` record, but this time, we use [record copy
syntax](https://reasonml.github.io/docs/en/record#updating-records-spreading) to
make copies of `burger` record that have slightly different field values. For
example,

```reason
{...burger, tomatoes: true}
```

means to make a copy of `burger` but with `tomatoes` set to `true`. It's just a
shorter and more convenient way to write this:

```reason
{
  lettuce: burger.lettuce,
  onions: burger.onions,
  cheese: burger.cheese,
  bacon: burger.bacon,
  tomatoes: true,
}
```

## Ignoring function return values

Add another test to `DiscountTests` that checks whether `Discount.getFreeBurger`
modifies the array passed to it:

```reason
test("Input array isn't changed", () => {
  let items = [|
    Item.Hotdog,
    Burger({...burger, tomatoes: true}),
    Sandwich(Ham),
    Burger({...burger, bacon: 2}),
  |];

  Discount.getFreeBurger(items);

  expect
  |> deepEqual(
       items,
       [|
         Item.Hotdog,
         Burger({...burger, tomatoes: true}),
         Sandwich(Ham),
         Burger({...burger, bacon: 2}),
       |],
     );
});
```

You'll get this compilation error:

```
File "src/order-confirmation/DiscountTests.re", line 65, characters 2-31:
65 |   Discount.getFreeBurger(items);
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Error: This expression has type float option
       but an expression was expected of type unit
       because it is in the left-hand side of a sequence
```

When you call a function in OCaml, you have use its return value, unless the
return value is `()` (the [unit
value](https://reasonml.github.io/docs/en/overview#unit)). However, inside this
test, we are calling `Discount.getFreeBurger` to test its side effects, so the
return value isn't needed; as such, we can explicitly discard it by using
[Stdlib.ignore](https://melange.re/v3.0.0/api/re/melange/Stdlib/#val-ignore)[^2]:

```reason
Discount.getFreeBurger(items) |> ignore;
```

## Runtime representation of variant

After compliation succeeds, we find that the "Input array isn't changed" unit
test fails. Part of the output (cleaned up for readability) looks like this:

```json
0,
{
  TAG: 1,
  _0: {
    bacon: 0,
    cheese: 0,
    lettuce: false,
    onions: 0,
    tomatoes: true
  }
},
{
  TAG: 0,
  _0: 1
},
{
  TAG: 1,
  _0: {
    bacon: 2,
    cheese: 0,
    lettuce: false,
    onions: 0,
    tomatoes: false
  }
}
```

This is how Melange maps the original OCaml values to their JavaScript runtime
values shown by Node test runner:

| OCaml source | JavaScript runtime |
|--------------|--------------------|
| `Item.Hotdog` | `0` |
| `Sandwich(Ham)` | `{TAG: 0, _0: 1}` |
| `Burger({...burger, tomatoes: true})` | `{TAG: 1, _0: {bacon: 0, cheese: 0, lettuce: false, onions: 0, tomatoes: true}}` |

A variant constructor without arguments, like `Hotdog`, gets turned into an
integer. If the constructor has an argument, like `Sandwich(Ham)`, then it's
turned into a record where the `TAG` field is an integer and the `_0` field
contains the argument. Records, like the one encased in the `Burger`
constructor, are turned into JS objects.

::: warning

Variant constructors in the runtime don't always have the `TAG` field. That
field only appears when there's more than one variant constructor with an
argument. See [Data types and runtime
representation](https://melange.re/v3.0.0/communicate-with-javascript.html#data-types-and-runtime-representation)
for more details.

:::

## Arrays are mutable

The "Input array isn't changed" unit test fails because arrays in OCaml are
mutable (just as in JavaScript) and the `Discount.getFreeBurger` function
mutates its array argument. The easiest way to fix this is to swap the order of
`Js.Array.sortInPlaceWith` and `Js.Array.filter` invocations:

<<< Discount.re#swap-function-order

Although sorting still happens in-place, the array being sorted is a new one
created by `Js.Array.filter` (the array containing only burgers), not the
original input array.

## Runtime representation of `option`

We need to add one more test to check that `Discount.getFreeBurger` works when
there are more than two burgers:

<<< DiscountTests.re#three-burgers

This test fails, with the key part of the output being:

```diff
+      Expected values to be strictly equal:
+      + actual - expected
+
+      + undefined
+      - 15.15
```

Recall that `Discount.getFreeBurger` has the return type `option(float)`.
This is how Melange maps `option(float)` values to the JavaScript runtime[^3]:

| OCaml source | JavaScript runtime |
|--------------|--------------------|
| `None` | `undefined` |
| `Some(15.15)` | `15.15` |

So Node test runner is basically telling you that `None` was returned, but
`Some(15.15)` was expected.

## Pattern matching on arrays

The test is failing because the current "success" pattern match only accounts
for a two-element array:

```reason{2}
switch (burgers) {
| [|Burger(_), Burger(cheaperBurger)|] =>
  Some(Item.Burger.toPrice(cheaperBurger))
| _ => None
```

OCaml [only allows you to pattern match on arrays of fixed
length](https://v2.ocaml.org/manual/patterns.html#sss:pat-array), so to fix
this, we must instead match on a tuple of the first and second elements of the
array:

<<< Discount.re#match-on-tuple

## Array access is unsafe

The first and second tests now fail due to `index out of bounds` errors (since
they work on arrays of length 0 and 1, respectively). Array access in OCaml is
unsafe by default, so the simplest fix is to catch the exception in the switch
expression:

<<< Discount.re#catch-exception{2}

[Stdlib.Invalid_argument](https://melange.re/v3.0.0/api/re/melange/Stdlib/#exception-Invalid_argument)
is the exception raised when you try to access an array with an invalid index.

## `Array.get` array access function

It looks like `burger[0]` is using an array access operator, but it's actually
just a function call. That is, `burger[0]` is completely equivalent to
`Array.get(burger, 0)`.

Since the [`Stdlib` module is opened by
default](https://melange.re/v3.0.0/api/re/melange/Stdlib/), the
[Stdlib.Array.get](https://melange.re/v3.0.0/api/re/melange/Stdlib/Array/#val-get)
function is used for `Array.get`, but it's possible to override this by defining
our own `Array` module. Add a new file `src/order-confirmation/Array.re`:

<<< Discount.re#module-array

This function returns `None` if the `index` is out of bounds; otherwise it
returns `Some(Stdlib.Array.get(array, index))`, i.e. the element at `index`
encased by `Some`.

Introducing our own `Array` module triggers a new compilation error:

```
File "src/order-confirmation/Discount.re", line 18, characters 5-11:
18 |   | (Burger(_), Burger(cheaperBurger)) =>
          ^^^^^^
Error: This variant pattern is expected to have type Item.t option
       There is no constructor Burger within type option
```

The "success" branch must now include `Some` in the pattern match:

<<< Discount.re#custom-array-get{2}

Your code should now compile and all unit tests should pass. If you haven't done
so already, run `npm run promote` to promote the latest test output to become
the expected test output inside `tests.t`.

---

Nice, you've implemented the burger discount, and you also understand more about
arrays in OCaml. In the next chapter, you'll implement the same discount logic
using lists, which are a better fit for this problem.

## Overview

- Type inference is less effective inside functions that don't call other
  functions. In those cases, you can give the compiler more information:
  - Type annotate the function arguments
  - Use the full name for value
- The `Stdlib` module is opened by default
- Records are immutable
- Use record copy syntax to make copies of records that have different values
  for some fields
- OCaml doesn't allow you to ignore the return value of functions (unless the
  value is `()`), so you can use `Stdlib.ignore` to explicitly discard return
  values
- Runtime representations of common data types:
  - Variant constructor without argument -> integer
  - Variant constructor with argument -> JS object
  - Record -> JS object
  - `None` -> `undefined`
  - `Some(value)` -> `value`
- Array facts:
  - Arrays are mutable, just like in JS
  - You can pattern match on arrays of fixed length
  - Array access is unsafe by default
  - `anArray[index]` is equivalent to `Array.get(anArray, index)`
  - You can create your own `Array` module to override the behavior of
    `Stdlib.Array.get`

## Exercises

<b>1.</b> `Discount.getFreeBurger` can be improved a bit. In particular, there's
no need to match on the `Burger` constructor when non-burger items have already
been filtered out. Refactor the function so that the "success" pattern match
looks like this:

```reason
| (Some(_), Some(cheaperPrice)) => Some(cheaperPrice)
```

::: details Hint

Use [Js.Array.map](https://melange.re/v3.0.0/api/re/melange/Js/Array/#val-map)

:::


::: details Solution

<<< Discount.re#improved-get-free-burger

:::

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/burger-discounts/)
and [demo](https://react-book.melange.re/demo/src/burger-discounts/) for this chapter.

-----

[^1]: The official term for something like `Item.Burger` (module name followed
    by value name) is [access
    path](https://v2.ocaml.org/manual/names.html#sss:refer-named), but this term
    isn't widely used.

[^2]: Another valid way to discard the return value of a function is:

    ```reason
    let _ = Discount.getFreeBurger(items);
    ```

[^3]: Technically `option` is a variant, but Melange treats them as a special
    case---`option` values are never represented as JS objects in the runtime.

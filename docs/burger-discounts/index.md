# Burger Discounts

International Burger Day falls on Tuesday of next week, so Madame Jellobutter
decides to celebrate it by running the classic "buy 2 burgers get 1 free"
promotion. She clearly lays out the rules of this promotion:

> Given an order comprising multiple items, find the price of the second most
> expensive burger. Subtract this number from the pre-tax total of the order.

Add a new file `src/order-confirmation/Discount.re`:

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

- [Js.Array.sortInPlaceWith](https://melange.re/v4.0.0/api/re/melange/Js/Array/index.html#val-sortInPlaceWith)
  takes a callback function `~f` with type signature `('a, 'a) => int` (accept
  two arguments of the same type and return `int`). It's used to sort the items
  by price (highest to lowest).
- [Stdlib.compare](https://melange.re/v4.0.0/api/re/melange/Stdlib/#val-compare)
  has type signature `('a, 'a) => int`. It's a polymorphic function capable of
  comparing many types, including `bool`, `int`, `string`, etc. Note that you
  can always just write `compare` instead of `Stdlib.compare`, because the
  [`Stdlib` module is always opened by
  default](https://melange.re/v4.0.0/api/re/melange/Stdlib/).
- [Js.Array.filter](https://melange.re/v4.0.0/api/re/melange/Js/Array/#val-filter)
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

## Use full name

Explicit type annotation always works, but sometimes it's enough to just give
the compiler a hint. For example, we can use full names[^1] for the constructors
inside the switch expression:

<<< Discount.re#full-name-constructors{3-5}

Because `Item.Burger` is a constructor of the `Item.t` variant type, `item` must
have type `Item.t`. For the sake of convenience, you don't need to use full
names in the second branch of the switch expression---OCaml is smart enough to
infer which module the `Sandwich` and `Hotdog` constructors belong to.

<<< Discount.re#full-name-constructor{4-5}

By using the full name of the `Burger` constructor, we can now easily refactor
the callback function to use the `fun` syntax:

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

## Record spread syntax

Add a new test to `DiscountTests`:

<<< DiscountTests.re#different-price-test

Again, we're reusing the `burger` record, but this time, we use [record spread
syntax](https://reasonml.github.io/docs/en/record#updating-records-spreading) to
make copies of the `burger` record that have slightly different field values.
For example,

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

When you call a function in OCaml, you must use its return value, unless the
return value is `()` (the [unit
value](https://reasonml.github.io/docs/en/overview#unit)). However, inside this
test, we are calling `Discount.getFreeBurger` to test its side effects, so the
return value isn't needed; as such, we can explicitly discard it by using
[Stdlib.ignore](https://melange.re/v4.0.0/api/re/melange/Stdlib/#val-ignore)[^2]:

```reason
Discount.getFreeBurger(items) |> ignore;
```

## Runtime representation of variant

After compilation succeeds, we find that the "Input array isn't changed" unit
test fails. Part of the output (cleaned up for readability) looks like this:

```json
[
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
]
```

This is how Melange maps the original OCaml values to their JavaScript runtime
values:

| OCaml source | JavaScript runtime |
|--------------|--------------------|
| `Item.Hotdog` | `0` |
| `Sandwich(Ham)` | `{TAG: 0, _0: 1}` |
| `Burger({...burger, tomatoes: true})` | `{TAG: 1, _0: {bacon: 0, cheese: 0, lettuce: false, onions: 0, tomatoes: true}}` |

A variant constructor without arguments, like `Hotdog`, gets turned into an
integer. If the constructor has an argument, like `Sandwich(Ham)`, then it's
turned into a record where the `TAG` field is an integer and the `_0` field
contains the argument. Records, like the one encased in the `Burger`
constructor, are turned into JS objects. OCaml arrays, like the one that
contains all the items, are turned into JS arrays.

::: warning

Variant constructors in the runtime don't always have the `TAG` field. That
field only appears when there's more than one variant constructor with an
argument. See [Data types and runtime
representation](https://melange.re/v4.0.0/communicate-with-javascript.html#data-types-and-runtime-representation)
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
length](https://ocaml.org/manual/5.2/patterns.html#sss:pat-array), so to fix
this, we must instead match on a tuple of the first and second elements of the
array:

<<< Discount.re#match-on-tuple

## Array access is unsafe

The first and second tests now fail due to `index out of bounds` errors (since
they work on arrays of length 0 and 1, respectively). Array access in OCaml is
unsafe by default, so the simplest fix is to check the length of the array
before using the switch expression:

<<< Discount.re#check-array-length{1-2}

An alternative approach is to catch the exception that gets raised using an
`exception` branch inside the switch expression:

<<< Discount.re#catch-exception{2}

## `Array.get` array access function

In OCaml, the array access operator `[]` is just a function call. That is,
`burger[0]` is completely equivalent to `Array.get(burger, 0)`.

Since the [`Stdlib` module is opened by
default](https://melange.re/v4.0.0/api/re/melange/Stdlib/), the
[Stdlib.Array.get](https://melange.re/v4.0.0/api/re/melange/Stdlib/Array/#val-get)
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

Note that we no longer need to check the array length or catch an exception. Our
new `Array.get` function is a safe function that returns `None` instead of
raising an exception.

Your code should now compile and all unit tests should pass. If you haven't done
so already, run `npm run promote` to promote the latest test output to become
the expected test output inside `tests.t`.

## Variant constructors are not functions

What happens if you to try to rewrite `Some(String.length("foobar"))`
to `"foobar" |> String.length |> Some`?


You'll get a compilation error:

```
Error This expression should not be a constructor, the expected type is int -> 'a
```

Variant constructors like `Some` are not functions, so they can't be used with
the pipe last (`|>`) operator. If you have a long chain of function invocations
but you need to return a variant at the end, consider using an extra variable,
e.g.

<<< Discount.re#return-variant-at-end

See [full example on Melange
Playground](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGNpcGhlckdyZWV0aW5nID0gbmFtZSA9PiB7CiAgc3dpdGNoIChTdHJpbmcudHJpbShuYW1lKSkgewogIHwgIiIgPT4gTm9uZQogIHwgbmFtZSA9PgogICAgbGV0IHJlc3VsdCA9CiAgICAgIG5hbWUKICAgICAgfD4gU3RyaW5nLnNwbGl0X29uX2NoYXIoJyAnKQogICAgICB8PiBMaXN0Lm1hcChTdHJpbmcubWFwKGMgPT4gYyB8PiBDaGFyLmNvZGUgfD4gKCspKDEpIHw%2BIENoYXIuY2hyKSkKICAgICAgfD4gU3RyaW5nLmNvbmNhdCgiICIpCiAgICAgIHw%2BIFN0cmluZy5jYXQoIkhlbGxvLCAiKTsKCiAgICBTb21lKHJlc3VsdCk7CiAgfTsKfTsKCkpzLmxvZyhjaXBoZXJHcmVldGluZygiIikpOwpKcy5sb2coY2lwaGVyR3JlZXRpbmcoIlhhdmllciBMZXJveSIpKTsK&live=off).

When the variant type you're using is `option`, though, you can still chain
functions by using the `Option.some` helper function:

<<< Discount.re#use-ok-some

---

Nice, you've implemented the burger discount, and you also understand more about
arrays in OCaml. In the next chapter, you'll implement the same discount logic
using lists, which are a better fit for this problem.

## Overview

- Type inference is less effective inside functions that don't call other
  functions. In those cases, you can give the compiler more information:
  - Type annotate the function arguments
  - Use the full name for a value used inside the function
- The `Stdlib` module is opened by default
- Records are immutable
- Use record spread syntax to copy a record while changing some fields on the
  copied record, e.g. `{...burger, lettuce: true, onions: 3}`
- OCaml doesn't allow you to ignore the return value of functions (unless the
  value is `()`), so you can use `Stdlib.ignore` to explicitly discard return
  values
- Runtime representations of common data types:
  - Variant constructor without argument → integer
  - Variant constructor with argument → JavaScript object
  - Record → JavaScript object
  - Array → JavaScript array
  - `None` → `undefined`
  - `Some(value)` → `value`
- Variant constructors are not functions
- Array facts:
  - Arrays are mutable, just like in JavaScript
  - You can pattern match on arrays of fixed length
  - Array access is unsafe by default
  - What looks like operator usage in `array[index]` is actually just a call to
    `Array.get(array, index)`
  - You can create your own `Array` module to override the behavior of
    `Array.get`

## Exercises

<b>1.</b> `Discount.getFreeBurger` can be improved a bit. In particular, there's
no need to match on the `Burger` constructor when non-burger items have already
been filtered out. Refactor the function so that the "success" pattern match
looks like this:

```reason
| Some(cheaperPrice) => Some(cheaperPrice)
```

Also refactor the "failure" pattern match so there's no wildcard.

::: details Hint

Use [Js.Array.map](https://melange.re/v4.0.0/api/re/melange/Js/Array/#val-map)

:::

::: details Solution

<<< Discount.re#improved-get-free-burger

Since the chain of function invocations now results in an array of `float`s, we
rename the variable from `burgers` to `prices`. We only need to match on the
second element of `prices` because if it exists, then a first element must also
exist (but we don't need to know its value).

:::

<b>2.</b> `Discount.getFreeBurger` can still be improved. Refactor it to remove
the switch expression entirely.

::: details Solution

<<< Discount.re#final-get-free-burger

:::

<b>3.</b> Add a new function `Discount.getHalfOff` that gives you a discount of
half off the entire meal if there’s at least one burger that has one of every
topping. Add these new tests to `DiscountTests` and make sure they pass:

<<< DiscountTests.re#half-off-tests

::: details Hint

Use [Js.Array.some](https://melange.re/v4.0.0/api/re/melange/Js/Array/#val-some)

:::

::: details Solution

<<< Discount.re#get-half-off-one

:::

<b>4.</b> Update `Discount.getHalfOff` so that it returns a discount of one half
off the entire meal if there’s at least one burger that has **at least** one of
every topping. While you're at it, update the relevant tests in `DiscountTests`:

<<< DiscountTests.re#new-half-off-tests

Note the use of submodule `HalfOff` to group these two tests together.

::: details Hint

Use [when](https://reasonml.github.io/docs/en/pattern-matching#when) guard

:::

::: details Solution

<<< Discount.re#get-half-off

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/burger-discounts/)
and [demo](https://react-book.melange.re/demo/src/burger-discounts/) for this chapter.

-----

[^1]: The official term for something like `Item.Burger` (module name followed
    by value name) is [access
    path](https://ocaml.org/manual/5.2/names.html#sss:refer-named), but this term
    isn't widely used.

[^2]: Another valid way to discard the return value of a function is:

    ```reason
    let _: option(float) = Discount.getFreeBurger(items);
    ```

    This works, but `ignore` is more explicit and therefore the recommended
    approach.

[^3]: Technically [`option` is a
    variant](https://melange.re/v4.0.0/api/re/melange/Stdlib/Option/#type-t),
    but Melange treats them as a special case---`option` values are never
    represented as JS objects in the runtime.

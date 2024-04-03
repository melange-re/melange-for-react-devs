# Burger Discounts

Madame Jellobutter decides to celebrate the upcoming International Burger Day by
running the classic "buy 2 burgers get 1 free" promotion. Add a new file
`src/order-confirmation/Discount.re`:

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

About the functions used in `Discount.getFreeBurger`:

- [Js.Array.sortInPlaceWith](https://melange.re/v3.0.0/api/re/melange/Js/Array/index.html#val-sortInPlaceWith)
  takes a callback function `~f` that accepts two arguments and returns `int`.
  It's used to used to sort the items by price (highest to lowest).
- [Stdlib.compare](https://melange.re/v3.0.0/api/re/melange/Stdlib/#val-compare)
  takes two arguments and returns `int`. It's a polymorphic function capable of
  comparing many types, including `bool`, `int`, `string`, etc. Note that you
  can always just write `compare` instead of `Stdlib.compare`, because the
  `Stdlib` module is always opened by default.
- [Js.Array.filter](https://melange.re/v3.0.0/api/re/melange/Js/Array/#val-filter)
  takes a callback function `~f` that accepts one argument and returns a `bool`.
  It's used to make sure all items in the resulting array are burgers.

There are a number of issues with this code, including the fact that it doesn't
compile, but we'll address each issue in the following sections.

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
      compare(Item.toPrice(item1), Item.toPrice(item2))
    )
```

The reason is that `Item.toPrice` is invoked inside this callback, and its type
signature is already known to be `Item.t => float`. So type inference can figure
out that `item1` and `item2` must both be of type `Item.t`, because
`Item.toPrice` can only accept a argument of type `Item.t`.

## Type annotate the callback argument

There aren't any function invocations inside the callback to `Js.Array.filter`,
so we can help the compiler out by type annotating the `item` argument:

<<< Discount.re#type-annotate-argument{1}

## Full name of constructor

There's another, less direct way to indicate the type of `item`, which is to use
the full name for the constructors used in the switch expression:

<<< Discount.re#full-name-constructors{3-5}

Because `Item.Burger` is a constructor of the `Item.t` variant type, `item` must
have type `Item.t`. By the way, you can just put `Item.` in front of the first
constructor in the switch expression, OCaml is smart to know what module the
following constructors come from:

<<< Discount.re#full-name-constructor{3}

By using the full name of the `Burger` constructor, we can also switch the
callback function to use the `fun` syntax:

<<< Discount.re#full-name-fun

## Add new tests

Let's add some tests in a new file called
`src/order-confirmation/DiscountTests.re`:

<<< DiscountTests.re#first-three

To run these tests, add a new cram test to `src/order-confirmation/tests.t`:

```cram
Discount tests
  $ node ./output/src/order-confirmation/DiscountTests.mjs | sed '/duration_ms/d'
```

Run `npm run test:watch` to see that the unit tests pass, then run `npm promote`
to make the cram test pass.

## Records are immutable

It's tiresome and also redundant to fully write out every burger record in the
tests. Define a `Item.Burger.t` record at the very top of `DiscountTests`:

<<< DiscountTests.re#burger-record

Then refactor the second and third tests to use this record:

<<< DiscountTests.re#refactor-use-burger-record{4,14,16}

It's perfectly fine to reuse records in this way because records are immutable.
You can pass a record to any function and not worry that its fields might change
in any way.

## Record copy syntax

Add a new test to `DiscountTests`:

<<< DiscountTests.re#fourth-test

Again, we're reusing the `burger` record, but this time, we use [record copy
syntax](https://reasonml.github.io/docs/en/record#updating-records-spreading) to
make copies of `burger` record that have slightly different field values. For
example,

```reason
{...burger, tomatoes: true}
```

means to make a copy of `burger` but with `tomatoes` set to `true`. It's just a
shorter way to write this:

```reason
{
  lettuce: burger.lettuce,
  onions: burger.onions,
  cheese: burger.cheese,
  bacon: burger.bacon,
  tomatoes: true,
}
```

---

Nice, you've implemented the burger discount, and you also understand more about
how to use arrays in OCaml. In the next chapter, you'll implement the same
discount logic using lists, which are better suited to this problem than arrays.

## Overview



## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/burger-discounts/)
and [demo](https://react-book.melange.re/demo/src/burger-discounts/) for this chapter.

-----

footnotes

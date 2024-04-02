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
        compare(Item.toPrice(item1), Item.toPrice(item2))
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

[Js.Array.sortInPlaceWith](https://melange.re/v3.0.0/api/re/melange/Js/Array/index.html#val-sortInPlaceWith)
is used to sort the items by price, and it takes a callback function that
accepts two arguments and returns an integer.

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

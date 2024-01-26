open Fest;

// update melange-fest dependency and then you can remove this:
[@mel.send.pipe: assertion]
external deepStrictEqual: ('a, 'a) => unit = "deepStrictEqual";

let makeBurger =
    (~lettuce=false, ~onions=0, ~cheese=0, ~tomatoes=false, ~bacon=0, ()) =>
  Item.Burger({lettuce, onions, cheese, tomatoes, bacon});

test("1 burger, no discount", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|Hotdog, Sandwich(Ham), makeBurger()|]),
       None,
     )
);

test("2 burgers of same price, discount", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|
         Hotdog,
         makeBurger(),
         Sandwich(Ham),
         makeBurger(),
       |]),
       Some(15.),
     )
);

test("2 burgers of different price, discount of cheaper one", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|
         Hotdog,
         makeBurger(~tomatoes=true, ()),
         Sandwich(Ham),
         makeBurger(~bacon=2, ()),
       |]),
       Some(15.05),
     )
);

test("input array isn't changed", () => {
  let items = [|
    Item.Hotdog,
    makeBurger(~tomatoes=true, ()),
    Sandwich(Ham),
    makeBurger(~bacon=2, ()),
  |];
  Discount.getBurgerDiscount(items) |> ignore;
  expect
  |> deepStrictEqual(
       items,
       [|
         Item.Hotdog,
         makeBurger(~tomatoes=true, ()),
         Sandwich(Ham),
         makeBurger(~bacon=2, ()),
       |],
     );
});

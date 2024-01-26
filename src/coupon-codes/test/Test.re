open Fest;

// update melange-fest dependency and then you can remove this:
[@mel.send.pipe: assertion]
external deepStrictEqual: ('a, 'a) => unit = "deepStrictEqual";

let makeBurger =
    (~lettuce=false, ~onions=0, ~cheese=0, ~tomatoes=false, ~bacon=0, ()) =>
  Lib.Item.Burger({lettuce, onions, cheese, tomatoes, bacon});

test("hello", () =>
  expect
  |> equal(
       None,
       Lib.Discount.getBurgerDiscount([|
         Hotdog,
         Sandwich(Ham),
         makeBurger(),
       |]),
     )
);

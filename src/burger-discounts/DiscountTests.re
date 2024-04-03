open Fest;

let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};

test("0 burgers, no discount", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|
         Hotdog,
         Sandwich(Ham),
         Sandwich(Turducken),
       |]),
       None,
     )
);

test("1 burger, no discount", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|Hotdog, Sandwich(Ham), Burger(burger)|]),
       None,
     )
);

test("2 burgers of same price, discount", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|
         Hotdog,
         Burger(burger),
         Sandwich(Ham),
         Burger(burger),
       |]),
       Some(15.),
     )
);

test("2 burgers of different price, discount of cheaper one", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|
         Hotdog,
         Burger({...burger, tomatoes: true}), // 15.05
         Sandwich(Ham),
         Burger({...burger, bacon: 2}) // 16.00
       |]),
       Some(15.05),
     )
);

test("Input array isn't changed", () => {
  let items = [|
    Item.Hotdog,
    Burger({...burger, tomatoes: true}),
    Sandwich(Ham),
    Burger({...burger, bacon: 2}),
  |];

  Discount.getFreeBurger(items) |> ignore;

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

test("3 burgers of different price, return Some(15.15)", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|
         Burger(burger), // 15
         Hotdog,
         Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
         Sandwich(Ham),
         Burger({...burger, bacon: 2}) // 16.00
       |]),
       Some(15.15),
     )
);

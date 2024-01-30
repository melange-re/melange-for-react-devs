open Fest;

// let makeBurger =
//     (~lettuce=false, ~onions=0, ~cheese=0, ~tomatoes=false, ~bacon=0, ()) =>
//   Item.Burger({lettuce, onions, cheese, tomatoes, bacon});

let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};

test("1 burger, no discount", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|
         Hotdog,
         Sandwich(Ham),
         Burger(burger),
       |]),
       None,
     )
);

test("2 burgers of same price, discount", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|
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
       Discount.getBurgerDiscount([|
         Hotdog,
         Burger({...burger, tomatoes: true}),
         Sandwich(Ham),
         Burger({...burger, bacon: 2}),
       |]),
       Some(15.05),
     )
);

test("input array isn't changed", () => {
  let items = [|
    Item.Hotdog,
    Burger({...burger, tomatoes: true}),
    Sandwich(Ham),
    Burger({...burger, bacon: 2}),
  |];
  Discount.getBurgerDiscount(items) |> ignore;
  expect
  |> deepStrictEqual(
       items,
       [|
         Item.Hotdog,
         Burger({...burger, bacon: 2}),
         Sandwich(Ham),
         Burger({...burger, bacon: 2}),
       |],
     );
});

test("3 burgers of different price, discount of second one", () =>
  expect
  |> equal(
       Discount.getBurgerDiscount([|
         Burger(burger),
         Hotdog,
         Burger({...burger, tomatoes: true, cheese: 1}),
         Sandwich(Ham),
         Burger({...burger, bacon: 2}),
       |]),
       Some(15.15),
     )
);

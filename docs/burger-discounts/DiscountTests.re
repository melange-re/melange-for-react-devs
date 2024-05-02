// #region first-three
open Fest;

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
       Discount.getFreeBurger([|
         Hotdog,
         Sandwich(Ham),
         Burger({
           lettuce: false,
           onions: 0,
           cheese: 0,
           tomatoes: false,
           bacon: 0,
         }),
       |]),
       None,
     )
);

test("2 burgers of same price, discount", () =>
  expect
  |> equal(
       Discount.getFreeBurger([|
         Hotdog,
         Burger({
           lettuce: false,
           onions: 0,
           cheese: 0,
           tomatoes: false,
           bacon: 0,
         }),
         Sandwich(Ham),
         Burger({
           lettuce: false,
           onions: 0,
           cheese: 0,
           tomatoes: false,
           bacon: 0,
         }),
       |]),
       Some(15.),
     )
);
// #endregion first-three

// #region burger-record
let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};
// #endregion burger-record

// #region refactor-use-burger-record
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
// #endregion refactor-use-burger-record

// #region different-price-test
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
// #endregion different-price-test

// #region three-burgers
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
// #endregion three-burgers

// #region half-off-tests
test("No burger has 1 of every topping, return None", () =>
  expect
  |> equal(
       Discount.getHalfOff([|
         Hotdog,
         Sandwich(Portabello),
         Burger({
           lettuce: true,
           tomatoes: true,
           cheese: 1,
           onions: 1,
           bacon: 0,
         }),
       |]),
       None,
     )
);

test("One burger has 1 of every topping, return Some(15.425)", () =>
  expect
  |> equal(
       Discount.getHalfOff([|
         Hotdog,
         Sandwich(Portabello),
         Burger({
           lettuce: true,
           tomatoes: true,
           cheese: 1,
           onions: 1,
           bacon: 1,
         }),
       |]),
       Some(15.425),
     )
);
// #endregion half-off-tests

// #region new-half-off-tests
module HalfOff = {
  test("No burger has 1+ of every topping, return None", () =>
    expect
    |> equal(
         Discount.getHalfOff([|
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 0,
           }),
         |]),
         None,
       )
  );

  test("One burger has 1+ of every topping, return Some(15.675)", () =>
    expect
    |> equal(
         Discount.getHalfOff([|
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 1,
           }),
         |]),
         Some(15.675),
       )
  );
};
// #endregion new-half-off-tests

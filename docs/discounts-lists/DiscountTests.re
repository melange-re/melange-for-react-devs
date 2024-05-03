open Fest;

// #region first-test
test("0 burgers, no discount", () =>
  expect
  |> equal(
       Discount.getFreeBurger([Hotdog, Sandwich(Ham), Sandwich(Turducken)]),
       None,
     )
);
// #endregion first-test

let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};

// #region get-free-burgers-test
test("7 burgers, return Some(46.75)", () =>
  expect
  |> equal(
       Discount.getFreeBurgers([
         Burger(burger), // 15
         Hotdog,
         Burger({...burger, cheese: 5}), // 15.50
         Sandwich(Unicorn),
         Burger({...burger, bacon: 4}), // 17.00
         Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
         Sandwich(Ham),
         Burger({...burger, bacon: 2}), // 16.00
         Burger({...burger, onions: 6}), // 16.20
         Sandwich(Portabello),
         Burger({...burger, tomatoes: true}) // 15.05
       ]),
       Some(46.75),
     )
);
// #endregion get-free-burgers-test

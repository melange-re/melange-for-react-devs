open Fest;

module FreeBurger = {
  let burger: Item.Burger.t = {
    lettuce: false,
    onions: 0,
    cheese: 0,
    tomatoes: false,
    bacon: 0,
  };

  test("0 burgers, no discount", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([
           Hotdog,
           Sandwich(Ham),
           Sandwich(Turducken),
         ]),
         Error("You must buy at least 2 burgers to use this promo code"),
       )
  );

  test("1 burger, no discount", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([Hotdog, Sandwich(Ham), Burger(burger)]),
         Error("You must buy at least 2 burgers to use this promo code"),
       )
  );

  test("2 burgers of same price, discount", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([
           Hotdog,
           Burger(burger),
           Sandwich(Ham),
           Burger(burger),
         ]),
         Ok(15.),
       )
  );

  test("2 burgers of different price, discount of cheaper one", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([
           Hotdog,
           Burger({...burger, tomatoes: true}), // 15.05
           Sandwich(Ham),
           Burger({...burger, bacon: 2}) // 16.00
         ]),
         Ok(15.05),
       )
  );

  test("3 burgers of different price, return Ok(15.15)", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([
           Burger(burger), // 15
           Hotdog,
           Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
           Sandwich(Ham),
           Burger({...burger, bacon: 2}) // 16.00
         ]),
         Ok(15.15),
       )
  );

  test("7 burgers, return Ok(46.75)", () =>
    expect
    |> deepEqual(
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
         Ok(46.75),
       )
  );
};

module HalfOff = {
  test("No burger has 1+ of every topping, return Error", () =>
    expect
    |> deepEqual(
         Discount.getHalfOff([
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 0,
           }),
         ]),
         Error(
           "You must buy a burger with at least 1 of every topping to use this promo code",
         ),
       )
  );

  test("One burger has 1+ of every topping, return Ok(15.675)", () =>
    expect
    |> deepEqual(
         Discount.getHalfOff([
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 2,
           }),
         ]),
         Ok(15.675),
       )
  );
};

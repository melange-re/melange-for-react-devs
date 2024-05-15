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
         Error(`TwoBurgers),
       )
  );

  test("1 burger, no discount", () =>
    expect
    |> deepEqual(
         Discount.getFreeBurgers([Hotdog, Sandwich(Ham), Burger(burger)]),
         Error(`OneBurger),
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

module SandwichHalfOff = {
  test("No sandwiches, return Error", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff([
           Hotdog,
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 0,
           }),
           Hotdog,
         ]),
         Error(
           `MissingSandwichTypes([
             "portabello",
             "ham",
             "unicorn",
             "turducken",
           ]),
         ),
       )
  );

  test("All sandwiches, return Ok", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff([
           Sandwich(Turducken),
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 2,
           }),
           Sandwich(Unicorn),
           Sandwich(Ham),
         ]),
         Ok(70.675),
       )
  );
};

module GetDiscount = {
  test("FREE promo code works in May but not other months", () => {
    List.init(12, i => i)
    |> List.iter(month => {
         let date =
           Js.Date.makeWithYMD(
             ~year=2024.,
             ~month=float_of_int(month),
             ~date=10.,
           );

         expect
         |> deepEqual(
              Discount.getDiscountFunction(~code="free", ~date),
              month == 4 ? Ok(Discount.getFreeBurgers) : Error(ExpiredCode),
            );
       })
  });

  test(
    "HALF promo code returns getHalfOff on May 28 but not other days of May",
    () => {
    for (dayOfMonth in 1 to 31) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=4.0,
          ~date=float_of_int(dayOfMonth),
        );

      expect
      |> deepEqual(
           Discount.getDiscountFunction(~code="half", ~date),
           dayOfMonth == 28 ? Ok(Discount.getHalfOff) : Error(ExpiredCode),
         );
    }
  });

  test(
    "HALF promo code returns getSandwichHalfOff on Nov 3 but not other days of Nov",
    () => {
    for (dayOfMonth in 1 to 30) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=10.0,
          ~date=float_of_int(dayOfMonth),
        );

      expect
      |> deepEqual(
           Discount.getDiscountFunction(~code="half", ~date),
           dayOfMonth == 3
             ? Ok(Discount.getSandwichHalfOff) : Error(ExpiredCode),
         );
    }
  });
};

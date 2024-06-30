open Fest;

// 2024 June 3 is a Monday
let june3 = Js.Date.fromString("2024-06-03T00:00");

module FreeBurger = {
  let getFreeBurgers = Discount.getFreeBurgers;

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
         getFreeBurgers([Hotdog, Sandwich(Ham), Sandwich(Turducken)]),
         Error(`NeedTwoBurgers),
       )
  );

  test("1 burger, no discount", () =>
    expect
    |> deepEqual(
         getFreeBurgers([Hotdog, Sandwich(Ham), Burger(burger)]),
         Error(`NeedOneBurger),
       )
  );

  test("2 burgers of same price, discount", () =>
    expect
    |> deepEqual(
         getFreeBurgers([
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
         getFreeBurgers([
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
         getFreeBurgers([
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
         getFreeBurgers([
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
  test("No burger has 1+ of every topping, return Error(`NeedMegaBurger)", () =>
    expect
    |> deepEqual(
         Discount.getHalfOff(
           ~date=june3,
           [
             Hotdog,
             Sandwich(Portabello),
             Burger({
               lettuce: true,
               tomatoes: true,
               cheese: 1,
               onions: 1,
               bacon: 0,
             }),
           ],
         ),
         Error(`NeedMegaBurger),
       )
  );

  test("One burger has 1+ of every topping, return Ok(15.675)", () =>
    expect
    |> deepEqual(
         Discount.getHalfOff(
           ~date=june3,
           [
             Hotdog,
             Sandwich(Portabello),
             Burger({
               lettuce: true,
               tomatoes: true,
               cheese: 1,
               onions: 1,
               bacon: 2,
             }),
           ],
         ),
         Ok(15.675),
       )
  );
};

module SandwichHalfOff = {
  test("Not all sandwiches, return Error", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff(
           ~date=june3,
           [
             Sandwich(Unicorn),
             Hotdog,
             Sandwich(Portabello),
             Sandwich(Ham),
           ],
         ),
         Error(`MissingSandwichTypes(["turducken"])),
       )
  );

  test("All sandwiches, return Ok", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff(
           ~date=june3,
           [
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
           ],
         ),
         Ok(70.675),
       )
  );
};

module GetDiscount = {
  let getDiscountFunction = (code, date) =>
    Discount.getDiscountPair(code, date) |> Result.map(fst);

  test("Invalid promo code return Error", () => {
    let date = Js.Date.make();
    ["", "FREEDOM", "UNICORN", "POO"]
    |> List.iter(code =>
         expect
         |> deepEqual(getDiscountFunction(code, date), Error(InvalidCode))
       );
  });

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
              getDiscountFunction("FREE", date),
              month == 4 ? Ok(`FreeBurgers) : Error(ExpiredCode),
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
           getDiscountFunction("HALF", date),
           dayOfMonth == 28 ? Ok(`HalfOff) : Error(ExpiredCode),
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
           getDiscountFunction("HALF", date),
           dayOfMonth == 3 ? Ok(`SandwichHalfOff) : Error(ExpiredCode),
         );
    }
  });
};

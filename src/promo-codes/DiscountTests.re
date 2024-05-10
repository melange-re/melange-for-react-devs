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
    |> equal(
         Discount.getFreeBurgers([
           Hotdog,
           Sandwich(Ham),
           Sandwich(Turducken),
         ]),
         None,
       )
  );

  test("1 burger, no discount", () =>
    expect
    |> equal(
         Discount.getFreeBurgers([Hotdog, Sandwich(Ham), Burger(burger)]),
         None,
       )
  );

  test("2 burgers of same price, discount", () =>
    expect
    |> equal(
         Discount.getFreeBurgers([
           Hotdog,
           Burger(burger),
           Sandwich(Ham),
           Burger(burger),
         ]),
         Some(15.),
       )
  );

  test("2 burgers of different price, discount of cheaper one", () =>
    expect
    |> equal(
         Discount.getFreeBurgers([
           Hotdog,
           Burger({...burger, tomatoes: true}), // 15.05
           Sandwich(Ham),
           Burger({...burger, bacon: 2}) // 16.00
         ]),
         Some(15.05),
       )
  );

  test("3 burgers of different price, return Some(15.15)", () =>
    expect
    |> equal(
         Discount.getFreeBurgers([
           Burger(burger), // 15
           Hotdog,
           Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
           Sandwich(Ham),
           Burger({...burger, bacon: 2}) // 16.00
         ]),
         Some(15.15),
       )
  );

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
};

module HalfOff = {
  test("No burger has 1+ of every topping, return None", () =>
    expect
    |> equal(
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
         None,
       )
  );

  test("One burger has 1+ of every topping, return Some(15.675)", () =>
    expect
    |> equal(
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
         Some(15.675),
       )
  );
};

module GetDiscount = {
  test("Invalid promo code return Error", () => {
    let date = Js.Date.make();
    ["", "FREEDOM", "UNICORN", "POO"]
    |> List.iter(code =>
         expect
         |> deepEqual(
              Discount.getDiscountFunction(code, date),
              Error("Invalid code"),
            )
       );
  });

  test("FREE promo code works in May but not other months", () => {
    for (month in 0 to 11) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=float_of_int(month),
          ~date=10.,
        );

      expect
      |> deepEqual(
           Discount.getDiscountFunction("FREE", date),
           month == 4 ? Ok(Discount.getFreeBurgers) : Error("Expired code"),
         );
    }
  });
};

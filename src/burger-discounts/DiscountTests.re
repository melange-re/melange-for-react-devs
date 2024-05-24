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
};

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

  test("One burger has 1+ of every topping, return Some", () => {
    let items = [|
      Item.Hotdog,
      Sandwich(Portabello),
      Burger({lettuce: true, tomatoes: true, cheese: 1, onions: 1, bacon: 2}),
    |];
    expect
    |> equal(
         Discount.getHalfOff(items),
         {
           // Don't use hardcoded value since Item.toPrice is non-deterministic
           let sum =
             items
             |> Js.Array.map(~f=Item.toPrice)
             |> Js.Array.reduce(~init=0.0, ~f=(+.));
           Some(sum /. 2.0);
         },
       );
  });
};

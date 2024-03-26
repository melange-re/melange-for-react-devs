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

  test("3 burgers of different price, discount of second one", () =>
    expect
    |> equal(
         Discount.getFreeBurger([|
           Burger(burger),
           Hotdog,
           Burger({...burger, tomatoes: true, cheese: 1}),
           Sandwich(Ham),
           Burger({...burger, bacon: 2}),
         |]),
         Some(15.15),
       )
  );
};

module HalfOff = {
  test("No burger that has at least one of every topping, return None", () =>
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

  test(
    "One burger that has at least one of every topping, return Some(15.675)",
    () =>
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
             bacon: 2,
           }),
         |]),
         Some(15.675),
       )
  );
};

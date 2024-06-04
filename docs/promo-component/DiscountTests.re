open Fest;

// #region get-free-burgers
let june3 = Js.Date.fromString("2024-06-03T00:00");

module FreeBurger = {
  let getFreeBurgers = Discount.getFreeBurgers(~date=june3);

  test("0 burgers, no discount", () =>
    expect
    |> deepEqual(
         getFreeBurgers([Hotdog, Sandwich(Ham), Sandwich(Turducken)]),
         Error(`NeedTwoBurgers),
       )
  );
  // ...
};
// #endregion get-free-burgers

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

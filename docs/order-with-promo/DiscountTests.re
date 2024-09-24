open Fest;

let june3 = Js.Date.fromString("2024-06-03T00:00");

module SandwichHalfOff = {
  // #region not-all-sandwiches
  test("Not all sandwiches, return Error", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff(
           ~date=june3,
           [Sandwich(Portabello), Hotdog, Sandwich(Ham)],
         ),
         Error(`MissingSandwichTypes(["unicorn", "turducken"])),
       )
  );
  // #endregion not-all-sandwiches
};

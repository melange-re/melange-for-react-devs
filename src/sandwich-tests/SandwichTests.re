open Fest;

test("Item.Sandwich.toEmoji", () => {
  let sandwiches: array(Item.Sandwich.t) = [|
    Portabello,
    Ham,
    Unicorn,
    Turducken,
  |];

  expect
  |> deepStrictEqual(
       sandwiches |> Js.Array.map(~f=Item.Sandwich.toEmoji),
       [|
         {js|🥪(🍄)|js},
         {js|🥪(🐷)|js},
         {js|🥪(🦄)|js},
         {js|🥪(🦃🦆🐓)|js},
       |],
     );
});

test("Test.Sandwich.toPrice returns lower price for Turducken on Tuesdays", () => {
  // 1 Jan 2024 is a Monday
  let dates =
    [|1., 2., 3., 4., 5., 6., 7.|]
    |> Js.Array.map(~f=date =>
         Js.Date.makeWithYMD(~year=2024., ~month=0., ~date)
       );

  expect
  |> deepStrictEqual(
       dates |> Js.Array.map(~f=Item.Sandwich.toPrice(Turducken)),
       [|20., 10., 20., 20., 20., 20., 20.|],
     );
});

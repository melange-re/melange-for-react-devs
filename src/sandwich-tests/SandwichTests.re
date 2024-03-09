open Fest;

test("Item.Sandwich.toEmoji", () => {
  expect
  |> deepEqual(
       [|Portabello, Ham, Unicorn, Turducken|]
       |> Js.Array.map(~f=Item.Sandwich.toEmoji),
       [|
         {js|🥪(🍄)|js},
         {js|🥪(🐷)|js},
         {js|🥪(🦄)|js},
         {js|🥪(🦃🦆🐓)|js},
       |],
     )
});

test("Item.Sandwich.toPrice", () => {
  // 14 Feb 2024 is a Wednesday
  let date = Js.Date.makeWithYMD(~year=2024., ~month=1., ~date=14.);

  expect
  |> deepEqual(
       [|Portabello, Ham, Unicorn, Turducken|]
       |> Js.Array.map(~f=Item.Sandwich.toPrice(~date)),
       [|10., 10., 80., 20.|],
     );
});

test("Item.Sandwich.toPrice returns lower price for Turducken on Tuesdays", () => {
  // Make an array of all dates in a single week; 1 Jan 2024 is a Monday
  let dates =
    [|1., 2., 3., 4., 5., 6., 7.|]
    |> Js.Array.map(~f=date =>
         Js.Date.makeWithYMD(~year=2024., ~month=0., ~date)
       );

  expect
  |> deepEqual(
       dates
       |> Js.Array.map(~f=date => Item.Sandwich.toPrice(Turducken, ~date)),
       [|20., 10., 20., 20., 20., 20., 20.|],
     );
});

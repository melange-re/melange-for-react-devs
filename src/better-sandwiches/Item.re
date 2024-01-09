type sandwich =
  | Portabello
  | Ham
  | Unicorn
  | Turducken;

type t =
  | Sandwich(sandwich)
  | Burger
  | Hotdog;

let toPrice = t => {
  let day = Js.Date.make() |> Js.Date.getDay |> int_of_float;

  switch (t) {
  | Sandwich(Portabello | Ham) => 10.
  | Sandwich(Unicorn) => 80.
  | Sandwich(Turducken) when day == 2 => 10.
  | Sandwich(Turducken) => 20.
  | Burger => 15.
  | Hotdog => 5.
  };
};

let toEmoji =
  fun
  | Burger => {js|🍔|js}
  | Hotdog => {js|🌭|js}
  | Sandwich(sandwich) =>
    // todo: Put 🥪 in format string after Melange 3 is released
    // https://github.com/melange-re/melange-for-react-devs/issues/12
    Printf.sprintf(
      "%s(%s)",
      {js|🥪|js},
      switch (sandwich) {
      | Portabello => {js|🍄|js}
      | Ham => {js|🐷|js}
      | Unicorn => {js|🦄|js}
      | Turducken => {js|🦃🦆🐓|js}
      },
    );

// #region sandwich-type
type sandwich =
  | Portabello
  | Ham
  | Unicorn;

type t =
  | Sandwich(sandwich)
  | Burger
  | Hotdog;
// #endregion sandwich-type

let _ = {
  // #region to-price-nested-switch
  let toPrice =
    fun
    | Sandwich(sandwich) =>
      switch (sandwich) {
      | Portabello => 7.
      | Ham => 10.
      | Unicorn => 80.
      }
    | Burger => 15.
    | Hotdog => 5.;
  // #endregion to-price-nested-switch
  toPrice(Hotdog);
};

let _ = {
  // #region to-price-flat
  let toPrice =
    fun
    | Sandwich(Portabello) => 7.
    | Sandwich(Ham) => 10.
    | Sandwich(Unicorn) => 80.
    | Burger => 15.
    | Hotdog => 5.;
  // #endregion to-price-flat
  toPrice(Hotdog);
};

let _ = {
  // #region to-price-same-price
  let toPrice =
    fun
    | Sandwich(Portabello) => 10.
    | Sandwich(Ham) => 10.
    | Sandwich(Unicorn) => 80.
    | Burger => 15.
    | Hotdog => 5.;
  // #endregion to-price-same-price
  toPrice(Hotdog);
};

let _ = {
  // #region to-price-combine
  let toPrice =
    fun
    | Sandwich(Portabello)
    | Sandwich(Ham) => 10.
    | Sandwich(Unicorn) => 80.
    | Burger => 15.
    | Hotdog => 5.;
  // #endregion to-price-combine
  toPrice(Hotdog);
};

// #region to-price
let toPrice =
  fun
  | Sandwich(Portabello | Ham) => 10.
  | Sandwich(Unicorn) => 80.
  | Burger => 15.
  | Hotdog => 5.;
// #endregion to-price

let _ = {
  // #region to-emoji-joinwith
  let toEmoji =
    fun
    | Sandwich(sandwich) =>
      [|
        {js|🥪(|js},
        switch (sandwich) {
        | Portabello => {js|🍄|js}
        | Ham => {js|🐷|js}
        | Unicorn => {js|🦄|js}
        },
        ")",
      |]
      |> Js.Array.join(~sep="")
    | Burger => {js|🍔|js}
    | Hotdog => {js|🌭|js};
  // #endregion to-emoji-joinwith
  toEmoji(Hotdog);
};

let _ = {
  // #region to-emoji-short-branch-first
  let toEmoji =
    fun
    | Burger => {js|🍔|js}
    | Hotdog => {js|🌭|js}
    | Sandwich(sandwich) =>
      [|
        {js|🥪(|js},
        switch (sandwich) {
        | Portabello => {js|🍄|js}
        | Ham => {js|🐷|js}
        | Unicorn => {js|🦄|js}
        },
        ")",
      |]
      |> Js.Array.join(~sep="");
  // #endregion to-emoji-short-branch-first
  toEmoji(Hotdog);
};

let _ = {
  // #region to-emoji-j-string-literal
  let toEmoji =
    fun
    | Burger => {js|🍔|js}
    | Hotdog => {js|🌭|js}
    | Sandwich(sandwich) => {
        let emoji =
          switch (sandwich) {
          | Portabello => {js|🍄|js}
          | Ham => {js|🐷|js}
          | Unicorn => {js|🦄|js}
          };
        {j|🥪($emoji)|j};
      };
  // #endregion to-emoji-j-string-literal
  toEmoji(Hotdog);
};

// #region to-emoji-sprintf
let toEmoji =
  fun
  | Burger => {js|🍔|js}
  | Hotdog => {js|🌭|js}
  | Sandwich(sandwich) =>
    Printf.sprintf(
      {|🥪(%s)|},
      switch (sandwich) {
      | Portabello => {js|🍄|js}
      | Ham => {js|🐷|js}
      | Unicorn => {js|🦄|js}
      },
    );
// #endregion to-emoji-sprintf

module ItemAddTurducken = {
  // #region item-turducken
  type sandwich =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  type t =
    | Sandwich(sandwich)
    | Burger
    | Hotdog;

  let toPrice =
    fun
    | Sandwich(Portabello | Ham) => 10.
    | Sandwich(Unicorn) => 80.
    | Sandwich(Turducken) => 20.
    | Burger => 15.
    | Hotdog => 5.;

  let toEmoji =
    fun
    | Burger => {js|🍔|js}
    | Hotdog => {js|🌭|js}
    | Sandwich(sandwich) =>
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
  // #endregion item-turducken

  let _ = {
    // #region to-price-turducken-tuesdays
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
    // #endregion to-price-turducken-tuesdays
    toPrice(Hotdog);
  };
};

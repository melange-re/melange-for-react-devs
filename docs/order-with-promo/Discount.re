type error =
  | InvalidCode
  | ExpiredCode;

let getDiscountFunction = (code, _date) => {
  switch (code) {
  | "FREE" => Ok(_items => Ok(0.0))
  | _ => Error(InvalidCode)
  };
};

let getSandwichHalfOff = (~date as _: Js.Date.t, _items: list(Item.t)) =>
  Error(`MissingSandwichTypes([]));

type sandwichTracker = {
  portabello: bool,
  ham: bool,
  unicorn: bool,
  turducken: bool,
};

let _ =
  (~tracker, ~date, ~items) => {
    let _ =
      // #region missing-sandwich-types
      switch (tracker) {
      | {portabello: true, ham: true, unicorn: true, turducken: true} =>
        let total =
          items
          |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
               total +. Item.toPrice(item, ~date)
             );
        Ok(total /. 2.0);
      | tracker =>
        let missing =
          [
            tracker.portabello ? "" : "portabello",
            tracker.ham ? "" : "ham",
            tracker.unicorn ? "" : "unicorn",
            tracker.turducken ? "" : "turducken",
          ]
          |> List.filter((!=)(""));
        Error(`MissingSandwichTypes(missing));
      };
    // #endregion missing-sandwich-types
    ();
  };

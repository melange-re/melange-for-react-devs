type error =
  | InvalidCode
  | ExpiredCode;

/** Buy n burgers, get n/2 burgers free */
let getFreeBurgers = (items: list(Item.t)) => {
  let prices =
    items
    |> List.filter_map(item =>
         switch (item) {
         | Item.Burger(burger) => Some(Item.Burger.toPrice(burger))
         | Sandwich(_)
         | Hotdog => None
         }
       );

  switch (prices) {
  | [] => Error(`NeedTwoBurgers)
  | [_] => Error(`NeedOneBurger)
  | prices =>
    let result =
      prices
      |> List.sort((x, y) => - Float.compare(x, y))
      |> List.filteri((index, _) => index mod 2 == 1)
      |> List.fold_left((+.), 0.0);
    Ok(result);
  };
};

/** Buy 1+ burger with 1+ of every topping, get half off */
let getHalfOff = (items: list(Item.t)) => {
  let meetsCondition =
    items
    |> List.exists(
         fun
         | Item.Burger({lettuce: true, tomatoes: true, onions, cheese, bacon})
             when onions > 0 && cheese > 0 && bacon > 0 =>
           true
         | Burger(_)
         | Sandwich(_)
         | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => Error(`NeedMegaBurger)
  | true =>
    let total =
      items
      |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Ok(total /. 2.0);
  };
};

type sandwichTracker = {
  portabello: bool,
  ham: bool,
  unicorn: bool,
  turducken: bool,
};

/** Buy 1+ of every type of sandwich, get half off */
let getSandwichHalfOff = (items: list(Item.t)) => {
  let tracker =
    items
    |> List.filter_map(
         fun
         | Item.Sandwich(sandwich) => Some(sandwich)
         | Burger(_)
         | Hotdog => None,
       )
    |> ListLabels.fold_left(
         ~init={
           portabello: false,
           ham: false,
           unicorn: false,
           turducken: false,
         },
         ~f=(tracker, sandwich: Item.Sandwich.t) =>
         switch (sandwich) {
         | Portabello => {...tracker, portabello: true}
         | Ham => {...tracker, ham: true}
         | Unicorn => {...tracker, unicorn: true}
         | Turducken => {...tracker, turducken: true}
         }
       );

  switch (tracker) {
  | {portabello: true, ham: true, unicorn: true, turducken: true} =>
    let total =
      items
      |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Ok(total /. 2.0);
  | _ => Error(`MissingSandwichTypes)
  };
};

let getDiscountFunction = (code, date) => {
  let month = date |> Js.Date.getMonth;
  let dayOfMonth = date |> Js.Date.getDate;

  switch (code |> Js.String.toUpperCase) {
  | "FREE" when month == 4.0 => Ok(getFreeBurgers)
  | "HALF" when month == 4.0 && dayOfMonth == 28.0 => Ok(getHalfOff)
  | "FREE"
  | "HALF" => Error(ExpiredCode)
  | _ => Error(InvalidCode)
  };
};

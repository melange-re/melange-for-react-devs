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
  | []
  | [_] => Error("Buy at least 2 burgers")
  | prices =>
    let result =
      prices
      |> List.sort((x, y) => - Float.compare(x, y))
      |> List.filteri((index, _) => index mod 2 == 1)
      |> List.fold_left((+.), 0.0);
    Ok(result);
  };
};

// Buy 1+ burger with 1+ of every topping, get half off
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
  | false => Error("Buy a burger with at least 1 of every topping")
  | true =>
    let total =
      items
      |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Ok(total /. 2.0);
  };
};

let applyDiscount = (~code, ~items, ~date) => {
  let month = date |> Js.Date.getMonth;
  let dayOfMonth = date |> Js.Date.getDate;

  switch (code |> Js.String.toLowerCase) {
  | "free" when month == 4.0 => getFreeBurgers(items) |> Result.to_option
  | "half" when month == 4.0 && dayOfMonth == 28.0 =>
    getHalfOff(items) |> Result.to_option
  | _ => None
  };
};

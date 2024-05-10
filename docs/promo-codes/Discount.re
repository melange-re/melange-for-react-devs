let getFreeBurgers = (_items: list(Item.t)) => Some(0.0);

let getHalfOff = (_items: list(Item.t)) => Some(0.0);

// #region simple-get-discount
let getDiscountFunction = code => {
  switch (code |> Js.String.toUpperCase) {
  | "FREE" => Some(getFreeBurgers)
  | "HALF" => Some(getHalfOff)
  | _ => None
  };
};
// #endregion simple-get-discount

ignore(getDiscountFunction);

// #region get-discount-with-date
let getDiscountFunction = (code, date) => {
  let month = date |> Js.Date.getMonth;

  switch (code |> Js.String.toUpperCase) {
  | "FREE" when month == 4.0 => Some(getFreeBurgers)
  | "FREE" => None
  | "HALF" => Some(getHalfOff)
  | _ => None
  };
};
// #endregion get-discount-with-date

ignore(getDiscountFunction);

// #region get-discount-return-result
let getDiscountFunction = (code, date) => {
  let month = date |> Js.Date.getMonth;

  switch (code |> Js.String.toUpperCase) {
  | "FREE" when month == 4.0 => Ok(getFreeBurgers)
  | "FREE" => Error("Expired code")
  | "HALF" => Ok(getHalfOff)
  | _ => Error("Invalid code")
  };
};
// #endregion get-discount-return-result

ignore(getDiscountFunction);

// #region get-free-burgers
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
  | [] => Error("To enjoy this promo, buy at least 2 burgers")
  | [_] => Error("To enjoy this promo, buy at least 1 more burger")
  | prices =>
    let result =
      prices
      |> List.sort((x, y) => - Float.compare(x, y))
      |> List.filteri((index, _) => index mod 2 == 1)
      |> List.fold_left((+.), 0.0);
    Ok(result);
  };
};
// #endregion get-free-burgers

// #region get-half-off
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
  | false => Error("To enjoy this promo, buy a burger that has every topping")
  | true =>
    let total =
      items
      |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Ok(total /. 2.0);
  };
};
// #endregion get-half-off

let getDiscountFunction = (code, date) => {
  let month = date |> Js.Date.getMonth;
  let dayOfMonth = date |> Js.Date.getDate;

  switch (code |> Js.String.toUpperCase) {
  | "FREE" when month == 4.0 => Ok(getFreeBurgers)
  | "HALF" when month == 4.0 && dayOfMonth == 28.0 => Ok(getHalfOff)
  | "FREE"
  | "HALF" => Error("Code expired")
  | _ => Error("Invalid code")
  };
};

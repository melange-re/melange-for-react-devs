type error =
  | InvalidCode
  | ExpiredCode;

let getFreeBurgers = (_: list(Item.t)) => Error(`NeedTwoBurgers);

let getHalfOff = (_: list(Item.t), ~date as _: Js.Date.t) =>
  Error(`NeedMegaBurger);
let getSandwichHalfOff = getHalfOff;

// #region get-discount-function
let getDiscountFunction = (code, date) => {
  let month = date |> Js.Date.getMonth;
  let dayOfMonth = date |> Js.Date.getDate;

  switch (code |> Js.String.toUpperCase) {
  | "FREE" when month == 4.0 => Ok(getFreeBurgers)
  | "HALF" when month == 4.0 && dayOfMonth == 28.0 => Ok(getHalfOff(~date))
  | "HALF" when month == 10.0 && dayOfMonth == 3.0 =>
    Ok(getSandwichHalfOff(~date))
  | "FREE"
  | "HALF" => Error(ExpiredCode)
  | _ => Error(InvalidCode)
  };
};
// #endregion get-discount-function

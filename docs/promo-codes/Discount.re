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

let getDiscountFunction = (~code, ~date) => {
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

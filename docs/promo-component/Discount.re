type error =
  | InvalidCode
  | ExpiredCode;

let getFreeBurgers = (_: list(Item.t), ~date as _) =>
  Error(`NeedTwoBurgers);

let getHalfOff = getFreeBurgers;
let getSandwichHalfOff = getFreeBurgers;

let getDiscountFunction = (_code, _date) => Ok(_items => Ok(1.0));

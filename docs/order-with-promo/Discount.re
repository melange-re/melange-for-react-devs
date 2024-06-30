type error =
  | InvalidCode
  | ExpiredCode;

let getDiscountFunction = (code, _date) => {
  switch (code) {
  | "FREE" => Ok(_items => Ok(0.0))
  | _ => Error(InvalidCode)
  };
};

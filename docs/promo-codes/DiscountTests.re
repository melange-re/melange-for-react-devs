open Fest;

module RealDiscount = Discount;

module Discount = {
  type error =
    | ExpiredCode
    | InvalidCode;

  let getFreeBurgers = (_items: list(Item.t)) => Some(0.0);

  let getHalfOff = (_items: list(Item.t)) => Some(0.0);

  let getDiscountFunction = (code, _date) =>
    switch (code) {
    | "" => Ok(getFreeBurgers)
    | _ => Error("")
    };
};

test("Invalid promo code return Error", () => {
  let date = Js.Date.make();
  // #region ignore-list
  ["", "FREEDOM", "UNICORN", "POO"]
  |> List.map(code =>
       expect
       |> equal(
            Discount.getDiscountFunction(code, date),
            Error("Invalid code"),
          )
     )
  |> ignore; // [!code ++]
  // #endregion ignore-list
});

test("Invalid promo code return Error", () => {
  let date = Js.Date.make();
  // #region list-iter
  ["", "FREEDOM", "UNICORN", "POO"]
  |> List.iter(code =>
       expect
       |> equal(
            Discount.getDiscountFunction(code, date),
            Error("Invalid code"),
          )
     );
  // #endregion list-iter
});

// #region free-promo-may
test("FREE promo code works in May but not other months", () => {
  for (month in 0 to 11) {
    let date =
      Js.Date.makeWithYMD(
        ~year=2024.,
        ~month=float_of_int(month),
        ~date=10.,
      );

    expect
    |> deepEqual(
         Discount.getDiscountFunction("FREE", date),
         month == 4 ? Ok(Discount.getFreeBurgers) : Error("Expired code"),
       );
  }
});
// #endregion free-promo-may

{
  module Discount = RealDiscount;

  // #region free-promo-may-list-iter
  test("FREE promo code works in May but not other months", () => {
    List.init(12, i => i)
    |> List.iter(month => {
         let date =
           Js.Date.makeWithYMD(
             ~year=2024.,
             ~month=float_of_int(month),
             ~date=10.,
           );

         expect
         |> deepEqual(
              Discount.getDiscountFunction("free", date),
              month == 4 ? Ok(Discount.getFreeBurgers) : Error(ExpiredCode),
            );
       })
  });
  // #endregion free-promo-may-list-iter
};

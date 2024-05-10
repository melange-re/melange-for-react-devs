open Fest;

// #region get-discount-module
module GetDiscount = {
  test("FREE promo code works in May but not other months", () => {
    for (month in 0 to 11) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=float_of_int(month),
          ~date=10.,
        );

      expect
      |> equal(
           Discount.getDiscountFunction("FREE", date),
           month == 4 ? Ok(Discount.getFreeBurgers) : Error("Expired code"),
         );
    }
  });
};
// #endregion get-discount-module

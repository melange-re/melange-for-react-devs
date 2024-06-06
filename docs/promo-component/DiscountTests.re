open Fest;

module GetDiscount = {
  // #region test-half-promo
  test(
    "HALF promo code returns getHalfOff on May 28 but not other days of May",
    () => {
    for (dayOfMonth in 1 to 31) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=4.0,
          ~date=float_of_int(dayOfMonth),
        );

      expect
      |> deepEqual(
           Discount.getDiscountFunction("HALF", date),
           dayOfMonth == 28
             ? Ok(Discount.getHalfOff(~date)) : Error(ExpiredCode),
         );
    }
  });
  // #endregion test-half-promo
};

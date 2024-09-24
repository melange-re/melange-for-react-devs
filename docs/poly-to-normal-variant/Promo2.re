// #region model-module
module Model = {
  type t('a) =
    | CodeError(Discount.error)
    | Discount(float)
    | DiscountError('a)
    | NoSubmittedCode;
};
// #endregion model-module

let _ =
  (submittedCode, date, items) => {
    // #region namespace-constructor
    let discount =
      switch (submittedCode) {
      | None => Model.NoSubmittedCode
      | Some(code) =>
        switch (Discount.getDiscountFunction(code, date)) {
        | Error(error) => CodeError(error)
        | Ok(discountFunction) =>
          switch (discountFunction(items)) {
          | Error(error) => DiscountError(error)
          | Ok(value) => Discount(value)
          }
        }
      };
    // #endregion namespace-constructor

    ignore(discount);
  };

let _ =
  (submittedCode, date, items) => {
    // #region annotate-constructor
    let discount =
      switch (submittedCode) {
      | None => (NoSubmittedCode: Model.t('a))
      | Some(code) =>
        switch (Discount.getDiscountFunction(code, date)) {
        | Error(error) => CodeError(error)
        | Ok(discountFunction) =>
          switch (discountFunction(items)) {
          | Error(error) => DiscountError(error)
          | Ok(value) => Discount(value)
          }
        }
      };
    // #endregion annotate-constructor

    ignore(discount);
  };

let _ =
  (submittedCode, date, items) => {
    // #region annotate-variable
    let discount: Model.t('a) =
      switch (submittedCode) {
      | None => NoSubmittedCode
      | Some(code) =>
        switch (Discount.getDiscountFunction(code, date)) {
        | Error(error) => CodeError(error)
        | Ok(discountFunction) =>
          switch (discountFunction(items)) {
          | Error(error) => DiscountError(error)
          | Ok(value) => Discount(value)
          }
        }
      };
    // #endregion annotate-variable

    ignore(discount);
  };

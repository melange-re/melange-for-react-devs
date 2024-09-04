[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t, ~onApply as _) =>
  <div />;

type discount('a) =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError('a)
  | NoSubmittedCode;

module Style = {
  let codeError = "";
  let discountError = "";
  let form = "";
};

[@warning "-27"]
module AddOnApply = {
  // #region add-on-apply
  [@react.component]
  let make =
      (~items: list(Item.t), ~date: Js.Date.t, ~onApply: float => unit) =>
    // #endregion add-on-apply
    <div />;
};

let _ =
  (setSubmittedCode, onApply, code, date, items) => {
    // #region switch-in-on-submit
    <form
      className=Style.form
      onSubmit={evt => {
        evt |> React.Event.Form.preventDefault;
        let newSubmittedCode =
          Js.String.trim(code) == "" ? None : Some(code);
        setSubmittedCode(newSubmittedCode);

        switch (newSubmittedCode) {
        | None => ()
        | Some(code) =>
          switch (Discount.getDiscountFunction(code, date)) {
          | Error(_) => ()
          | Ok(discountFunction) =>
            switch (discountFunction(items)) {
            | Error(_) => ()
            | Ok(value) => onApply(value)
            }
          }
        };
      }}>
      // #endregion switch-in-on-submit
       React.null </form>;
  };

let _ =
  (submittedCode, date, items) => {
    // #region discount-derived-variable
    let discount =
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
    // #endregion discount-derived-variable
    ignore(discount);
  };

let _ =
  (date, items) => {
    // #region get-discount
    let getDiscount =
      fun
      | None => NoSubmittedCode
      | Some(code) =>
        switch (Discount.getDiscountFunction(code, date)) {
        | Error(error) => CodeError(error)
        | Ok(discountFunc) =>
          switch (discountFunc(items)) {
          | Error(error) => DiscountError(error)
          | Ok(value) => Discount(value)
          }
        };
    // #endregion get-discount

    ignore(getDiscount);
  };

let _ =
  (getDiscount, onApply, code, setSubmittedCode) => {
    <form
      // #region on-submit-using-get-discount
      onSubmit={evt => {
        evt |> React.Event.Form.preventDefault;
        let newSubmittedCode = Some(code);
        setSubmittedCode(newSubmittedCode);
        switch (getDiscount(newSubmittedCode)) {
        | NoSubmittedCode
        | CodeError(_)
        | DiscountError(_) => ()
        | Discount(value) => onApply(value)
        };
      }}
      // #endregion on-submit-using-get-discount
    />;
  };

let _ =
  (getDiscount, submittedCode) => {
    // #region get-discount-in-render
    switch (getDiscount(submittedCode)) {
    | NoSubmittedCode => React.null
    // ...
    // #endregion get-discount-in-render
    | _ => React.null
    };
  };

let _ =
  (~thing) => {
    switch (thing) {
    // #region show-missing-sandwich-types
    | `DiscountError(code) =>
      let buyWhat =
        switch (code) {
        | `NeedOneBurger => "at least 1 more burger"
        | `NeedTwoBurgers => "at least 2 burgers"
        | `NeedMegaBurger => "a burger with every topping"
        | `MissingSandwichTypes(missing) =>
          (missing |> Stdlib.Array.of_list |> Js.Array.join(~sep=", "))
          ++ " sandwiches"
        };
      <div className=Style.discountError>
        {RR.s({j|Buy $buyWhat to enjoy this promotion|j})}
      </div>;
    // #endregion show-missing-sandwich-types
    };
  };

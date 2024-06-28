[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t, ~onApply as _) =>
  <div />;

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
  (discount, onApply) => {
    // #region use-effect
    React.useEffect1(
      () => {
        switch (discount) {
        | `NoSubmittedCode
        | `CodeError(_)
        | `DiscountError(_) => ()
        | `Discount(value) => onApply(value)
        };
        None;
      },
      [|discount|],
    );
    // #endregion use-effect
    ();
  };

let _ =
  (discount, onApply) => {
    // #region use-effect-helper
    RR.useEffect1(
      () => {
        switch (discount) {
        | `NoSubmittedCode
        | `CodeError(_)
        | `DiscountError(_) => ()
        | `Discount(value) => onApply(value)
        };
        None;
      },
      discount,
    );
    // #endregion use-effect-helper
    ();
  };

let _ =
  (discount, onApply) => {
    // #region log
    RR.useEffect1(
      () => {
        switch (discount) {
        | `NoSubmittedCode
        | `CodeError(_)
        | `DiscountError(_) => ()
        | `Discount(value) =>
          Js.log2("useEffect1 depending on discount", value);
          onApply(value);
        };
        None;
      },
      discount,
    );
    // #endregion log
    ();
  };

let _ =
  (discount, submittedCode, onApply) => {
    // #region submitted-code-dep
    RR.useEffect1(
      () => {
        switch (discount) {
        | `NoSubmittedCode
        | `CodeError(_)
        | `DiscountError(_) => ()
        | `Discount(value) =>
          Js.log2("useEffect1 depending on discount", value);
          onApply(value);
        };
        None;
      },
      submittedCode,
    );
    // #endregion submitted-code-dep
    ();
  };

let _ =
  (date, items) => {
    // #region get-discount
    let getDiscount =
      fun
      | None => `NoSubmittedCode
      | Some(code) =>
        switch (Discount.getDiscountFunction(code, date)) {
        | Error(error) => `CodeError(error)
        | Ok(discountFunc) =>
          switch (discountFunc(items)) {
          | Error(error) => `DiscountError(error)
          | Ok(value) => `Discount(value)
          }
        };
    // #endregion get-discount

    ignore(getDiscount);
  };

let _ =
  (getDiscount, onApply, code, setSubmittedCode) => {
    <form
      // #region on-submit
      onSubmit={evt => {
        evt |> React.Event.Form.preventDefault;
        let newSubmittedCode = Some(code);
        setSubmittedCode(newSubmittedCode);
        switch (getDiscount(newSubmittedCode)) {
        | `NoSubmittedCode
        | `CodeError(_)
        | `DiscountError(_) => ()
        | `Discount(value) => onApply(value)
        };
      }}
      // #endregion on-submit
    />;
  };

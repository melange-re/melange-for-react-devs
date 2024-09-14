[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t, ~onApply as _) =>
  <div />;

type discount('a) =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError('a)
  | NoSubmittedCode;

let _ =
  (submittedCode, date, items) => {
    // #region discount-variant
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
    // #endregion discount-variant

    ignore(discount);
  };

module Style = {
  let codeError = "";
  let discountError = "";
};

let _ =
  discount => {
    <>
      // #region discount-render
      {switch (discount) {
       | NoSubmittedCode => React.null
       | Discount(discount) => discount |> Float.neg |> RR.currency
       | CodeError(error) =>
         <div className=Style.codeError>
           {let errorType =
              switch (error) {
              | Discount.InvalidCode => "Invalid"
              | ExpiredCode => "Expired"
              };
            {j|$errorType promo code|j} |> RR.s}
         </div>
       | DiscountError(code) =>
         let buyWhat =
           switch (code) {
           | `NeedOneBurger => "at least 1 more burger"
           | `NeedTwoBurgers => "at least 2 burgers"
           | `NeedMegaBurger => "a burger with every topping"
           | `MissingSandwichTypes => "every sandwich"
           };
         <div className=Style.discountError>
           {RR.s({j|Buy $buyWhat to enjoy this promotion|j})}
         </div>;
       }}
      // #endregion discount-render
    </>;
  };

let _ =
  (submittedCode, date, items) => {
    // #region when-solution
    let discount =
      switch (submittedCode) {
      | None => NoSubmittedCode
      | Some(code) when Js.String.trim(code) == "" => NoSubmittedCode
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
    // #endregion when-solution

    ignore(discount);
  };

let _ =
  (code, setSubmittedCode) => {
    // #region onsubmit-solution
    <form
      onSubmit={evt => {
        evt |> React.Event.Form.preventDefault;
        setSubmittedCode(Js.String.trim(code) == "" ? None : Some(code));
      }}>
      // #endregion onsubmit-solution
       {RR.s("")} </form>;
  };

// #region model-module
module Model = {
  type t('a) =
    | CodeError(Discount.error)
    | Discount(float)
    | DiscountError([> ] as 'a)
    | NoSubmittedCode;
};
// #endregion model-module

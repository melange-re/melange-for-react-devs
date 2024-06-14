// Did not implement onApply yet

module Style = {
  let form = [%cx
    {|
    display: flex;
    flex-direction: column;
    align-items: flex-end;
    gap: 0.5em;
    width: 15em;
    |}
  ];
  let input = [%cx
    {|
    width: 8em;
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];

  let discount = [%cx {|color: green;|}];
  let promoError = [%cx {|color: red|}];
  let discountError = [%cx {|color: purple|}];
  let buyWhat = [%cx {|text-decoration: underline|}];
};

// type discount('a) =
//   | CodeError(Discount.error)
//   | Discount(float)
//   | DiscountError(
//       [>
//         | `MissingSandwichTypes(list(string))
//         | `NeedMegaBurger
//         | `NeedOneBurger
//         | `NeedTwoBurgers
//       ] as 'a,
//     )
//   | NoSubmittedCode;

type discount('a) =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError([> ] as 'a)
  | NoSubmittedCode;

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t, ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let getDiscount =
    fun
    | None => NoSubmittedCode
    | Some(code) =>
      switch (Discount.getDiscountFunction(~code, ~date)) {
      | Error(error) => CodeError(error)
      | Ok(discountFunc) =>
        switch (discountFunc(items)) {
        | Error(error) => DiscountError(error)
        | Ok(value) => Discount(value)
        }
      };

  <form
    className=Style.form
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
    }}>
    <input
      className=Style.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {switch (getDiscount(submittedCode)) {
     | NoSubmittedCode => React.null
     | Discount(discount) =>
       <div className=Style.discount>
         {discount |> Float.neg |> RR.currency}
       </div>
     | CodeError(error) =>
       <div className=Style.promoError>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     | DiscountError(code) =>
       <div className=Style.discountError>
         {RR.s("Buy ")}
         <span className=Style.buyWhat>
           {(
              switch (code) {
              | `NeedOneBurger => "at least 1 more burger"
              | `NeedTwoBurgers => "at least 2 burgers"
              | `NeedMegaBurger => "a burger with every topping"
              | `MissingSandwichTypes(missing) =>
                (missing |> Stdlib.Array.of_list |> Js.Array.join(~sep=", "))
                ++ " sandwiches"
              }
            )
            |> RR.s}
         </span>
         {RR.s(" to enjoy this promo.")}
       </div>
     }}
  </form>;
};

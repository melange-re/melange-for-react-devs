module Css = {
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
  let error = [%cx {|color: red;|}];
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let discountFunc =
    submittedCode
    |> Option.map(code => Discount.getDiscountFunction(~code, ~date));

  let discount =
    switch (discountFunc) {
    | None
    | Some(Error(_)) => None
    | Some(Ok(discountFunc)) => Some(discountFunc(items))
    };

  <form
    className=Css.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
    }}>
    <input
      className=Css.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {switch (discountFunc) {
     | None
     | Some(Ok(_)) => React.null
     | Some(Error(error)) =>
       <div className=[%cx {|color: red|}]>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     }}
    {switch (discount) {
     | None => React.null
     | Some(Ok(value)) => value |> Float.neg |> string_of_float |> RR.s
     | Some(Error(_code)) =>
       <div className=[%cx {|color: purple|}]>
         {RR.s("Todo: discount error message")}
       </div>
     }}
  </form>;
};

module Style = {
  let form = [%cx {|
    display: flex;
    flex-direction: column;
  |}];

  let input = [%cx
    {|
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];

  let error = [%cx {|color: red|}];
};

[@react.component]
let make = (~items as _: list(Item.t), ~date: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let discountFunction =
    submittedCode
    |> Option.map(code => Discount.getDiscountFunction(code, date));

  <form
    className=Style.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
    }}>
    <input
      className=Style.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {switch (discountFunction) {
     | None
     | Some(Ok(_)) => React.null
     | Some(Error(error)) =>
       <div className=Style.error>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     }}
  </form>;
};

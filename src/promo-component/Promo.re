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
};

[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");

  <form
    className=Style.form
    onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      className=Style.input
      value=code
      onChange={evt => evt |> RR.getValueFromEvent |> setCode}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};

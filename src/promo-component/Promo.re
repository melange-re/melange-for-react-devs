[@react.component]
let make = (~items as _: list(Item.t), ~date as _: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");

  <form onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      value=code
      onChange={evt => evt |> RR.getValueFromEvent |> setCode}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};

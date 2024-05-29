[@react.component]
let make = () => {
  let (code, setCode) = RR.useStateValue("");

  <form onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      value=code
      onChange={evt => evt |> RR.getValueFromEvent |> setCode}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};

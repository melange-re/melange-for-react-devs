let stringToDate = s =>
  // add "T00:00" to make sure the date is in local time
  s ++ "T00:00" |> Js.Date.fromString;

[@react.component]
let make = (~date: Js.Date.t, ~onChange: Js.Date.t => unit) => {
  <input
    type_="date"
    required=true
    value={date |> Js.Date.toISOString |> Js.String.substring(~end_=10)}
    onChange={evt => evt |> RR.getValueFromEvent |> stringToDate |> onChange}
  />;
};

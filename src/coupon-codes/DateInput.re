let fromString = s => {
  // add "T00:00" to make sure the date is in local time
  let date = s ++ "T00:00" |> Js.Date.fromString;
  date |> Js.Date.valueOf |> Js.Float.isNaN ? None : Some(date);
};

[@react.component]
let make = (~date: Js.Date.t, ~onChange: Js.Date.t => unit) => {
  <input
    type_="date"
    required=true
    value={date |> Js.Date.toISOString |> Js.String.substring(~end_=10)}
    onChange={evt =>
      evt
      |> RR.getValueFromEvent
      |> fromString
      |> Option.value(~default=date)
      |> onChange
    }
  />;
};

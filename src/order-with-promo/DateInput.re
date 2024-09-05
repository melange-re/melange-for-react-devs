let stringToDate = s =>
  // add "T00:00" to make sure the date is in local time
  s ++ "T00:00" |> Js.Date.fromString;

/* Convert a Date to the yyyy-mm-dd format that the input element accepts for
   its value attribute */
let dateToString = d =>
  Printf.sprintf(
    "%4.0f-%02.0f-%02.0f",
    Js.Date.getFullYear(d),
    Js.Date.getMonth(d) +. 1.,
    Js.Date.getDate(d),
  );

[@react.component]
let make = (~date: Js.Date.t, ~onChange: Js.Date.t => unit) => {
  <input
    type_="date"
    required=true
    value={dateToString(date)}
    onChange={evt => evt |> RR.getValueFromEvent |> stringToDate |> onChange}
  />;
};

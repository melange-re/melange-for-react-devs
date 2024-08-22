[@react.component]
let make = (~value: int, ~disabled, ~onChange: int => unit) =>
  <input
    type_="number"
    disabled
    value={value |> string_of_int}
    onChange={evt =>
      evt
      |> RR.getValueFromEvent
      |> int_of_string_opt
      |> Option.iter(onChange)
    }
  />;

// Solution to exercise 3

let getValueFromEvent = (evt): string => React.Event.Form.target(evt)##value;

let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

let floatFromString = text => {
  let value = Js.Float.fromString(text);
  Js.Float.isNaN(value) ? None : Some(value);
};

[@react.component]
let make = () => {
  let (celsius, setCelsius) = React.useState(() => "");

  <div>
    <input
      value=celsius
      onChange={evt => {
        let newCelsius = getValueFromEvent(evt);
        setCelsius(_ => newCelsius);
      }}
    />
    {React.string({js|°C = |js})}
    {(
       String.trim(celsius) == ""
         ? {js|?°F|js}
         : (
           switch (celsius |> floatFromString |> Option.map(convert)) {
           | None => "error"
           | Some(fahrenheit) when fahrenheit < (-128.6) => {js|Unreasonably cold🥶|js}
           | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
           | Some(fahrenheit) =>
             Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js|°F|js}
           }
         )
     )
     |> React.string}
  </div>;
};

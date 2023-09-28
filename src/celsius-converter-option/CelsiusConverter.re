let getValueFromEvent = (evt): string => ReactEvent.Form.target(evt)##value;

let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

[@react.component]
let make = () => {
  let (celsius, setCelsius) = React.useState(() => "");

  <div>
    <input value=celsius onChange={evt => setCelsius(_ => getValueFromEvent(evt))} />
    {React.string({js|°C = |js})}
    {(
       String.trim(celsius) == ""
         ? {js|? °F|js}
         : (
           switch (celsius |> float_of_string_opt |> Option.map(convert)) {
           | None => "error"
           | Some(fahrenheit) when fahrenheit < (-128.6) => {js|Unreasonably cold🥶|js}
           | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
           | Some(fahrenheit) => Js.Float.toFixedWithPrecision(fahrenheit, ~digits=2) ++ {js| °F|js}
           }
         )
     )
     |> React.string}
  </div>;
};

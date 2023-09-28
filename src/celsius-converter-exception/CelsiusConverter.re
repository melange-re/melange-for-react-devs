let getValueFromEvent = (evt): string => ReactEvent.Form.target(evt)##value;

let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

[@react.component]
let make = () => {
  let (celsius, setCelsius) = React.useState(() => "");

  <div>
    <input value=celsius onChange={evt => setCelsius(_ => getValueFromEvent(evt))} />
    {React.string({js|°C = |js})}
    {(
       celsius == ""
         ? {js|? °F|js}
         : (
           switch (celsius |> float_of_string |> convert |> Js.Float.toFixedWithPrecision(~digits=2)) {
           | exception _ => "error"
           | fahrenheit => fahrenheit ++ {js| °F|js}
           }
         )
     )
     |> React.string}
  </div>;
};

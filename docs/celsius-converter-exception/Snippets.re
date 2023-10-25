module CelsiusConverterOriginal = {
  // #region celsius-converter-v1
  let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

  [@react.component]
  let make = () => {
    let (celsius, setCelsius) = React.useState(() => "");

    <div>
      <input
        value=celsius
        onChange={evt => {
          let newCelsius = ReactEvent.Form.target(evt)##value;
          setCelsius(_ => newCelsius);
        }}
      />
      {React.string({js|°C = |js})}
      {React.string({js|?°F|js})}
    </div>;
  };
  // #endregion celsius-converter-v1
};

module CelsiusConverterGetValueFromEvent = {
  // #region get-value-from-event
  let getValueFromEvent = (evt): string =>
    ReactEvent.Form.target(evt)##value;

  let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

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
      {React.string({js|?°F|js})}
    </div>;
  };
  // #endregion get-value-from-event
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region pipes
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {celsius |> float_of_string |> convert |> string_of_float |> React.string}
  </div>;
  // #endregion pipes
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region string-concat
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(celsius |> float_of_string |> convert |> string_of_float)
     ++ {js|°F|js}
     |> React.string}
  </div>;
  // #endregion string-concat
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region catch-exception
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       switch (celsius |> float_of_string |> convert |> string_of_float) {
       | exception _ => "error"
       | fahrenheit => fahrenheit ++ {js|°F|js}
       }
     )
     |> React.string}
  </div>;
  // #endregion catch-exception
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region catch-failure
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       switch (celsius |> float_of_string |> convert |> string_of_float) {
       | exception (Failure(_)) => "error"
       | fahrenheit => fahrenheit ++ {js|°F|js}
       }
     )
     |> React.string}
  </div>;
  // #endregion catch-failure
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region ternary
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       celsius == ""
         ? {js|?°F|js}
         : (
           switch (celsius |> float_of_string |> convert |> string_of_float) {
           | exception _ => "error"
           | fahrenheit => fahrenheit ++ {js|°F|js}
           }
         )
     )
     |> React.string}
  </div>;
  // #endregion ternary
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region if-else
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       if (celsius == "") {
         {js|?°F|js};
       } else {
         switch (celsius |> float_of_string |> convert |> string_of_float) {
         | exception _ => "error"
         | fahrenheit => fahrenheit ++ {js|°F|js}
         };
       }
     )
     |> React.string}
  </div>;
  // #endregion if-else
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region fixed-precision
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       celsius == ""
         ? {js|?°F|js}
         : (
           switch (celsius |> float_of_string |> convert) {
           | exception _ => "error"
           | fahrenheit =>
             Js.Float.toFixedWithPrecision(fahrenheit, ~digits=2)
             ++ {js|°F|js}
           }
         )
     )
     |> React.string}
  </div>;
  // #endregion fixed-precision
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region partial-application
  <div>
    <input /* ... */ />
    {React.string({js|°C = |js})}
    {(
       celsius == ""
         ? {js|?°F|js}
         : (
           switch (
             celsius
             |> float_of_string
             |> convert
             |> Js.Float.toFixedWithPrecision(~digits=2)
           ) {
           | exception _ => "error"
           | fahrenheit => fahrenheit ++ {js|°F|js}
           }
         )
     )
     |> React.string}
  </div>;
  // #endregion partial-application
};

let _ = {
  let celsius = "";
  let setCelsius = _ => ();
  let getValueFromEvent = _ => "";
  // #region single-expression
  <input
    value=celsius
    onChange={evt => setCelsius(_ => getValueFromEvent(evt))}
  />;
  // #endregion single-expression
};

let _ = {
  let celsius = "";
  let setCelsius = _ => ();
  let getValueFromEvent = _ => "";
  // #region single-expression-annotated
  <input
    value=celsius
    onChange={evt =>
      setCelsius(_ => {
        // we are no longer in the scope of onChange
        getValueFromEvent(evt)
      })
    }
  />;
  // #endregion single-expression-annotated
};

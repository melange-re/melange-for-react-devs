let currency = value =>
  value |> Js.Float.toFixedWithPrecision(~digits=2) |> React.string;

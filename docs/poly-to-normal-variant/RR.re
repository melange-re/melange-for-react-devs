let s = React.string;

let currency = value => value |> Js.Float.toFixed(~digits=2) |> React.string;

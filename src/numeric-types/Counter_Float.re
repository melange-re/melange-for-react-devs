[@react.component]
let make = () => {
  let (counter, setCounter) = React.useState(() => 0.0);

  <div
    style={ReactDOMStyle.make(
      ~padding="1em",
      ~display="flex",
      ~gridGap="1em",
      (),
    )}>
    <button onClick={_evt => setCounter(v => v -. 0.5)}>
      {React.string("-")}
    </button>
    <span> {counter |> Float.to_string |> React.string} </span>
    <button onClick={_evt => setCounter(v => v +. 1.5)}>
      {React.string("+")}
    </button>
  </div>;
};

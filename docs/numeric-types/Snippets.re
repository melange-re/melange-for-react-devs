// #region counter
module Counter = {
  [@react.component]
  let make = () => {
    let (counter, setCounter) = React.useState(() => 0);

    <div
      style={ReactDOM.Style.make(
        ~padding="1em",
        ~display="flex",
        ~gridGap="1em",
        (),
      )}>
      <button onClick={_evt => setCounter(v => v - 1)}>
        {React.string("-")}
      </button>
      <span> {counter |> Int.to_string |> React.string} </span>
      <button onClick={_evt => setCounter(v => v + 1)}>
        {React.string("+")}
      </button>
    </div>;
  };
};

switch (ReactDOM.querySelector("#preview")) {
| None => Js.log("Failed to start React: couldn't find the #preview element")
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <Counter />);
};
// #endregion counter

// #region styles
module Styles = {
  // Alias the function to save on keystrokes
  let make = ReactDOM.Style.make;

  let root =
    make(
      ~fontSize="2em",
      ~padding="1em",
      ~display="flex",
      ~gridGap="1em",
      ~alignItems="center",
      (),
    );

  let button =
    make(
      ~fontSize="1em",
      ~border="1px solid white",
      ~borderRadius="0.5em",
      ~padding="0.5em",
      (),
    );

  let number = make(~minWidth="2em", ~textAlign="center", ());
};
// #endregion styles

let _ = {
  let setCounter = _ => ();
  let counter = 0;
  // #region render-with-styles
  <div style=Styles.root>
    <button style=Styles.button onClick={_evt => setCounter(v => v - 1)}>
      {React.string("-")}
    </button>
    <span style=Styles.number>
      {counter |> Int.to_string |> React.string}
    </span>
    <button style=Styles.button onClick={_evt => setCounter(v => v + 1)}>
      {React.string("+")}
    </button>
  </div>;
  // #endregion render-with-styles
};

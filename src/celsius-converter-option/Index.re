module App = {
  [@react.component]
  let make = () =>
    <div>
      <h1> {React.string("Celsius Converter using Option")} </h1>
      <CelsiusConverter />
      <h2> {React.string("Using Float.fromString")} </h2>
      <CelsiusConverter_FloatFromString />
    </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) => ReactDOM.render(<App />, root)
};

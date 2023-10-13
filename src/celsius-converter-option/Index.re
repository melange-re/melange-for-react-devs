module App = {
  [@react.component]
  let make = () =>
    <div>
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

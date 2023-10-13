module App = {
  [@react.component]
  let make = () =>
    <div>
      <Counter />
      <h2> {React.string("Counter using floats")} </h2>
      <Counter_Float />
    </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) => ReactDOM.render(<App />, root)
};

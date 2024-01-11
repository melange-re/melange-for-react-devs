module App = {
  [@react.component]
  let make = () =>
    <div>
      <h1> {React.string("Counter using float")} </h1>
      <Counter_Float />
    </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) => ReactDOM.render(<App />, root)
};

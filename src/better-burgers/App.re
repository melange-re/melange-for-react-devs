type example = {
  title: string,
  items: Order.t,
};

let examples = [|
  {
    title: "All items",
    items: [|
      Hotdog,
      Sandwich(Portabello),
      Sandwich(Unicorn),
      Sandwich(Ham),
      Sandwich(Turducken),
      Burger({lettuce: true, tomatoes: true, onions: 2, cheese: 3, bacon: 4}),
    |],
  },
  {
    title: "Burger examples",
    items: [|
      Burger({
        lettuce: false,
        tomatoes: false,
        onions: 0,
        cheese: 0,
        bacon: 0,
      }),
      Burger({
        lettuce: true,
        tomatoes: false,
        onions: 1,
        cheese: 1,
        bacon: 1,
      }),
      Burger({
        lettuce: false,
        tomatoes: false,
        onions: 1,
        cheese: 0,
        bacon: 0,
      }),
      Burger({
        lettuce: false,
        tomatoes: false,
        onions: 0,
        cheese: 1,
        bacon: 0,
      }),
    |],
  },
|];

[@react.component]
let make = () => {
  let (title, setTitle) =
    React.useState(() =>
      examples
      |> ArrayEx.get(0)
      |> Option.map(example => example.title)
      |> Option.value(~default="")
    );

  <div>
    <h1> {React.string("Order confirmation")} </h1>
    <div>
      {examples
       |> Js.Array.map(example =>
            <button
              key={example.title}
              style={
                example.title == title
                  ? ReactDOMStyle.make(~fontWeight="bold", ())
                  : ReactDOMStyle.make()
              }
              onClick={_ => setTitle(_ => example.title)}>
              {React.string(example.title)}
            </button>
          )
       |> React.array}
    </div>
    {switch (examples |> Js.Array.find(example => example.title == title)) {
     | None => React.null
     | Some({items, title: _}) => <Order items />
     }}
  </div>;
};

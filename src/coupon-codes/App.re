let burger =
  Item.Burger.{
    lettuce: false,
    tomatoes: false,
    onions: 0,
    cheese: 0,
    bacon: 0,
  };

let datasets = {
  [
    (
      "No burgers",
      Item.[
        Sandwich(Unicorn),
        Hotdog,
        Sandwich(Ham),
        Sandwich(Turducken),
        Hotdog,
      ],
    ),
    {
      let burger =
        Item.Burger.{
          lettuce: false,
          tomatoes: false,
          onions: 0,
          cheese: 0,
          bacon: 0,
        };
      (
        "5 burgers",
        {
          [
            Burger({...burger, tomatoes: true}),
            Burger({...burger, lettuce: true}),
            Burger({...burger, bacon: 2}),
            Burger({...burger, cheese: 3, onions: 9, tomatoes: true}),
            Burger({...burger, onions: 2}),
          ];
        },
      );
    },
    (
      "1 burger with at least one of every topping",
      [
        Hotdog,
        Burger({
          lettuce: true,
          tomatoes: true,
          onions: 1,
          cheese: 2,
          bacon: 3,
        }),
        Sandwich(Turducken),
      ],
    ),
    (
      "All sandwiches",
      [
        Sandwich(Ham),
        Hotdog,
        Sandwich(Portabello),
        Sandwich(Unicorn),
        Hotdog,
        Sandwich(Turducken),
      ],
    ),
  ];
};

let getSlug = title =>
  title
  |> Js.String.toLowerCase
  |> Js.String.replaceByRe(~regexp=[%re "/ /g"], ~replacement="-");

[@react.component]
let make = () => {
  let (date, setDate) =
    RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    <DateInput date onChange=setDate />
    <h2> {RR.s("PromoEffect")} </h2>
    {datasets
     |> List.map(((label, items)) => {
          let slug = "promo-effect-" ++ getSlug(label);
          <div key=slug>
            <a name=slug href={"#" ++ slug}> <h2> {label |> RR.s} </h2> </a>
            <PromoEffect items date onApply={value => Js.log(value)} />
          </div>;
        })
     |> RR.list}
    // <h2> {RR.s("PromoPure")} </h2>
    // {datasets
    //  |> List.map(((label, items)) => {
    //       let slug = "promo-pure-" ++ getSlug(label);
    //       <div key=slug>
    //         <a name=slug href={"#" ++ slug}> <h2> {label |> RR.s} </h2> </a>
    //         <PromoPure items date />
    //       </div>;
    //     })
    //  |> RR.list}
    <h2> {RR.s("Promo")} </h2>
    {datasets
     |> List.map(((label, items)) => {
          let slug = "promo-" ++ getSlug(label);
          <div key=slug>
            <a name=slug href={"#" ++ slug}> <h2> {label |> RR.s} </h2> </a>
            <Promo items date onApply={value => Js.log(value)} />
          </div>;
        })
     |> RR.list}
    <h2> {RR.s("Order")} </h2>
    {datasets
     |> List.map(((label, items)) => {
          let slug = label |> getSlug;
          <div key={"order-" ++ slug}>
            <a name=slug href={"#" ++ slug}> <h2> {label |> RR.s} </h2> </a>
            <Order items date />
          </div>;
        })
     |> RR.list}
  </div>;
};

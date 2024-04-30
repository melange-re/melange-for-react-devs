type view =
  | Menu
  | Order;

[@react.component]
let make = () => {
  let (view, setView) = RR.useStateValue(Menu);
  let (items: list(Item.t), setItems) = RR.useStateValue([]);

  <div>
    {switch (view) {
     | Menu =>
       <Menu
         items
         onSubmit={order => {
           setItems(order);
           setView(Order);
         }}
       />
     | Order => <Order items onClose={() => setView(Menu)} />
     }}
  </div>;
};

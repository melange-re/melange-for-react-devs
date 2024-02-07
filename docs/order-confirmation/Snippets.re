let _ = {
  // #region react-array-demo
  let elemArray: array(React.element) =
    [|"a", "b", "c"|] |> Js.Array.map(~f=x => React.string(x));
  Js.log(elemArray);
  Js.log(React.array(elemArray));
  // #endregion react-array-demo
};

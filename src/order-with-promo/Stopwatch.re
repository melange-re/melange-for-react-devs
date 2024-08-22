module Voice = SpeechSynthesis.Voice;

let getVoices = () => {
  SpeechSynthesis.getVoices()
  |> Promise.map(voices =>
       voices
       |> List.filter(v =>
            v |> Voice.getLang |> Js.String.startsWith(~prefix="en-")
          )
     );
};

module Inner = {
  [@react.component]
  let make = (~voice as initVoice: Voice.t, ~voices: list(Voice.t)) => {
    let (voice, setVoice) = RR.useStateValue(initVoice);

    <div>
      <select>
        {voices
         |> List.map(voice => {
              let name = voice |> Voice.getName;
              <option key=name value=name> {name |> RR.s} </option>;
            })
         |> RR.list}
      </select>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([]);

  React.useEffect0(() => {
    getVoices() |> Promise.iter(setVoices);
    None;
  });

  switch (voices) {
  | [] => React.null
  | [voice, ..._] => <Inner voice voices />
  };
};
